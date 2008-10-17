#include "NetWatcherApp.h"
#include "NetWatchWin.h"

#include <cstdio>

int main(void)
{
	NetWatcherApp testApp("application/x-vnd.Simon.NetWatcher");

	// Start Application
	testApp.Run();

	return 0;
}

NetWatcherApp::NetWatcherApp(const char *signature)
: BApplication(signature)
{
	NetWatchWin* watchWin = 
	new NetWatchWin(
		BRect(50.00,100.00,300.00,400.00),
		"BNetEndpoint Watcher");

	// Show window
	watchWin->Show();
}

