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

// ---------------------------- [ ADDITIONAL FUNCTIONS FOR PARSING ] -------------------------------- //

// Cerror message and exit
void ServConfig::CerrExit(const char* message, std::string precision)
{
	std::cerr << message << precision << std::endl;
	exit(1);
}

// Return the number of times occurence has been found
int ServConfig::CountOccurrences(const std::string& str, char charToFind)
{
    int count = 0;
    for (std::size_t i = 0; i < str.size(); ++i)
        if (str[i] == charToFind)
            ++count;
    return count;
}

// Second part of the body size checker
long long int ServConfig::setSize(std::string size)
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
int long long ServConfig::BodySizeChecker(std::string size)
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

bool ServConfig::setBool(std::string boolean)
{
	bool value;

	if (!strcmp(boolean.c_str(), "FALSE") || !strcmp(boolean.c_str(), "false"))
		return (value = false);
	else if (!strcmp(boolean.c_str(), "TRUE") || !strcmp(boolean.c_str(), "true"))
		return (value = true);
	else
		CerrExit("Error: bad boolean type: ", boolean);
	return (0);
}

// ---------------------------- [ MAIN FUNCTIONS FOR PARSING ] -------------------------------- //

// Renvoie le nom du serveur entre crochets
// NULL si les crochets ne sont pas complets
std::string ServConfig::ExtractServerName(const std::string& line)
{
    std::size_t start = line.find("[");
    std::size_t end = line.find("]", start);

    if (start != std::string::npos && end != std::string::npos)
	{
        // Extrait le nom du serveur, en omettant les crochets eux-mêmes.
        // +1 pour commencer après le crochet ouvrant, et end-start-1 pour exclure les deux crochets.
        return line.substr(start + 1, end - start - 1);
    }
    return ""; // Renvoie une chaîne vide si les crochets ne sont pas trouvés.
}

// Check if the sever already exists
bool ServConfig::ServerNameExists(std::vector<ServConfig> servers, std::string serverName)
{
    for (std::size_t i = 0; i < servers.size(); ++i)
        if (servers[i].getName() == serverName)
            return true;
    return false;
}

// Return the index of the existing server
int ServConfig::IndexServer(std::vector<ServConfig> servers, std::string serverName)
{
    for (std::size_t i = 0; i < servers.size(); ++i)
        if (servers[i].getName() == serverName)
            return i;
    return 0;
}

// Remplis le serveur mis en arguments avec le reste du file
void ServConfig::fillServer(ServConfig &server, std::string line, std::ifstream &file)
{
	// Continue de lire le fichier jusqu'au prochain []
    while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		// Faire la liste de toutes les variables du serveur
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
			server.setDefaultErrorPage(line.substr(21));
		else
			CerrExit("[ ERROR ] Invalid server line: ", line);
    }
    file.close();
}

// Le serveur n'existe pas, on le cree
void ServConfig::createAndFillServer(std::vector<ServConfig> server, std::string line, std::ifstream &file)
{
	ServConfig newServer;
	newServer.setName(ExtractServerName(line));
	fillServer(newServer, line, file);
	server.push_back(newServer);
}

// Gere les cas [NOM_SERVER]
void ServConfig::RegularServerTreatment(std::vector<ServConfig> servers, std::string line, std::ifstream &file)
{
    std::string serverName = ExtractServerName(line);

	if (serverName.empty())
		CerrExit("[ ERROR ] Le nom du serveur est vide... :", line);
	// Si le serveur est deja declare et que l'on est dans la main config
    if (ServerNameExists(servers, serverName))
		fillServer(servers[IndexServer(servers, serverName)], line, file); // Remplir les infos dans le serveur deja existant
	else
		createAndFillServer(servers, line, file); // Créer puis remplir les infos dans un new serveur
}

// Gere les cas [NOM_SERVER:OPTION] / [NOM_SERVER:OPTION:OPTION]
void	ServConfig::ServerOptionTreatment(std::vector<ServConfig> servers, std::string line, std::ifstream &file)
{
	// Si un seul double point
	if (CountOccurrences(line, ':') == 1)
	{
        std::string option =  line.substr(line.find(':'), line.length());
		if (option == "ERROR" || option == "ROUTES")
			setRoute(file, line);
		else
			CerrExit("[ ERROR ] Invalid server option: ", option);
	}
	else // Si deux doubles points
	{
		std::string option =  line.substr(line.find(':'), line.length());
		if (option.substr(0, 6) == "ROUTES" || option.substr(0, 5) == "ERROR")
			setRoute(file, line);
		else
			CerrExit("[ ERROR ] Invalid server option: ", option);
	}
}

// Traitement route
void	ServConfig::setRoute(std::fstream &file, std::string &line)
{
	std::string		key;
	std::string		value;

	if (line[0] != '[')
		CerrExit("Error: unexpected line", line);
	Route route = Route();
	route.setId(line.substr(line.find_last_of(':') + 1, line.length() - line.find_last_of(':') - 2));

	while (std::getline(file, line) && line[0] != '[')
	{
		if (line.empty())
			continue ;
		else if (!line.find("route = "))
			route.setPath(line.substr(8));
		else if (!line.find("root = "))
			route.setRoot(line.substr(7));
		else if (!line.find("default_page = "))
			route.setDefaultPage(line.substr(15));
		else if (!line.find("methods = "))
			route.setMethods(line.substr(10));
		else if (!line.find("listing = "))
			route.setListing(line.substr(10));
		else if (!line.find("upload = "))
			route.setIsUpload(line.substr(9));
		else if (!line.find("force_upload = "))
			route.setForceUpload(line.substr(15));
		else if (!line.find("download = "))
			route.setIsDownload(line.substr(11));
		else if (!line.find("download_dir = "))
			route.setDownloadDir(line.substr(15));
		else if (!line.find("redir = "))
			route.setIsRedir(line.substr(8));
		else if (!line.find("redir_route = "))
			route.setRedirDir(line.substr(14));
		else if (!line.find("cgi = "))
			route.setCgiExt(line.substr(6));
		else
			CerrExit("Error: invalid route line: ", line);
	}
	this->_routes[route.getId()] = route;

	std::string newRouteFormat = "[" + this->_id + ":ROUTES:";
	if (line.find(newRouteFormat) == 0 && line[line.length() - 1] == ']')
	{
		int colonCount = 0;
		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == ':')
				colonCount++;
		}
		if (colonCount == 2 && line.find_last_of(':') < line.length() - 2)
			setRoute(file, line);
		else
			CerrExit("Error: invalid route format", line);
	}
} 

// Tu recois le debut d'un nouveau block de config
void ServConfig::BeginBlockConfig(std::string line, std::ifstream &file, std::vector<ServConfig>	servers)
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
std::vector<ServConfig>	ServConfig::ServerParsing(char *filename)
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