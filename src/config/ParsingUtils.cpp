#include "../../include/config/ParsingUtils.hpp"

// Cerror message and exit
void CerrExit(const char* message, std::string precision)
{
	std::cerr << message << precision << std::endl;
	exit(1);
}

bool isLocalOrExternal(struct sockaddr_in addr)
{
    // Convertit l'adresse IP de network byte order à dotted-decimal string
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ipstr, INET_ADDRSTRLEN);

    // Vérifie si l'adresse IP est localhost ou l'adresse IP externe connue
    if (strcmp(ipstr, LOCALHOST) == 0 || strcmp(ipstr, EXTERNAL_IP) == 0)
        return true;
    return false;
}
