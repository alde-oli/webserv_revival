#pragma once

#include "libs.hpp"

#include "SmartTypes.hpp"
#include "Request.hpp"
#include "Response.hpp"

//Client class to store client information and handle client events
class Client
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////
		Client() = delete;
		Client(int clientFd, sockaddr_in ClientAddr, int serverFd);
		Client(const Client& c) = delete;
		~Client();

		//////////////////////
		//operators overload//
		//////////////////////

		Client&					operator=(const Client& c) = delete;
		friend std::ostream&	operator<<(std::ostream& os, const Client& c);

		///////////
		//setters//
		///////////

		void					updateActivity();

		///////////
		//getters//
		///////////

		int						getServFd();
		sockaddr_in				getAddr();
		std::time_t				getLastActivity();

		////////////////////
		//member functions//
		////////////////////

		bool					read(ServConfig &server, int kq);
		bool					write(int kq);

		bool					setWriteEvent(int kq);
		bool					unsetWriteEvent(int kq);


	private:
		AutoFD					_clientFd;
		int						_ServerFd;
		sockaddr_in				_clientAddr;
		std::time_t				_lastActivity;

		std::string				_rawRequest;
		static bool				_EOHFound;
		static size_t			_bodyToRead;
		

		Request					_request;

		Response				_response;
}