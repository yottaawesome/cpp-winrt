module;

#include <type_traits>
#include <memory>
#include <string>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.data.json.h>

export module json;

using std::string_view;

namespace JSON
{
	struct RootObjectImpl;
}

export namespace JSON
{
	enum class JSONValueType
	{
		Object,
		Array,
		String,
		Number
	};

	template<JSONValueType T>
	struct Value : std::false_type { };

	class JSONObject final
	{
		public:
			JSONObject();
			~JSONObject();

			JSONObject(const JSONObject&) = delete;
			JSONObject& operator=(const JSONObject&) = delete;

			JSONObject(JSONObject&&) noexcept;
			JSONObject& operator=(JSONObject&&) noexcept;

		private:
			void Close();

		private:
			RootObjectImpl* m_obj; // redefinition bug
	};

	template<>
	struct Value<JSONValueType::Object>
	{
		std::string Name;
		JSONObject Data;
	};

	template<>
	struct Value<JSONValueType::Array>
	{
		std::string Name;
	};

	template<>
	struct Value<JSONValueType::String>
	{
		std::string Name;
		std::wstring Data;
	};

	template<>
	struct Value<JSONValueType::Number>
	{
		std::string Name;
		double Data = 0;
	};
}

module :private;

namespace JSON
{
	struct RootObjectImpl
	{
		void DoSomething() {}

		winrt::Windows::Data::Json::JsonObject m_root = nullptr;
	};

	JSONObject::JSONObject()
		: m_obj(new RootObjectImpl())
	{ }

	JSONObject::~JSONObject()
	{
		Close();
	}

	JSONObject::JSONObject(JSONObject&& other) noexcept
	{
		m_obj = other.m_obj;
		other.m_obj = nullptr;
	}

	JSONObject& JSONObject::operator=(JSONObject&& other) noexcept
	{
		Close();
		m_obj = other.m_obj;
		other.m_obj = nullptr;
		return *this;
	}

	void JSONObject::Close()
	{
		if (m_obj)
		{
			delete m_obj;
			m_obj = nullptr;
		}
	}
}