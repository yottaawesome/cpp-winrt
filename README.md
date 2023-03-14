# C++/WinRT

## Introduction

This repo house samples I've developed for familiarising myself with C++/WinRT. C++/WinRT is a modern C++ language projection over the Windows Runtime APIs.

## Building

You need Visual Studio 2022 with the _Desktop development with C++_ and the _Universal Windows Platform_ development workloads installed. Make sure the _C++/WinRT_ option under _SDKs, libraries, and frameworks_ in Visual Studio Installer is selected. You should then be able to open and build the cpp-winrt.sln file in Visual Studio. 

## Samples

* json: shows basic JSON manipulation;
* message-websocket-client: a websocket client that echoes out to the `echo.websocket.events` service;
* message-websocket-server: work-in-progress, not yet implemented;
* stream-websocket-client: a TCP stream socket client that works with `stream-websocket-server`;
* stream-websocket-server: a TCP stream socket server that works with `stream-websocket-client`;
* syndication: basic syndication sample provided by MSDN.

## Additional Resources

* [C++/WinRT on MSDN](https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/)
