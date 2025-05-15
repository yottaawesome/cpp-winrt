module;

#include <memory.h>
#include "memcpy_shim.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/windows.data.json.h>

export module testmodule;

namespace WinRTJSON = winrt::Windows::Data::Json;

// Can't be namespaced
export
{
	using winrt::Windows::Foundation::operator==;
	using winrt::Windows::Foundation::operator!=;
}

export namespace Collections
{
	using IKeyValuePair = winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, WinRTJSON::IJsonValue>;
}

export namespace JSON
{
	using winrt::hresult_error;
	using WinRTJSON::JsonValueType;
	using WinRTJSON::IJsonValue;
	using WinRTJSON::JsonObject;
	using WinRTJSON::JsonArray;
	using WinRTJSON::JsonValue;
}
