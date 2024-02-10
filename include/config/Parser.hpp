#pragma once

#include "libs.hpp"

#include "ServConfig.hpp"
#include "Codes.hpp"
#include "Route.hpp"


//regroups all the parsing functions
//and functions for the verifications of validity
class Parser
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		Parser() = delete;
		Parser(Parser const &src) = delete;
		~Parser() = delete;

		//////////////////////
		//operators overload//
		//////////////////////

		Parser							&operator=(Parser const &src) = delete;

		static std::vector<ServConfig>	parseConfig(std::string path);


	private:
		static void						parseServer(fstream &file, std::string &line, ServConfig &server);
		static Codes					parseError(fstream &file, std::string &line);
		static Route					parseRoute(fstream &file, std::string &line);
};
