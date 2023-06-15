#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/windows.data.json.h>

import <iostream>;
import <random>;
import <format>;

// https://docs.microsoft.com/en-us/uwp/api/windows.data.json?view=winrt-22621
namespace JSON = winrt::Windows::Data::Json;

void One()
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
}

using IKeyValuePair = winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, JSON::IJsonValue>;

std::wstring TypeToString(const JSON::JsonValueType& value)
{
    switch (value)
    {
        case JSON::JsonValueType::Array:
            return L"Array";
        case JSON::JsonValueType::Boolean:
            return L"Boolean";
        case JSON::JsonValueType::Null:
            return L"Null";
        case JSON::JsonValueType::Number:
            return L"Number";
        case JSON::JsonValueType::Object:
            return L"Object";
        case JSON::JsonValueType::String:
            return L"String";
    }
    return L"Unknown";
}

struct X
{
    JSON::JsonValueType Type;
};

template<JSON::JsonValueType Type>
auto JSONValueToValue(const JSON::IJsonValue& value)
{
    if constexpr (Type == JSON::JsonValueType::Array)
    {
        return L"NotSupported";

    }
    else
    {
        return L"Unknown";

    }
}

void Two() try
{
    constexpr std::wstring_view json = LR"({"field1":{"innerField":1},"field2":{"innerField":2},"field/field":false})";
    JSON::JsonObject root = JSON::JsonObject::Parse(json.data());

    // this works
    std::wcout << root.GetNamedBoolean(L"field/field") << std::endl;


    for (const IKeyValuePair& f : root)
    {
        //JSONValueToValue<JSON::JsonValueType::Array>(f.Value());
        std::wcout << std::format(
            L"Name: {} -- Type: {} -- Value: \n", 
            f.Key(), 
            TypeToString(f.Value().ValueType())
        );
    }

}
catch (const winrt::hresult_error& err)
{
    std::wcout << std::wstring{err.message()} << std::endl;
}

int main()
{
    Two();
    return 0;
}
