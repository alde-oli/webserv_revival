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

		Data();
		Data(std::string contentType, std::string rawContent);
		Data(Data const &src) = delete;
		~Data();

		//////////////////////
		//operators overload//
		//////////////////////

		Data						&operator=(Data const &src) = delete;
		friend std::ostream			&operator<<(std::ostream &out, Data const &src);

		///////////
		//setters//
		///////////

		bool						setFiles(std::string boundary, std::string rawContent);
		bool						setCgiArgs(std::string rawContent);

		///////////
		//getters//
		///////////

		std::vector<contentData>	getFiles();
		std::vector<std::string>	getCgiArgs();

	private:
		std::vector<contentData>	_files;

		std::vector<std::string>	_cgiArgs;
};