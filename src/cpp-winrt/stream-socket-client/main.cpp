#include <windows.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>

import <iostream>;

int main()
{
    try
    {
        winrt::init_apartment();

        namespace Sockets = winrt::Windows::Networking::Sockets;
        namespace Networking = winrt::Windows::Networking;

        Sockets::StreamSocket ss;
        winrt::Windows::Foundation::IAsyncAction async = ss.ConnectAsync(
            Networking::HostName(L"localhost"), 
            L"59725", 
            Sockets::SocketProtectionLevel::PlainSocket
        );

        async.get();
        Sleep(1000);

        winrt::Windows::Storage::Streams::DataWriter dw(ss.OutputStream());
        //dw.WriteString(L"Test");
        dw.WriteUInt32(32);
        dw.WriteUInt32(32);
        dw.WriteUInt32(32);
        dw.WriteUInt32(6);
        dw.FlushAsync().get();

        winrt::Windows::Storage::Streams::DataReader dr{ ss.InputStream() };


        while (true)
        {
            Sleep(1000);
        }

        ss.Close();
    }
    catch (...)
    {
        std::wcout << L"Error\n";
    }

    return 0;
}
