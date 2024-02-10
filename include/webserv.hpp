#pragma once

#include "libs.hpp"
#include "SmartTypes.hpp"
#include "ServConfig.hpp"
#include "Route.hpp"
#include "Codes.hpp"
#include "Parser.hpp"
#include "Data.hpp"
#include "request.hpp"
#include "Response.hpp"

//timeouts
#define INACTIVE_TO 120
#define SEND_TO 30
#define RECV_TO 30
#define ACCEPT_TO 15

//memory|perfs limits
#define MAX_EVENTS 255
#define BUF_SIZE 2048
