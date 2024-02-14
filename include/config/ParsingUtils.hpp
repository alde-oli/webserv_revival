#pragma once

#include "libs.hpp"

#include "SmartTypes.hpp"
#include "Codes.hpp"
#include "Route.hpp"

void CerrExit(const char* message, std::string precision);
bool isLocalOrExternal(struct sockaddr_in addr);