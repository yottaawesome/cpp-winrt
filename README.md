# C++/WinRT

## Introduction

This repo house samples I've developed for familiarising myself with C++/WinRT. C++/WinRT is a modern C++ language projection over the Windows Runtime APIs. However, it is now in maitenance mode, and will probably be abandoned in the short to medium term.

## Building

You need Visual Studio 2026 with the _Desktop development with C++_ and the _Universal Windows Platform_ development workloads installed. Make sure the _C++/WinRT_ option under _SDKs, libraries, and frameworks_ in Visual Studio Installer is selected. You should then be able to open and build the cpp-winrt.sln file in Visual Studio. 

## Observed issues

C++/WinRT lacks C++20 module support. It's unclear at this point when or even if this will be added, but it is possible to consume C++/WinRT by stuffing it into and re-exporting it from a module. A possible workaround for this is to hide C++/WinRT behind wrapper functions and classes for only the functionality you actually need rather than consume it directly in client code. This is probably a good idea in most cases, as it means your codebase avoids a direct dependency on third-party code and those wrapper functions and classes can theoretically be updated to use a different implementation in the future without changing the client code.

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
