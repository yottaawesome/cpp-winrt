module;

#include <type_traits>
#include <memory>
#include <variant>
#include <optional>
#include <vector>
#include <string>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.data.json.h>

export module json2;

export namespace JSON
{
	template<typename T>
	concept Name = std::is_same_v<T, std::wstring> or std::is_same_v<T, std::wstring_view> or std::is_convertible_v<T, std::wstring_view>;

	template<typename...T>
	concept AllStrings = (... and std::same_as<T, std::wstring>);

	template<typename T, typename...Ts>
	concept OneOf = (... or std::same_as<T, Ts>);

	template<typename T>
	concept Type = Name<T> or std::is_convertible_v<T, double>;

	void Add(const Name auto& arg, Type auto&& val)
	{
		//static_assert(false, "false");
	}

	template<typename...Types>
	struct BasicValue
	{
		std::optional<std::wstring> Name;
		std::variant<Types...> Value;

		auto operator=(OneOf<Types...> auto&& value) noexcept(std::is_nothrow_assignable_v<decltype(value), std::variant<Types...>>)
		{
			Value = value;
			return *this;
		}

		template<OneOf<Types...> T>
		const T& Get()
		{
			if (not std::holds_alternative<T>(Value))
				throw std::runtime_error("Wrong type");
			return std::get<T>(Value);
		}

		template<OneOf<Types...> T>
		bool Is() const noexcept
		{
			return std::holds_alternative<T>(Value);
		}
	};

	struct JSONObject;
	using JSONValue = BasicValue<double, std::wstring, JSONObject, std::vector<JSONObject>, std::nullptr_t>;

	struct JSONObject
	{
		std::vector<JSONValue> Value;
	};
}

module :private;

namespace JSON
{

}