#include "../../include/config/ServConfig.hpp"
#include "../../include/config/ParsingUtils.hpp"

// Cerror message and exit
void CerrExit(const char* message, std::string precision)
{
	std::cerr << message << precision << std::endl;
	exit(1);
}