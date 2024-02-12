#pragma once

#include "libs.hpp"
#include "ServConfig.hpp"
#include "Client.hpp"

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

		void	run(std::vector<ServConfig> &servers);
		void	acceptNew(int kq, int serverFd, std::map<int, Client&> &clients);
		void	setSockets(std::vector<ServConfig> &servers);
		void	setKqueue(AutoFD &kq, std::vector<ServConfig> &servers);
}