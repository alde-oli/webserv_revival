#include "Request.hpp"


//////////////////////////////
//Constructor and destructor//
//////////////////////////////

Request::Request()
	: _method(""), _uri(""), _httpVersion(""), _rawBody(""), _refinedBody()
{}

Request::b