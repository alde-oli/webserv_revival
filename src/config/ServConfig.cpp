#include "ServConfig.hpp"

/////////////////
// [ GETTERS ] //
/////////////////

std::string ServConfig::getName()
{
	return (this->_name);
}

std::string	ServConfig::getId()
{
	return (this->_id);
}

sockaddr_in	ServConfig::getAddr()
{
	return (this->_addr);
}

int			ServConfig::getSocketFd()
{
	return (this->_socketFd);
}

bool		ServConfig::getIsDefault()
{
	return (this->_isDefault);
}

std::string	ServConfig::getDefaultPage()
{
	return (this->_defaultPage);
}

int			ServConfig::getMaxBodySize()
{
	return (this->_maxBodySize);
}

bool		ServConfig::isCookies()
{
	return (this->_cookies);
}

// [ A FAIRE ] Verifier le chemin 
bool		ServConfig::isRoute(std::string path)
{

}

// [ A FAIRE ]
Route		&ServConfig::	getRoute(std::string path)
{

}

std::map<std::string, Route&>	ServConfig::getRoutes()
{
	return (this->_routes);
}

// [ A FAIRE ] Return the good path of the error page
std::string		ServConfig::getErrorPage(int code)
{

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

// map : pas de push back ?
void	ServConfig::addRoute(Route &route)
{
	this->_routes.push_back(route);
}

void	ServConfig::setDefaultErrorPage(std::string error)
{
	 this->_defaultErrorPage = error;
}

// [ A MODIF ] c'est une map 
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
	}
	return *this;
}

// ---------------------------- [ ADDITIONAL FUNCTIONS FOR PARSING ] -------------------------------- //

// Cerror message and exit
static void CerrExit(const char* message, std::string precision)
{
	std::cerr << message << precision << std::endl;
	exit(1);
}

// Return the number of times occurence has been found
static int CountOccurrences(const std::string& str, char charToFind)
{
    int count = 0;
    for (std::size_t i = 0; i < str.size(); ++i)
        if (str[i] == charToFind)
            ++count;
    return count;
}

// Second part of the body size checker
static long long int setSize(std::string size)
{
	long long nbr;

	if (size.find('M') != std::string::npos)
	{
		size = size.substr(0, size.find('M'));
		nbr = atol(size.c_str());
		nbr *= 1000;
		return (nbr);
	}
	else if (size.find('G') != std::string::npos)
	{
		size = size.substr(0, size.find('G'));
		nbr = atol(size.c_str());
		nbr *= 1000000;
		return (nbr);
	}
	else
	{
		return (nbr = atol(size.c_str()));
	}
}

// Chek if the input in body size is valid
static int long long BodySizeChecker(std::string size)
{
	if (!size.empty()) 
	{
		unsigned int i = 0;
		while (i < size.length() && isdigit(size[i]))
			i++;
		if (i < size.length() && !isdigit(size[i]) && (size[i] == 'M' || size[i] == 'G'))
		{
			if (i + 1 == size.length())
				return (setSize(size));
			else
				CerrExit("Error: bad body size input: ", size);
		}
		else if (i == size.length())
			return (setSize(size));
		else
			CerrExit("Error: bad body size input: ", size);
	}
	CerrExit("[ ERROR ] Invalid value for max_body_size: ", size);
	return (0);
}

static bool setBool(std::string boolean)
{
	bool value;

	if (!strcmp(boolean.c_str(), "FALSE") || !strcmp(boolean.c_str(), "false"))
	{
		return (value = false);
	}
	else if (!strcmp(boolean.c_str(), "TRUE") || !strcmp(boolean.c_str(), "true"))
	{
		return (value = true);
	}
	else
		CerrExit("Error: bad boolean type: ", boolean);
	return (0);
}

// ---------------------------- [ MAIN FUNCTIONS FOR PARSING ] -------------------------------- //

