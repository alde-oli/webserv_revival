#include "../../include/config/CheckParsing.hpp"

//Check IP server

bool checkIPServer(ServConfig server, std::string ip)
{
	std::string ipAddress = inet_ntoa(server.getAddr().sin_addr);
	if (LOCALHOST == ipAddress || ipAddress == EXTERNAL_IP)
		return true;
	return false;
}