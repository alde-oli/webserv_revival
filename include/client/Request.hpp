#pragma once

#include "libs.hpp"

//Class to store the parsed request from the client
class	Request
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		Request();
		Request(Request const &src) = delete;
		~Request();

		//////////////////////
		//operators overload//
		//////////////////////

		Request								&operator=(Request const &src);
		friend std::ostream					&operator<<(std::ostream &out, Request const &src);

		///////////
		//setters//
		///////////

		void								setMethod(std::string method);
		void								setUri(std::string uri);
		void								setHttpVersion(std::string httpVersion);

		void								addHeader(std::string key, std::string value);

		void								addRawBody(std::string rawBody);
		void								RefinedBody(Data &refinedBody);

		///////////
		//getters//
		///////////

		std::string							getMethod();
		std::string							getUri();
		std::string							getHttpVersion();

		std::map<std::string, std::string>	getHeaders();

		std::string							getRawBody();
		Data								getRefinedBody();

		/////////////////////
		//member functions//
		/////////////////////

		void								build(std::string rawRequest);
		void								clear();


	private:
		std::string							_method;
		std::string							_uri;
		std::string							_httpVersion;

		std::map<std::string, std::string>	_headers;

		std::string							_rawBody;
		Data								_refinedBody;
}