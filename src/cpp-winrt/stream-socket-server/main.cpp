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
    namespace Foundation = winrt::Windows::Foundation;

    winrt::init_apartment();

    Sockets::StreamSocketListener ssl;
    ssl.ConnectionReceived(
        [](
            const Sockets::StreamSocketListener& /* sender */,
            const Sockets::StreamSocketListenerConnectionReceivedEventArgs& args
        )
        {
            try
            {
                std::wcout << L"Received a socket connection...\n";
                Sockets::StreamSocket socket{ args.Socket() };
                Streams::DataReader reader{ socket.InputStream() };
                int bytes = reader.LoadAsync(sizeof(unsigned int)).get();
                int data = reader.ReadInt32();
                std::wcout << std::format(L"Read {} bytes: {}\n", bytes, data);
            }
            catch (const winrt::hresult_error& ex)
            {
                std::wcerr << std::format(L"Error: {}\n", ex.message());
            }
        });
    
    Foundation::IAsyncAction async = ssl.BindServiceNameAsync(L"59725");
    std::wcout << L"Waiting for connection...\n";
    async.get();
    
    std::wstring x;
    std::wcin >> x;

    return 0;
}
