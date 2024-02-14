#pragma once

#include "../libs.hpp"

void CerrExit(const char* message, std::string precision);
bool isLocalOrExternal(struct sockaddr_in addr);