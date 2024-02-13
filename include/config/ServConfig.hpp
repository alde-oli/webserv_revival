#pragma once

#include "libs.hpp"

#include "SmartTypes.hpp"
#include "Codes.hpp"
#include "Route.hpp"

// class to store the subserver configuration
// also used to store the socket fd of the subserver
class ServConfig
{
	public:
		/////////////////////////////////
		// Constructors and destructor //
		/////////////////////////////////

		ServConfig() {};
		ServConfig(ServConfig const &src) = delete;
		~ServConfig() {};

		///////////////////////
		// operatos overload //
		///////////////////////

		friend std::ostream&			operator<<(std::ostream& os, const ServConfig& servConfig);
		ServConfig						&operator=(ServConfig const &src) = delete;

		/////////////
		// setters //
		/////////////

		void							setId(std::string id);
		void							setName(std::string name);
		void							setPort(std::string port);
		void							setHost(std::string host);
		void							setSocketFd(int socketFd);

		void							setIsDefault(std::string isDefault);
		void							setDefaultPage(std::string defaultPage);

		void							addRoute(Route &route);
		void							setErrors(Codes &errors);
		void							setDefaultErrorPage(std::string error);

		void							setMaxBodySize(int maxBodySize);
		void							setCookies(bool cookies);

		/////////////
		// getters //
		/////////////

		std::string						getId();
		std::string						getName();
		sockaddr_in						getAddr();
		int								getSocketFd();

		bool							getIsDefault();
		std::string						getDefaultPage();

		int								getMaxBodySize();
		bool							isCookies();

		bool							isRoute(std::string path);
		Route							&getRoute(std::string path);

		std::map<std::string, Route&>	getRoutes();
		std::string						getErrorPage(int code);

		/////////////
		// checker //
		/////////////

		bool							isValid();

		/////////////////////
		// member function //
		/////////////////////
		
		void 						CerrExit(const char* message, std::string precision);
		int 						CountOccurrences(const std::string& str, char charToFind);
		long long int			 	setSize(std::string size);
		int long long				BodySizeChecker(std::string size);
		bool 						setBool(std::string boolean);
		std::string 				ExtractServerName(const std::string& line);
		bool						ServerNameExists(std::vector<ServConfig> servers, std::string serverName);
		int 						IndexServer(std::vector<ServConfig> servers, std::string serverName);
		void						fillServer(ServConfig &server, std::string line, std::ifstream &file);
		void						createAndFillServer(std::vector<ServConfig> server, std::string line, std::ifstream &file);
		void						RegularServerTreatment(std::vector<ServConfig> servers, std::string line, std::ifstream &file);
		void						ServerOptionTreatment(std::vector<ServConfig> servers, std::string line, std::ifstream &file);
		void 						BeginBlockConfig(std::string line, std::ifstream &file, std::vector<ServConfig>	servers);
		std::vector<ServConfig>		ServerParsing(char *filename);
		void						setRoute(std::fstream &file, std::string &line);

	private:
		std::string						_id;
		std::string						_name;
		sockaddr_in						_addr;
		AutoFD							_socketFd;

		bool							_isDefault;
		std::string						_defaultPage;

		std::map<std::string, Route&>	_routes;
		Codes							_errors;

		int								_maxBodySize;
		bool							_cookies;

		////////////////////////
		// forgoten variables //
		////////////////////////
		std::string				  		_port;
		std::string						_host;
		std::string						_defaultErrorPage
};
