#include "../../include/config/ServConfig.hpp"
#include "../../include/config/ParsingExec.hpp"

// ---------------------------- [ ADDITIONAL FUNCTIONS FOR PARSING ] -------------------------------- //

static int	countDeuxPoints(std::string &line)
{
	int count = 0;
	for (unsigned int i = 0; i < line.size(); i++)
		if (line[i] == ':')
			count++;
	return (count);
}

static bool hasDuplicateAddress(const std::vector<ServConfig>& servers)
{
	std::vector<sockaddr_in> addresses;
	std::vector<ServConfig>::const_iterator it;
	for (it = servers.begin(); it != servers.end(); ++it)
	{
		sockaddr_in serverAddr = it->getAddr();
		bool duplicateAddress = false;
		std::vector<sockaddr_in>::const_iterator addressIt;
		for (addressIt = addresses.begin(); addressIt != addresses.end(); ++addressIt)
		{
			if (memcmp(&(*addressIt), &serverAddr, sizeof(sockaddr_in)) == 0)
			{
				duplicateAddress = true;
				break;
			}
		}
		if (duplicateAddress)
			return true;
		addresses.push_back(serverAddr);
	}
	return false;
}

// ---------------------------- [ MAIN FUNCTIONS FOR PARSING ] -------------------------------- //

void setServer(std::fstream &file, std::string &line, ServConfig &server)
{
	server.setMain(file, line);
	if (line[0] == '[' && countDeuxPoints(line) > 0 && line.find_first_of(server.getId()) != line.npos)
	{
		if (countDeuxPoints(line) == 1 && line.find("ERROR") == line.find_first_of(":") + 1 && line.find("ERROR") != line.npos)
			server.setError(file, line);
		else if (countDeuxPoints(line) == 2 && line.find("ROUTE") == line.find_first_of(":") + 1 && line.find("ROUTE") != line.npos)
			server.setRoute(file, line);
		else
			CerrExit("Error: bad line: ", line);
	}
	while (line[0] == '[' || (getline(file, line) && line[0] != '['))
	{
		if (line[0] == '[' && countDeuxPoints(line) == 0)
			break ;
		if (line.empty())
			continue;
		if (countDeuxPoints(line) == 1 && line.find("ERROR") == line.find_first_of(":") + 1 && line.find("ERROR") != line.npos)
			server.setError(file, line);
		else if (countDeuxPoints(line) == 2 && line.find("ROUTE") == line.find_first_of(":") + 1 && line.find("ROUTE") != line.npos)
			server.setRoute(file, line);
		else
			CerrExit("Error: bad line: ", line);
	}
	server.checkValidity();
}

// Main function to parse the server configuration
std::vector<ServConfig>	ServerParsing(char *filename)
{
	std::fstream file(filename);
	
	if (!file.is_open())
		CerrExit("Error: Failed to open file: ", filename);
	
	std::vector<ServConfig> servers;
	std::string line;
	while (getline(file, line))
	{
		ServConfig server = ServConfig();
		if (line.empty())
			continue;
		else if (line[0] == '[' && line[line.size() - 1] == ']')
		{
			if (line.find(":") != std::string::npos)
				CerrExit("Error: bad server name: ", line);
			while (line[0] == '[' && countDeuxPoints(line) == 0)
			{
				server = ServConfig();
				setServer(file, line, server);
				bool duplicateId = false;
				for (std::vector<ServConfig>::iterator it = servers.begin(); it != servers.end(); ++it)
				{
					if (it->getId() == server.getId())
					{
						duplicateId = true;
						break;
					}
				}
				if (duplicateId)
					CerrExit("Error: duplicate server ID: ", server.getId());
				servers.push_back(server);
			}
		}
		else
			CerrExit("Error: bad line: ", line);
	}
	file.close();
	if (hasDuplicateAddress(servers))
		CerrExit("Error: duplicate server address", "");
	return (servers);
}
