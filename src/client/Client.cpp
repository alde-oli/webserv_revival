#include "Client.hpp"
#include "webserv.hpp"


//////////////////////////////
//Constructor and destructor//
//////////////////////////////

//input: the fd returned by accept(), the client address, and the server socketFd it is connected to
//creates a client ready to handle events
Client::Client(int clientFd, sockaddr_in clientAddr, int serverFd)
	: _clientFd(clientFd), _serverFd(serverFd), _clientAddr(clientAddr), _lastActivity(std::time(0)), _rawRequest(""), _EOHFound(false), _bodyToRead(0)
{}

//client socket is automatically closed if open
Client::~Client()
{}

//////////////////////
//operators overload//
//////////////////////

std::ostream	&operator<<(std::ostream &os, const Client &c)
{
	os << "--------Client identity--------" << std::endl;
	os << "Address: " << inet_ntoa(c._clientAddr.sin_addr) << ":" << ntohs(c._clientAddr.sin_port) << std::endl;
	os << "socket fd: " << c._clientFd.get() << " | ";
	os << "last activity: " << std::ctime(&c._lastActivity) << std::endl;
	os << "-------------------------------" << std::endl;
	c._clientFd.~AutoFD();
	return (os);
}

///////////
//setters//
///////////

//updates the last activity time to the current time
void	Client::updateActivity()
	{_lastActivity = std::time(0);}

///////////
//getters//
///////////

//returns the server socket fd the client is connected to
int		Client::getServFd()
	{return (_serverFd);}


//returns the client address
sockaddr_in	Client::getAddr()
	{return (_clientAddr);}

//returns the last activity time
std::time_t	Client::getLastActivity()
	{return (_lastActivity);}

////////////////////
//member functions//
////////////////////

//  !!!!complex function, needs a second check!!!!

//reads the client request and processes it into _response
//return: true if client need to be closed else false
bool	Client::read(ServConfig &server, int kq)
{
	updateActivity();

	const std::string	EOHeader = "\r\n\r\n"; //end of header
	char				buff[BUF_SIZE]; //stores data read by recv
	//try to find end of header
	if (_EOHFound == false)
	{
		std::string::const_iterator it = std::search(_rawRequest.begin(), _rawRequest.end(), EOHeader.begin(), EOHeader.end());
		if (it != _rawRequest.end())//end of header found so we can extract and process the header
		{
			_EOHFound = true;
			if (_request.buildHeader(_rawRequest.substr(0, it - _rawRequest.begin())))
				return true;
			_rawRequest = _rawRequest.substr(it - _rawRequest.begin() + EOHeader.size());

			if (_request.getMethod() == "POST") //if POST we need content length to read body
				{if ( _request.getHeaders().find("Content-Length") != _request.getHeaders().end())
					{_bodyToRead = std::atoi(_request.getHeaders().find("Content-Length")->second.c_str());
					if (_bodyToRead > server.getMaxBodySize())
						{_EOHFound = false; _response.build(413, "", server); setWriteEvent(kq); return false;}}
				else //build error response
					{_EOHFound = false; _response.build(411, "", server); setWriteEvent(kq); return false;}}
			else //no body needed, handle request and reset EOHFound to handle next request
			{
				_EOHFound = false;
				bool wasError = _request.handle(server, _response);
				if (wasError || setWriteEvent(kq))
					return true;
				return false;
		}	}
		else //end of header not found, read more data
		{
			ssize_t	bytesRead = recv(_clientFd.get(), buff, BUF_SIZE, 0);
			if (bytesRead == 0) //if client closed connection before sending the header
				return true;
			else if (bytesRead < 0) //if error reading
				{std::cerr << "recv() fail, closing client" << std::endl << *this; return true;}
			_rawRequest += std::string(buff, bytesRead);
	}	}
	else if (_bodyToRead > 0) //if we need to read the body
	{
		ssize_t	bytesRead = recv(_clientFd.get(), buff, BUF_SIZE, 0);
		if (bytesRead == 0) //if client closed connection before sending the body
			return true;
		else if (bytesRead < 0) //if error reading
			{std::cerr << "recv() fail, closing client" << std::endl << *this; return true;}
		_rawRequest += std::string(buff, bytesRead);
		if (_rawRequest.size() >= _bodyToRead)
		{
			_request.buildBody(_rawRequest.substr(0, _bodyToRead));
			_rawRequest = _rawRequest.substr(_bodyToRead);
			bool wasError = _request.handle(server, _response);
			if (wasError || setWriteEvent(kq))
				return true;
			_EOHFound = false;
			_bodyToRead = 0;
	}	}
	else
		return true; //unexpected situation, fct should never go there
	return false;
} 

//writes the response to the client
//return: true if client needs to be closed, else false
bool	Client::write(int kq)
{
	updateActivity();

	bool closeClient = false;
	bool fullySent = _response.send(_clientFd.get());

	if (_response.getHeaders().find("Connection") != _response.getHeaders().end() && _response.getHeaders().find("Connection")->second == "close")
		{_response.clear(); closeClient = true;}
	if (fullySent = true)//get ready to handle next interaction
	{
		_response.clear();
		if (unsetWriteEvent(kq))
			return true;
	}
	return closeClient;
}

//set WriteEvent to notify kevent we will send data to client in next kevent loop
bool	Client::setWriteEvent(int kq)
{
	struct kevent ev;
	EV_SET(&ev, _clientFd.get(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
		{std::cerr << "error while registering write event, closing client" << std::endl << *this; return true;}
	return false;
}

//end write event
bool	Client::unsetWriteEvent(int kq)
{
    struct kevent ev;
    EV_SET(&ev, _clientFd.get(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
		{std::cerr << "error while unregistering write event, closing client" << std::endl << *this; return true;}
	return false;
}
