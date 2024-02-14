#include "Request.hpp"


//////////////////////////////
//Constructor and destructor//
//////////////////////////////

Request::Request()
	: _method(""), _uri(""), _httpVersion(""), _headers(), _rawBody(""), _refinedBody()
{}

Request::~Request()
{}

//////////////////////
//operators overload//
//////////////////////

Request		&Request::operator=(Request const &src)
{
	if (this != &src)
	{
		_method = src._method;
		_uri = src._uri;
		_httpVersion = src._httpVersion;
		_headers = src._headers;
		_rawBody = src._rawBody;
		_refinedBody = src._refinedBody;
	}
	return *this;
}

std::ostream	&operator<<(std::ostream &out, Request const &src)
{
	os << "#----------Request----------#" << std::endl;
	out << "Method: " << src._method << std::endl;
	out << "Uri: " << src._uri << std::endl;
	out << "HttpVersion: " << src._httpVersion << std::endl;
	out << "Headers: " << std::endl;
	for (auto it = src._headers.begin(); it != src._headers.end(); it++)
		out << it->first << ": " << it->second << std::endl;
	out << "RawBody: " << src._rawBody << std::endl;
	out << "RefinedBody: " << src._refinedBody << std::endl;
	out << "#---------------------------#" << std::endl;
	return out;
}

std::string	Request::operator[](std::string key)
{
	if (key == "method")
		return _method;
	else if (key == "uri")
		return _uri;
	else if (key == "httpVersion")
		return _httpVersion;
	else if (_headers.find(key) != _headers.end())
		return _headers[key];
	else
		return "";
}

////////////////////
//member functions//
////////////////////

//process the raw body into variables wich are then accessible with request["key"]
bool	Request::buildHeader(std::string rawHeader)
{
	std::vector<std::string>	lines;
	std::string					line;
	std::string					key;
	std::string					value;
	size_t						pos;

	//Split the rawHeader into lines
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = rawHeader.find("\r\n");
		line = rawHeader.substr(0, pos);
		if (line.size() > 0)
			lines.push_back(line);
		rawHeader = rawHeader.substr(pos + 2);
	}

	//Parse the first line
	if (lines.size() == 0)
		return true;
	line = lines[0];

	pos = line.find(" ");
	if (pos == std::string::npos)
		return true;
	_method = line.substr(0, pos);

	line = line.substr(pos + 1);
	pos = line.find(" ");
	if (pos == std::string::npos)
		return true;
	_uri = line.substr(0, pos);

	line = line.substr(pos + 1);
	_httpVersion = line; 

	//Parse the headers
	for (size_t i = 1; i < lines.size(); i++)
	{
		line = lines[i];
		pos = line.find(": ");
		if (pos == std::string::npos)
			return true;
		key = line.substr(0, pos);
		value = line.substr(pos + 2);
		_headers[key] = value;
	}
	return false;
}

//builds the body for the request either  by spliting and decoding cgi args or extracting data of files to download
bool	Request::buildBody(std::string rawBody)
{
	_rawBody = rawBody;
	if (_method == "POST")
	{
		if (_headers.find("Content-Type") != _headers.end())
		{
			std::string	contentType = _headers["Content-Type"];
			if (contentType.find("multipart/form-data") != std::string::npos && _headers["Content-Type"].find("boundary=") != std::string::npos)
				{if (_refinedBody.setFiles(_headers["Content-type"].substr(_headers["Content-type"].find("boundary=") + 9), _rawBody))
					{clear(); return true;}}
			else if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
				{if(_refinedBody.setCgiArgs(_rawBody))
					{clear(); return true;}}
			else
				{clear(); return true;}
}	}	}

//return true if the header contains the key
bool	Request::isHeader(std::string key)
{
	if (key == "method")
		return _method != "";
	else if (key == "uri")
		return _uri != "";
	else if (key == "httpVersion")
		return _httpVersion != "";
	else if (_headers.find(key) != _headers.end())
		return true;
	else
		return false;
}

//handle the request and build the response
bool	Request::handle(ServConfig &server, Response &response)
{
	if (_method == "GET")
		return RequestHandler::rGet(*this, response);
	else if (_method == "POST")
		return RequestHandler::rPost(*this, response);
	else if (_method == "DELETE")
		return RequestHandler::rDelete(*this, response);
	else
		{response.build(501, "", server); clear(); return true;}
}

//clear the request
void	Request::clear()
	{*this = Request();}
