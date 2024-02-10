#pragma once

#include "libs.hpp"

class ServRunner
{
	public:
		///////////////////////////////
		//Constructors and destructor//
		///////////////////////////////
		ServRunner() = delete;
		ServRunner(ServRunner const &src) = delete;
		ServRunner(ServRunner&&) = delete;

		//////////////////////
		//operators overload//
		//////////////////////

		Parser	&operator=(ServRunner const &src) = delete;

		////////////////////
		//member functions//
		////////////////////

		void	setSockets(std::vector<ServConfig> &servers);
		void	setKqueue(std::vector<ServConfig> &servers);
		void	run(std::vector<ServConfig> &servers);
}