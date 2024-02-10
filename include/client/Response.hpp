#pragma once

#include "libs.hpp"

//if we are having troubles with many connections, we can use connection: close every time

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

		Response			&operator=(Response const &src);
		friend std::ostream	&operator<<(std::ostream &out, Response const &src);

		///////////
		//setters//
		///////////

		void				setCode(int code);
		void				setKeepAlive(bool keepAlive);

		void				setCookie(std::string cookie);

		void				setContentDisposition(std::string contentDisposition);
		void				setContentType(std::string contentType);
		void				setContentLength(std::string contentLength);

		void				setContent(std::string content);
		

		///////////
		//getters//
		///////////

		int					getCode();
		bool				getKeepAlive();

		std::string			getCookie();

		std::string			getContentDisposition();
		std::string			getContentType();
		std::string			getContentLength();

		std::string			getContent();

		////////////////////
		//member functions//
		////////////////////

		void				send(int socket);
		void				clear();
		

	private:
		int					_code;
		bool				_keepAlive;

		std::string			_cookie;

		std::string			_contentDisposition;
		std::string			_contentType;
		std::string			_contentLength;

		std::string			_content;
		
}

//response examples:

// HTTP/1.1 200 OK
// Content-Type: text/html; charset=UTF-8
// Content-Length: 678
// Set-Cookie: sessionId=abc123; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Path=/; Secure; HttpOnly
// Connection: close
// 
// <!DOCTYPE html>
// <html>
// <head>
//     <title>Page avec Cookie</title>
// </head>
// <body>
//     <h1>Bienvenue sur notre site!</h1>
//     <p>Ce site utilise des cookies pour améliorer votre expérience.</p>
// </body>
// </html>


// HTTP/1.1 200 OK
// Content-Type: application/pdf
// Content-Disposition: attachment; filename="example.pdf"
// Content-Length: 34200
// Connection: close
// 
// %PDF-1.4
// %âãÏÓ
// ... contenu binaire du PDF ...


// HTTP/1.1 302 Found
// Location: http://www.exemple-site.com/nouvelle-page.html
// Content-Type: text/html; charset=UTF-8
// Content-Length: 123
// Connection: close
// 
// <!DOCTYPE html>
// <html>
// <head>
//     <title>Redirection</title>
// </head>
// <body>
//     <h1>Found</h1>
//     <p>The document has been found <a href="http://www.exemple-site.com/nouvelle-page.html">here</a>.</p>
// </body>
// </html>


// HTTP/1.1 404 Not Found
// Content-Type: text/html; charset=UTF-8
// Content-Length: 128
// Connection: close
// 
// <!DOCTYPE html>
// <html>
// <head>
//     <title>404 Not Found</title>
// </head>
// <body>
//     <h1>Not Found</h1>
//     <p>The requested URL was not found on this server.</p>
// </body>
// </html>


// HTTP/1.1 500 Internal Server Error
// Content-Type: text/html; charset=UTF-8
// Content-Length: 150
// Connection: close
// 
// <!DOCTYPE html>
// <html>
// <head>
//     <title>500 Internal Server Error</title>
// </head>
// <body>
//     <h1>Internal Server Error</h1>
//     <p>The server encountered an internal error and was unable to complete your request.</p>
// </body>
// </html>
