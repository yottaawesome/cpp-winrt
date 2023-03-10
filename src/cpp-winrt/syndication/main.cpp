// Adapted from https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/get-started

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Syndication.h>

import <iostream>;

int main()
{
    winrt::init_apartment();

    winrt::Windows::Foundation::Uri rssFeedUri{ L"https://blogs.windows.com/feed" };
    winrt::Windows::Web::Syndication::SyndicationClient syndicationClient;
    winrt::Windows::Web::Syndication::SyndicationFeed syndicationFeed = syndicationClient.RetrieveFeedAsync(rssFeedUri).get();
    for (const winrt::Windows::Web::Syndication::SyndicationItem syndicationItem : syndicationFeed.Items())
    {
        winrt::hstring titleAsHstring = syndicationItem.Title().Text();

        // A workaround to remove the trademark symbol from the title string, because it causes issues in this case.
        std::wstring titleAsStdWstring{ titleAsHstring.c_str() };
        titleAsStdWstring.erase(
            remove(titleAsStdWstring.begin(), titleAsStdWstring.end(), L'™'), 
            titleAsStdWstring.end()
        );
        titleAsHstring = titleAsStdWstring;

        std::wcout << titleAsHstring.c_str() << std::endl;
    }

    return 0;
}

