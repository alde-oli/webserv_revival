#pragma once

#define DEFAULT_DIR "error_pages"

#include "libs.hpp"

#define DEFAULT_ERRORS "error_pages/"

//class to store custom error pages
//is used to get custom and default error pages from a given code
//is also used to get the right message for codes from 1XX to 5XX
class Codes
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		Error();
		Error(Error const &src) = delete;
		~Error();

		//////////////////////
		//operators overload//
		//////////////////////

		Error						&operator=(Error const &src) = delete;

		///////////
		//setters//
		///////////

		void						addErrPage(int code, std::string page);

		///////////
		//getters//
		///////////

		std::string					getErrPage(int code);


	private:
		std::map<int, std::string>	_errPages;	
};
