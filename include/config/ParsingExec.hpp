#pragma once

#include "libs.hpp"

#include "SmartTypes.hpp"
#include "Codes.hpp"
#include "Route.hpp"
#include "ServConfig.hpp"
#include "ParsingExec.hpp"
#include "ParsingUtils.hpp"

static int					countDeuxPoints(std::string &line);
static bool 				hasDuplicateAddress(const std::vector<ServConfig>& servers);
void 						setServer(std::fstream &file, std::string &line, ServConfig &server);
std::vector<ServConfig>		ServerParsing(char *filename);