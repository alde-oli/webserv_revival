#include "../../include/config/Route.hpp"

void	Route::verif() const
{
	int count = 0;

	if (_id.empty())
		CerrExit("Route ID is empty please insert a valid Route ID", "");
	else if (_path.empty())
		CerrExit("Route is empty please insert a valid Route", "");
	else if (_root.empty())
		CerrExit("Root is empty please insert a valid Root", "");
	else if (_defaultPage.empty())
		CerrExit("Page is empty please insert a valid Page", "");
	else if (_methods.empty())
		CerrExit("Methods are empty please insert valids Methods", "");
	else if (_isDownload && _downloadDir.empty())
		CerrExit("DownloadDir is empty please insert a valid DownloadDir", "");
	else if (_isRedir && _redirDir.empty())
		CerrExit("RedirDir is empty please insert a valid RedirDir", "");
	else if (_listing)
		count++;
	else if (_isRedir)
		count++;
	if (count == 2 && !_defaultPage.empty())
		CerrExit("Multiple requests type not supported", "");
}