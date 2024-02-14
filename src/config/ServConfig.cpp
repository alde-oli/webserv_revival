#include "../../include/config/ServConfig.hpp"

/////////////////
// [ GETTERS ] //
/////////////////

std::string ServConfig::getName() const
{
	return (this->_name);
}

std::string	ServConfig::getId() const
{
	return (this->_id);
}

sockaddr_in	ServConfig::getAddr() const
{
	return (this->_addr);
}

int			ServConfig::getSocketFd() const
{
	return (this->_socketFd.get());
}

bool		ServConfig::getIsDefault() const
{
	return (this->_isDefault);
}

std::string	ServConfig::getDefaultPage() const
{
	return (this->_defaultPage);
}

int			ServConfig::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

bool		ServConfig::isCookies() const
{
	return (this->_cookies);
}

// Verifier le chemin 
bool		ServConfig::isRoute(std::string path) const
{
	DIR	*dirp;

	dirp = opendir(path.c_str());
	if (!dirp)
		return false;
	closedir(dirp);
	return true;
}

Route		&ServConfig::getRoute(std::string path) const
{
	std::map<std::string, Route&>::const_iterator it = this->_routes.find(path);
	if (it == this->_routes.end())
		CerrExit("[ ERROR ] Route not found: ", path);
	return it->second;
}

std::map<std::string, Route&>	ServConfig::getRoutes() const
{
	return (this->_routes);
}

// Return the good path of the error page
std::string		ServConfig::getErrorPage(int code) const
{
	return (this->_errors.getErrPage(code));
}

/////////////////
// [ SETTERS ] //
/////////////////

void	ServConfig::setId(std::string id)
{
	this->_id = id;
}

void	ServConfig::setName(std::string name)
{
	this->_name = name;
}

void	ServConfig::setPort(std::string port)
{
	this->_port = port;
}

void	ServConfig::setHost(std::string host)
{
	this->_host = host;
}

void	ServConfig::setSocketFd(int socketFd)
{
	this->_socketFd.set(socketFd);
}

void	ServConfig::setIsDefault(std::string isDefault)
{
	if (isDefault == "true" || isDefault == "TRUE")
		this->_isDefault = true;
	else if (isDefault == "false" || isDefault == "FALSE")
		this->_isDefault = false;
	else
		CerrExit("[ ERROR ] Invalid value for default: ", isDefault);
}

void	ServConfig::setDefaultPage(std::string defaultPage)
{
	this->_defaultPage = defaultPage;
}

void	ServConfig::addRoute(Route &route)
{
	this->_routes[route.getId()] = route;
}

void	ServConfig::setDefaultErrorPage(std::string error)
{
	this->_defaultErrorPage = error;
}

void	ServConfig::setErrors(Codes &errors)
{
	this->_errors = errors;
}

void	ServConfig::setMaxBodySize(int maxBodySize)
{
	this->_maxBodySize = maxBodySize;
}

void	ServConfig::setCookies(bool cookies)
{
	this->_cookies = cookies;
}

////////////////////////
// operators overload //
////////////////////////

ServConfig	&ServConfig::operator=(const ServConfig &other)
{
	if (this != &other)
	{
		this->_id = other._id;
		this->_name = other._name;
		this->_addr = other._addr;
		this->_socketFd.set(other._socketFd.get());
		this->_isDefault = other._isDefault;
		this->_defaultPage = other._defaultPage;
		this->_routes = other._routes;
		this->_errors = other._errors;
		this->_maxBodySize = other._maxBodySize;
		this->_cookies = other._cookies;
		this->_port = other._port;
		this->_host = other._host;
		this->_defaultErrorPage = other._defaultErrorPage;
	}
	return *this;
}

// ------------------ [ Main functions of the class ] ------------------ //

void	ServerConfig::checkValidity()
{
	if (_id.empty())
		CerrExit("Error : ID is empty please insert a valid ID", "");
	else if (_serverName.empty())
		CerrExit("Error : ServerName is empty please insert a valid ServerName", "");
	else if (_defaultPage.empty())
		CerrExit("Error : DefaultPage is empty please insert a valid DefaultPage", "");
	else if (_maxBodySize <= 0)
		CerrExit("Error : MaxBodySize is empty please insert a valid MaxBodySize", "");
	if (isLocalOrExternal(_serverAddr) == false)
		CerrExit("Error : ServerAddr is not valid please insert a valid ServerAddr", "");
	
	std::map<std::string, Route>::iterator it;

    for (it = _routes.begin(); it != _routes.end(); ++it)
	{
		it->second.verif();
    }
}