#pragma comment(lib, "runtimeobject.lib")

#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.networking.sockets.h>

import <iostream>;

int main()
{
    namespace Sockets = winrt::Windows::Networking::Sockets;
    
    // TODO: not sure how to go about this one. It appears you 
    // can't create an arbitrary websocket server.
    // https://learn.microsoft.com/en-us/uwp/api/windows.networking.sockets.servermessagewebsocket?view=winrt-22621

    return 0;
}
