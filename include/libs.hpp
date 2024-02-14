#pragma once
#include 	<algorithm>
#include	<string>
#include	<vector>
#include	<map>
#include	<set>
#include	<fstream>
#include	<sstream>
#include	<ostream>
#include	<sys/socket.h>
#include	<sys/event.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<arpa/inet.h>
#include	<netinet/in.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<cstdlib>
#include	<iomanip>
#include	<signal.h>
#include	<exception>
#include	<dirent.h>

#include 	"SmartTypes.hpp"
#include	"config/ServConfig.hpp"
#include	"config/Codes.hpp"
#include	"config/ParsingExec.hpp"
#include	"config/ParsingUtils.hpp"
#include	"config/Route.hpp"


#define LOCALHOST "127.0.0.1"
#define EXTERNAL_IP "11.10.14.5"