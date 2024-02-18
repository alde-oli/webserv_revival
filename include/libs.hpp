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
#include	<sys/stat.h>
#include	<sys/wait.h>
#include	<iostream>

# define ERROR(arg) std::cout << "\n\033[1;31m˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜\n🤖 ERROR\033[0;37m [File] " << __FILE__ << " [line] " << __LINE__ << " : " << arg << std::endl << "\n\033[1;31m˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜\033[0;37m\n";
# define CERRANDEXIT std::cout << "\n\033[1;31m❌ ERROR\033[0;37m [File] " << __FILE__ << " [line] " << __LINE__ << " : ";