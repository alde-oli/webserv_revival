#include "../../include/config/Codes.hpp"
#include "../../include/config/ServConfig.hpp"

Codes						&Codes::operator=(Codes const &src)
{
	this->_errPages = src._errPages;
}

std::ostream				&operator<<(std::ostream &out, Codes const &src)
{
	out << "Error pages: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = src._errPages.begin(); it != src._errPages.end(); it++)
	{
		out << "Code: " << it->first << " Page: " << it->second << std::endl;
	}
	return out;
}

void						Codes::addErrPage(int code, std::string page)
{
	this->_errPages[code] = page;
}

std::string					Codes::getErrPage(int code)
{
	std::map<int, std::string>::iterator it = this->_errPages.find(code);
	if (it != this->_errPages.end())
		return it->second;
	return "";
}

bool 	Codes::checkValidity(std::string dir)
{
	DIR	*dirp;

	dirp = opendir(dir.c_str());
	if (!dirp)
		CerrExit("Error: Failed to open directory: ", dir);
	closedir(dirp);

	return false; // zero si le dossier est valide
}
