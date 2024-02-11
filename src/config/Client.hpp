#include "Client.hpp"

///////////////////////////////
//Constructors and destructor//
///////////////////////////////

Client::Client(int clientFd, int serverFd)
	: _clientFd(clientFd), _serverFd(serverFd), _lastActivity(std::time(0))
{}

Client::~Client()
{
	this->close();
}