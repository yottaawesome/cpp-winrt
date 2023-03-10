#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/windows.data.json.h>

import <iostream>;
import <random>;

// https://docs.microsoft.com/en-us/uwp/api/windows.data.json?view=winrt-22621
namespace JSON = winrt::Windows::Data::Json;

int main()
{
    JSON::JsonObject root;
    JSON::JsonArray arrayValue;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6); // distribution in range [1, 6]

    for (int i = 0; i < 5; i++)
    {
        JSON::JsonArray internalArray;
        for (int j = 0; j < 3; j++)
            internalArray.Append(JSON::JsonValue::CreateNumberValue(dist6(rng)));

        arrayValue.Append(internalArray);
    }

    root.SetNamedValue(L"values", arrayValue);

    std::wcout << root.ToString().c_str() << std::endl;

    return 0;
}
