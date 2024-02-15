#include "libs.hpp"
#include "ServConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

static bool	rCgi()
{

}

bool	RequestHandler::rGet(Request &request, Response &response, ServConfig &server, int socket)
{
}

bool	RequestHandler::rPost(Request &request, Response &response, ServConfig &server, int socket)
{
}

bool	RequestHandler::rDel(Request &request, Response &response, ServConfig &server, int socket)
{
	std::string path = request["uri"].substr(0, request["uri"].find_last_of('/') + 1); //need to check it is correctly truncated

	if (!server.isRoute(path))
		{response.setCode(404); return true;}
	
	Route &route = server.getRoute(path);
	#include <cstdio> // Add this line to include the <cstdio> header

	if (route.getMethods().find("DELETE") == route.getMethods().end())
		{response.setCode(405); return true;}

	std::string toDel = request["uri"].substr(request["uri"].find_last_of('/') + 1); // need to check it is correctly truncated
	if (toDel == "")
		{response.setCode(400); return true;}

	toDel = route.getRoot() + toDel;

	if (std::remove(toDel.c_str()) != 0)
		{response.setCode(500); return true;}
	response.setCode(204);
	response.setKeepAlive(true);
	return true;
}
