#pragma once

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
		friend std::ostream			&operator<<(std::ostream &out, Error const &src);

		///////////
		//setters//
		///////////

		void						addErrPage(int code, std::string page);

		///////////
		//getters//
		///////////

		std::string					getErrPage(int code);

		////////////////////
		//member functions//
		////////////////////

		void						checkValidity();


	private:
		std::map<int, std::string>	_errPages;	
};
