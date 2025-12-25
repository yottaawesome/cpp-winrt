#pragma comment(lib, "runtimeobject.lib")

// See also: https://learn.microsoft.com/en-us/windows/uwp/networking/sockets#references-to-streamsockets-in-c-ppl-continuations
// Run the stream-socket-server project first; 
// this application will connect to that app's
// socket.

#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>

import <iostream>;
import <format>;

int main()
{
    namespace Sockets = winrt::Windows::Networking::Sockets;
    namespace Networking = winrt::Windows::Networking;
    namespace Streams = winrt::Windows::Storage::Streams;

    try
    {
        winrt::init_apartment();

        Sockets::StreamSocket ss;
        winrt::Windows::Foundation::IAsyncAction async = ss.ConnectAsync(
            Networking::HostName(L"localhost"), 
            L"59725", 
            Sockets::SocketProtectionLevel::PlainSocket
        );
        async.get();

        Streams::DataWriter dw(ss.OutputStream());
        std::wcout << (L"Enter a string to send to the server...\n");
        std::wstring output;
        std::getline(std::wcin, output);

        dw.WriteUInt32(output.size());
        dw.WriteString(output);
        dw.StoreAsync().get();
        dw.FlushAsync().get();
        // Some sample code on MSDN also detach 
        // the stream, but this is probably not 
        // necessary.
        
        ss.Close();
    }
    catch (const winrt::hresult_error& ex)
    {
        std::wcerr << std::format(L"Error: {}\n", ex.message());
    }

    return 0;
}
