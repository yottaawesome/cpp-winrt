#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/windows.data.json.h>

import <iostream>;

// https://docs.microsoft.com/en-us/uwp/api/windows.data.json?view=winrt-22621
namespace JSON = winrt::Windows::Data::Json;

int main()
{
    JSON::JsonObject root;
    JSON::JsonArray arrayValue;

    for (int i = 0; i < 5; i++)
    {
        JSON::JsonArray internalArray;

        internalArray.Append(JSON::JsonValue::CreateNumberValue(1));
        internalArray.Append(JSON::JsonValue::CreateNumberValue(2));
        internalArray.Append(JSON::JsonValue::CreateNumberValue(3));

        arrayValue.Append(internalArray);
    }

    root.SetNamedValue(L"values", arrayValue);

    std::wcout << root.ToString().c_str() << std::endl;

    return 0;
}