std::string ExtractServerName(const std::string& line) {
    std::size_t start = line.find("[");
    std::size_t end = line.find("]", start);

    if (start != std::string::npos && end != std::string::npos) {
        // Extrait le nom du serveur, en omettant les crochets eux-mêmes.
        // +1 pour commencer après le crochet ouvrant, et end-start-1 pour exclure les deux crochets.
        return line.substr(start + 1, end - start - 1);
    }
    return ""; // Renvoie une chaîne vide si les crochets ne sont pas trouvés.
}

// Check if the sever already exists
static bool ServerNameExists(std::vector<ServConfig> servers, std::string serverName)
{
    for (std::size_t i = 0; i < servers.size(); ++i)
        if (servers[i].getName() == serverName)
            return true;
    return false;
}

// Return the index of the existing server
static int IndexServer(std::vector<ServConfig> servers, std::string serverName)
{
    for (std::size_t i = 0; i < servers.size(); ++i)
        if (servers[i].getName() == serverName)
            return i;
    return 0;
}

// Remplis le serveur mis en arguments avec le reste du file
static void fillServer(ServConfig &server, std::string line, std::ifstream &file)
{
	// Continue de lire le fichier jusqu'au prochain []
    while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		// faire la liste de toutes les variables du serveur
		// Si une existe deja : ERROR
		if (!line.find("port = "))
			server.setPort(line.substr(7));
		else if (!line.find("host = "))
			server.setHost(line.substr(7));
		else if (!line.find("name = "))
			server.setName(line.substr(7));
		else if (!line.find("default_page = "))
			server.setDefaultPage(line.substr(15));
		else if (!line.find("max_body_size = "))
		{
			int ibodySize = BodySizeChecker(line.substr(16));
			server.setMaxBodySize(ibodySize);
		}
		else if (!line.find("default = "))
			server.setIsDefault(line.substr(10));
		else if (!line.find("cookies = "))
		{
			int icookies = setBool(line.substr(10));
			server.setCookies(icookies);
		}
		else if (!line.find("default_error_page = "))
		{
			server.setDefaultErrorPage(line.substr(21));
		}
		else
			CerrExit("[ ERROR ] Invalid server line: ", line);
    }
    file.close();
}

// Gere les cas [NOM_SERVER]
static void RegularServerTreatment(std::vector<ServConfig> servers, std::string line, std::ifstream &file)
{
    std::string serverName = ExtractServerName(line);

	if (serverName.empty())
		CerrExit("[ ERROR ] Le nom du serveur est vide... :", line);
	// Si le serveur est deja declare et que l'on est dans la main config
    if (ServerNameExists(servers, serverName))
		fillServer(servers[IndexServer(servers, serverName)], line, file); // [ A CREER ] remplir les infos dans le serveur deja existant
	else
		createAndFillServer(); // [ A CREER ] remplir les infos dans le serveur deja existant
}

// Gere les cas [NOM_SERVER:OPTION] / [NOM_SERVER:OPTION:OPTION]
static void	ServerOptionTreatment(std::vector<ServConfig> servers, std::string line, std::ifstream &file)
{

}

// Tu recois le debut d'un nouveau block de config
static void BeginBlockConfig(std::string line, std::ifstream &file, std::vector<ServConfig>	servers)
{
	std::size_t bracketPos = line.find(']');
	std::size_t doubleDotsPos = line.find(':');

    if (bracketPos == std::string::npos) // Check if there is the brackets []
		CerrExit("[ ERROR ] Brackets are not closed :", line);

	if (CountOccurrences(line, ':') > 0 && CountOccurrences(line, ':') < 3) // Check if there is not too many dots ':'
		ServerOptionTreatment(servers, line, file); // Traiter les deux points	
	else if (CountOccurrences(line, ':') > 2)
		CerrExit("[ ERROR ] Too many double dots :", line);
	else
		RegularServerTreatment(servers, line, file); // Traite le cas ou cest juste [SERVER]
}

// Args: 	FileName
//			Map Sever Config
std::vector<ServConfig>	ServerParsing(char *filename)
{
	std::vector<ServConfig>	servers;
    std::ifstream file(filename);

    if (!file) 
		CerrExit("Cannot open file: ", filename);
    std::string line;
    while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		else if(line[0] == '[')
			BeginBlockConfig(line, file, servers);
    }
    file.close();
	return (servers);
}