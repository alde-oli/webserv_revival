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
#include "Client.hpp"
#include "ServRunner.hpp"

//timeouts
//acceptable to for inactive are 60-120s
#define INACTIVE_TO 120
//acceptable to for kevent are 3-10s
#define KEVENT_TO 3
//acceptable to for sread and write are 5-30s
#define SEND_TO 30
#define RECV_TO 30
#define ACCEPT_TO 15

//memory|perfs limits
#define MAX_EVENTS 255
#define BUF_SIZE 2048
