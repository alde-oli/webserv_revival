#pragma once

#include "libs.hpp"

#include "SmartTypes.hpp"
#include "Codes.hpp"
#include "Route.hpp"

//class to store the subserver configuration
//also used to store the socket fd of the subserver
class ServConfig
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		ServConfig();
		ServConfig(ServConfig const &src) = delete;
		~ServConfig();

		/////////////////////
		//operatos overload//
		/////////////////////

		friend std::ostream&			operator<<(std::ostream& os, const ServConfig& servConfig);
		ServConfig						&operator=(ServConfig const &src) = delete;

		///////////
		//setters//
		///////////

		void							setId(std::string id);
		void							setName(std::string name);
		void							setPort(std::string port);
		void							setHost(std::string host);
		void							setSocketFd(int socketFd);

		void							setIsDefault(std::string isDefault);
		void							setDefaultPage(std::string defaultPage);

		void							addRoute(Route &route);
		void							setErrors(Codes &errors);

		void							setMaxBodySize(int maxBodySize);
		void							setCookies(bool cookies);

		///////////
		//getters//
		///////////

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

		///////////
		//checker//
		///////////

		bool							isValid();

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
};
