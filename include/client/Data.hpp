#pragma once

#include "libs.hpp"

//struct to store a single file and it's name + extension
struct contentData
{
	std::string	_fileName;
	std::string _contentType;
	std::string	_data;
};

//class to proccess the raw body for upload post requests
//it stores the boundary and every files contained in the body
class Data
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////

		MultipartFormData();
		MultipartFormData(std::string contentType, std::string rawContent);
		MultipartFormData(MultipartFormData const &src) = delete;
		~MultipartFormData();

		//////////////////////
		//operators overload//
		//////////////////////

		MultipartFormData			&operator=(MultipartFormData const &src) = delete;
		friend std::ostream			&operator<<(std::ostream &out, MultipartFormData const &src);

		///////////
		//setters//
		///////////

		void 						setBoundary(std::string boundary);
		void						setContent(std::vector<contentData> content);

		///////////
		//getters//
		///////////

		std::string 				getBoundary();
		std::vector<contentData>	getContent();


	private:
		std::string					_boundary;
		std::vector<contentData>	_content;
};