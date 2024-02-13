#pragma once

#include "libs.hpp"

#define DEFAULT_ERRORS "error_pages/"

// class to store custom error pages
// is used to get custom and default error pages from a given code
// is also used to get the right message for codes from 1XX to 5XX
class Codes
{
	public:
		/////////////////////////////////
		// Constructors and destructor //
		/////////////////////////////////

		Codes() {};
		Codes(Codes const &src) = delete;
		~Codes() {};

		////////////////////////
		// operators overload //
		////////////////////////

		Codes						&operator=(Codes const &src) = delete;
		friend std::ostream			&operator<<(std::ostream &out, Codes const &src);

		/////////////
		// setters //
		/////////////

		void						addErrPage(int code, std::string page);

		/////////////
		// getters //
		/////////////

		std::string					getErrPage(int code);

		//////////////////////
		// member functions //
		//////////////////////

		bool						checkValidity(std::string dir);


	private:
		std::map<int, std::string>	_errPages;	
};
