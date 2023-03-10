#include <windows.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>

import <iostream>;
import <format>;

namespace Sockets = winrt::Windows::Networking::Sockets;
namespace Networking = winrt::Windows::Networking;

void OnConnectionReceived(
    winrt::Windows::Networking::Sockets::StreamSocketListener /* sender */, 
    winrt::Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs args
)
{
    try
    {
        std::wcout << L"Hello" << std::endl;
        auto socket{ args.Socket() };
        winrt::Windows::Storage::Streams::DataReader dr{ socket.InputStream() };
        int bytes = dr.LoadAsync(sizeof(unsigned int)).get();
        std::wcout << bytes << std::endl;
        int data = dr.ReadInt32();
        std::wcout << L"2" << std::endl;
        //winrt::hstring hs = dr.ReadLi(2);
        std::wcout << data << std::endl;
    }
    catch (const winrt::hresult_error& ex)
    {
        std::wcerr << std::format(L"Error: {}\n", ex.message());
    }
}

int main()
{
    winrt::init_apartment();

    Sockets::StreamSocketListener ssl;
    ssl.ConnectionReceived(&OnConnectionReceived);
    
    winrt::Windows::Foundation::IAsyncAction async = ssl.BindServiceNameAsync(L"59725");
    async.get();
    
    while (true)
    {
        Sleep(1000);
    }

    return 0;
}
