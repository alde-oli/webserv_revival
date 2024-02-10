#pragma once

#include "libs.hpp"

#include "SmartTypes.hpp"
#include "Request.hpp"
#include "Response.hpp"

//Client class to store the client fd and the server fd it is connected to
//Also stores the request and response objects
class Client
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		//////////////////////
		//operators overload//
		//////////////////////


	private:
		AutoFD		_flientFd;
		int			_ServerFd;

		Request		_request;

		Response	_response;
}