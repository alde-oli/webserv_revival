#include "../include/webserv.hpp"
#include "../include/config/ServConfig.hpp"
#include "../include/ServRunner.hpp"

int main(int argc, char **argv)
{
	std::vector<ServConfig>	servers;
	if (argc > 2)
		{std::cerr << "Usage: ./webserv [config_file.ini]" << std::endl; return 1;}
	else if (argc == 1)
		servers = ServerParsing("defaultConfig.ini");
	else
		servers = ServerParsing(argv[1]);

	if (checkValidity(servers) == false)
		{std::cerr << "Critical configuration error: Exiting program" << std::endl; return 1;}

	ServRunner::run(servers);
}
