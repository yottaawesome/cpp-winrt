// See also: https://learn.microsoft.com/en-us/windows/uwp/networking/sockets#references-to-streamsockets-in-c-ppl-continuations

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
        dw.WriteUInt32(32);
        dw.StoreAsync().get();
        dw.FlushAsync().get();
        dw.DetachStream();
        
        ss.Close();
    }
    catch (const winrt::hresult_error& ex)
    {
        std::wcerr << std::format(L"Error: {}\n", ex.message());
    }

    return 0;
}
