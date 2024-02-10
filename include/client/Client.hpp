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
		Client() = delete;
		Client(int clientFd, int serverFd);
		Client(const Client& c) = delete;
		~Client();

		//////////////////////
		//operators overload//
		//////////////////////

		Client&					operator=(const Client& c);
		friend std::ostream&	operator<<(std::ostream& os, const Client& c);

		///////////
		//setters//
		///////////

		void					setRequest(Request &request);
		void					setResponse(Response &response);

		///////////
		//getters//
		///////////

		Request					&getRequest();
		Response				&getResponse();


	private:
		AutoFD					_flientFd;
		int						_ServerFd;

		Request					_request;

		Response				_response;
}