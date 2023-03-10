#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/windows.data.json.h>

import <iostream>;

namespace JSON = winrt::Windows::Data::Json;

int main()
{
    JSON::JsonObject root;
    JSON::JsonArray array1;

    for (int i = 0; i < 5; i++)
    {
        JSON::JsonArray internalArray;

        internalArray.Append(JSON::JsonValue::CreateNumberValue(1));
        internalArray.Append(JSON::JsonValue::CreateNumberValue(2));
        internalArray.Append(JSON::JsonValue::CreateNumberValue(3));

        array1.Append(internalArray);
    }

    root.SetNamedValue(L"values", array1);

    std::wcout << root.ToString().c_str() << std::endl;

    return 0;
}
