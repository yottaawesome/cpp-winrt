import std; // blocked by compiler bugs with redefinitions
import json2;

int main()
{
    std::wstring s;
    JSON::Add(L"a", 1);
    JSON::JSONValue v;
    v = nullptr;
    v = std::wstring{ L"a" };

    bool b = std::holds_alternative<std::wstring>(v.Value);

    //JSON::DoSomethingWithAString(s);
    return 0;
}
