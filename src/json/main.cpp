
import std;
import testmodule;

// https://docs.microsoft.com/en-us/uwp/api/windows.data.json?view=winrt-22621
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

JSON::IJsonValue GetValueByPath(JSON::JsonObject& root, const std::vector<std::wstring>& path)
{
    if (path.size() == 1)
    {
        if (root.HasKey(path[0]))
            root.GetNamedValue(path[0]);
        return nullptr;
    }

    JSON::JsonObject current = root;
    for (const std::wstring& entry : path)
    {
        if (current.HasKey(entry))
        {
            if (current.GetNamedValue(entry).ValueType() == JSON::JsonValueType::Object)
            {
                current = root.GetNamedObject(entry);
            }
            else
            {
                return current.GetNamedValue(entry);
            }
        }
        else
        {
            return nullptr;
        }
    }

    return current;
}

void Two() try
{
    constexpr std::wstring_view json = LR"({"field1":{"innerField":1},"field2":{"innerField":2},"field/field":false})";
    JSON::JsonObject root = JSON::JsonObject::Parse(json.data());

    GetValueByPath(root, { L"field1", L"innerField" });

    // this works
    std::wcout << root.GetNamedBoolean(L"field/field") << std::endl;

    for (const Collections::IKeyValuePair& f : root)
    {
        std::wstring out = L"";

        std::wcout << std::format(
            L"Name: {} -- Type: {} -- Value: {}\n", 
            std::wstring{ f.Key() },
            TypeToString(f.Value().ValueType()),
            out
        );
    }

}
catch (const JSON::hresult_error& err)
{
    std::wcout << std::wstring{err.message()} << std::endl;
}

int main()
{
    Two();
    return 0;
}
