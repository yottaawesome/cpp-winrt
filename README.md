# C++/WinRT

## Introduction

This repo house samples I've developed for familiarising myself with C++/WinRT. C++/WinRT is a modern C++ language projection over the Windows Runtime APIs.

## Building

You need Visual Studio 2022 with the _Desktop development with C++_ and the _Universal Windows Platform_ development workloads installed. Make sure the _C++/WinRT_ option under _SDKs, libraries, and frameworks_ in Visual Studio Installer is selected. You should then be able to open and build the cpp-winrt.sln file in Visual Studio. 

## Observed issues

C++/WinRT lacks C++20 module support. It's unclear at this point when or even if this will be added. It is possible to consume C++/WinRT by stuffing it into and re-exporting it from a module, but I found that this doesn't play well with client code that consumes the standard module `std` ([redefinition bug](https://developercommunity.visualstudio.com/t/VS2022-175-Preview-3---Compiler-bug-wit/10256508)). A possible workaround for this is to hide C++/WinRT behind wrapper functions and classes for only the functionality you actually need rather than consume it directly in client code. This is probably a good idea in most cases, as it means your codebase avoids a direct dependency and those wrapper functions and classes can theoretically be updated to use a different implementation in the future without changing the client code.

## Samples

* `json`: shows basic JSON manipulation;
* `message-websocket-client`: a websocket client that echoes out to the `echo.websocket.events` service;
* `message-websocket-server`: work-in-progress, not yet implemented;
* `stream-websocket-client`: a TCP stream socket client that works with `stream-websocket-server`;
* `stream-websocket-server`: a TCP stream socket server that works with `stream-websocket-client`;
* `syndication`: basic syndication sample provided by MSDN.
* `hello-world`: Universal Windows MSDN sample.
* `http-client`: demonstrates use of the `HttpClient` class.

## Additional Resources

* [C++/WinRT on MSDN](https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/)
