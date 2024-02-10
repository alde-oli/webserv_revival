#pragma once

#include "libs.hpp"

//responsible for fully building the response and sending it to the client
class Response
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		Response();
		Response(Response const &src) = delete;
		~Response();

		//////////////////////
		//operators overload//
		//////////////////////

		Response	&operator=(Response const &src) = delete;

		///////////
		//setters//
		///////////
		

		///////////
		//getters//
		///////////
		

	private:
		int			_code;
		
}