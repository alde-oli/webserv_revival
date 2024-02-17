#include "../../include/client/Client.hpp"
#include "../../include/config/ServConfig.hpp"
#include "../../include/client/Request.hpp"
#include "../../include/client/Response.hpp"
#include "../../include/webserv.hpp"


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

//copy assignment operator
Client	&Client::operator=(const Client &c)
{
	_clientFd = c._clientFd;
	_serverFd = c._serverFd;
	_clientAddr = c._clientAddr;
	_lastActivity = c._lastActivity;
	_rawRequest = c._rawRequest;
	_EOHFound = c._EOHFound;
	_bodyToRead = c._bodyToRead;
	_request = c._request;
	_response = c._response;
	return (*this);
}

std::ostream	&operator<<(std::ostream &os, const Client &c)
{
	os << "[][]#-----Client identity-----#[][]" << std::endl;
	os << "Address: " << inet_ntoa(c._clientAddr.sin_addr) << ":" << ntohs(c._clientAddr.sin_port) << std::endl;
	os << "socket fd: " << c._clientFd.get() << " | ";
	os << "last activity: " << std::ctime(&c._lastActivity) << std::endl;
	//os << c._request << std::endl;
	//os << c._response << std::endl;
	os << "[][]#--------------------------#[][]" << std::endl;

	return (os);
}

///////////
//setters//
///////////

//updates the last activity time to the current time
void	Client::updateActivity()
	{_lastActivity = std::time(0);}

//sets the client socket fd
void	Client::setClientFd(int fd)
	{_clientFd.set(fd);}

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

void	Client::printRequest()
	{READLOG(_request)}

////////////////////
//member functions//
////////////////////

//reads the client request and processes it into _response
//return: true if client need to be closed else false
bool Client::read(ServConfig &server, int kq)
{
	updateActivity();

	const std::string EOHeader = "\r\n\r\n"; // End of header
	char buff[BUF_SIZE]; // Stores data read by recv
	ssize_t bytesRead;
	READLOG("Reading header")
	READLOG("EOHFound: " << _EOHFound)
	// Try to find end of header if not already found
	if (!_EOHFound) {
		bytesRead = recv(_clientFd.get(), buff, BUF_SIZE, 0);
		if (bytesRead <= 0) {
			if (bytesRead == 0) std::cout << "Client closed connection" << std::endl;
			else READLOG("recv() fail, closing client")
			return true; // True indicates an error or closed connection
		}
		_rawRequest.append(buff, bytesRead);

		std::string::size_type pos = _rawRequest.find(EOHeader);
		if (pos != std::string::npos) {
			_EOHFound = true;
			if (std::cout << "hello1" << std::endl && _request.buildHeader(_rawRequest.substr(0, pos))) {
				_response.setCode(400); // Bad request
				return false;
			}
			_rawRequest.erase(0, pos + EOHeader.length());

			// If request is POST, check for Content-Length
			if (_request["method"] == "POST" && _request.isHeader("Content-Length")) {
				_bodyToRead = std::atoi(_request["Content-Length"].c_str());
				if (_bodyToRead > static_cast<size_t>(server.getMaxBodySize())) {
					_response.setCode(413); // Payload Too Large
					return false;
				}
			} else {
				// No body expected, handle request
				bool ret = !_request.handle(server, _response);
				_request.clear();
				setWriteEvent(kq);
				return ret;
			}
		}
		else
			return false; // Continue reading
	}
	READLOG("Reading body")
	READLOG("EOHFound: " << _EOHFound)
	// Handle POST requests with expected body
	if (_EOHFound && _bodyToRead > 0)
	{
		if (_rawRequest.size() >= _bodyToRead) {
			if (_request.buildBody(_rawRequest.substr(0, _bodyToRead)))
				{ _response.setCode(400); return false;}
			// Request has been fully read, handle it
			_EOHFound = false; // Reset for the next request
			_bodyToRead = 0; // Reset body length for the next request
			bool ret = !_request.handle(server, _response);
			_rawRequest.clear();
			_request.clear();
			setWriteEvent(kq);
			return ret;
		}
		bytesRead = recv(_clientFd.get(), buff, BUF_SIZE, 0);
		if (bytesRead <= 0) {
			if (bytesRead == 0) std::cout << "Client closed connection during body read" << std::endl;
			else std::cerr << "recv() fail during body read, closing client" << std::endl;
			return true; // Error or closed connection
		}
		_rawRequest.append(buff, bytesRead);

		if (_rawRequest.size() >= _bodyToRead) {
			if (_request.buildBody(_rawRequest.substr(0, _bodyToRead)))
				{std::cout << "hello3" << std::endl; _response.setCode(400); // Bad request
				return false;}
			std::cout << "bye bye 3" << std::endl;
			// Request has been fully read, handle it
			_EOHFound = false; // Reset for the next request
			_bodyToRead = 0; // Reset body length for the next request
			bool ret = !_request.handle(server, _response);
			_rawRequest.clear();
			_request.clear();
			setWriteEvent(kq);
			return ret;
		}
	}
	return false; // Continue reading
}
 

//writes the response to the client
//return: true if client needs to be closed, else false
bool	Client::write(int kq)
{
	WRITELOG(_response)
	updateActivity();

	bool closeClient = false;
	bool fullySent = _response.deliver(_clientFd.get());

	if (_response.getKeepAlive() == false)
		{_response.clear(); closeClient = true;}
	if (fullySent)//get ready to handle next interaction
	{
		_response.clear();
		if (unsetWriteEvent(kq))
			return true;
		else
			return closeClient;
	}
	unsetWriteEvent(kq);
	return closeClient;
}

//set WriteEvent to notify kevent we will send data to client in next kevent loop
bool Client::setWriteEvent(int kq)
{
	struct kevent evWrite;
	EV_SET(&evWrite, _clientFd.get(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &evWrite, 1, NULL, 0, NULL) < 0) {
		std::cerr << "error while registering write event, closing client" << std::endl << *this;
		return true; // Signaler une erreur
	}
	
	struct kevent evRead;
	EV_SET(&evRead, _clientFd.get(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	kevent(kq, &evRead, 1, NULL, 0, NULL);

	return false;
}

//end write event
bool Client::unsetWriteEvent(int kq)
{
	struct kevent evWrite;
	EV_SET(&evWrite, _clientFd.get(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	if (kevent(kq, &evWrite, 1, NULL, 0, NULL) < 0)
	{
		std::cerr << "error while unregistering write event, closing client" << std::endl << *this;
		return false;
	}

	struct kevent evRead;
	EV_SET(&evRead, _clientFd.get(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(kq, &evRead, 1, NULL, 0, NULL);

	return true;
}
