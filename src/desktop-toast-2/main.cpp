// Adapted from https://gist.github.com/kennykerr/d983767262118ae0366ef1ec282e428a
#pragma comment(lib, "onecore")
#pragma comment(lib, "propsys")
#pragma comment(lib, "runtimeobject.lib")

#include <shlobj.h>
#include <propvarutil.h>
#include <propkey.h>
#include <notificationactivationcallback.h>
#include <winrt/Windows.foundation.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <iostream>

using namespace winrt;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml::Dom;

static constexpr GUID callback_guid // B85B068A-169E-420F-B70E-5FEFCA71AE28
{
    0xB85B068A, 0x169E, 0x420F, {0xB7, 0x0E, 0x5F, 0xEF, 0xCA, 0x71, 0xAE, 0x28}
};

struct callback : winrt::implements<callback, INotificationActivationCallback>
{
    HRESULT __stdcall Activate(
        [[maybe_unused]] LPCWSTR app,
        [[maybe_unused]] LPCWSTR args,
        [[maybe_unused]] NOTIFICATION_USER_INPUT_DATA const* data,
        [[maybe_unused]] ULONG count) noexcept final
    {
        printf("\nCallback\n");
        return S_OK;
    }
};

struct callback_factory : implements<callback_factory, IClassFactory>
{
    HRESULT __stdcall CreateInstance(
        IUnknown* outer,
        GUID const& iid,
        void** result) noexcept final
    {
        *result = nullptr;

        if (outer)
        {
            return CLASS_E_NOAGGREGATION;
        }

        return make<callback>()->QueryInterface(iid, result);
    }

    HRESULT __stdcall LockServer(BOOL) noexcept final
    {
        return S_OK;
    }
};

struct prop_variant : PROPVARIANT
{
    prop_variant() noexcept : PROPVARIANT{}
    {
    }

    ~prop_variant() noexcept
    {
        clear();
    }

    void clear() noexcept
    {
        WINRT_VERIFY_(S_OK, PropVariantClear(this));
    }
};

struct registry_traits
{
    using type = HKEY;

    static void close(type value) noexcept
    {
        WINRT_VERIFY_(ERROR_SUCCESS, RegCloseKey(value));
    }

    static constexpr type invalid() noexcept
    {
        return nullptr;
    }
};

using registry_key = winrt::handle_type<registry_traits>;

std::wstring get_module_path()
{
    std::wstring path(100, L'?');
    DWORD actual_size{};

    while (true)
    {
        actual_size = GetModuleFileNameW(nullptr, path.data(), 1 + static_cast<uint32_t>(path.size()));

        if (actual_size < 1 + path.size())
        {
            path.resize(actual_size);
            break;
        }
        else
        {
            path.resize(path.size() * 2, L'?');
        }
    }

    return path;
}

std::wstring get_shortcut_path()
{
    // This was the original code, but updating this path requires admin privileges,
    // I've updated the sample to install it in the user's roaming profile.
    //auto format = LR"(%ProgramData%\Microsoft\Windows\Start Menu\Programs\SampleApp.lnk)";
    //auto required = ExpandEnvironmentStringsW(format, nullptr, 0);
    //std::wstring path(required - 1, L'?');
    //ExpandEnvironmentStringsW(format, path.data(), required);
    //return path;

    PWSTR appData = nullptr;
    auto hr = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &appData);
    if (FAILED(hr))
    {
        CoTaskMemFree(appData);
        throw std::runtime_error("Failed");
    }
    std::wstring format(appData);
    CoTaskMemFree(appData);
    format += L"\\SampleApp.lnk";
    return format;
}

void create_shortcut()
{
    auto link = create_instance<IShellLinkW>(CLSID_ShellLink);
    check_hresult(link->SetPath(get_module_path().c_str()));

    auto store = link.as<IPropertyStore>();
    prop_variant value;
    check_hresult(InitPropVariantFromString(L"SampleApp", &value));
    check_hresult(store->SetValue(PKEY_AppUserModel_ID, value));
    value.clear();
    check_hresult(InitPropVariantFromCLSID(callback_guid, &value));
    check_hresult(store->SetValue(PKEY_AppUserModel_ToastActivatorCLSID, value));

    auto file = store.as<IPersistFile>();
    check_hresult(file->Save(get_shortcut_path().c_str(), TRUE));
}

void update_registry()
{
    std::wstring key_path{ LR"(SOFTWARE\Classes\CLSID\{????????-????-????-????-????????????})" };
    StringFromGUID2(callback_guid, key_path.data() + 23, 39);
    key_path += LR"(\LocalServer32)";
    registry_key key;

    check_win32(RegCreateKeyExW(
        HKEY_CURRENT_USER,
        key_path.c_str(),
        0,
        nullptr,
        0,
        KEY_WRITE,
        nullptr,
        key.put(),
        nullptr));

    auto path = get_module_path();

    check_win32(RegSetValueExW(
        key.get(),
        nullptr,
        0,
        REG_SZ,
        reinterpret_cast<BYTE const*>(path.c_str()),
        static_cast<uint32_t>((path.size() + 1) * sizeof(wchar_t))));
}

void register_callback()
{
    DWORD registration{};

    check_hresult(CoRegisterClassObject(
        callback_guid,
        make<callback_factory>().get(),
        CLSCTX_LOCAL_SERVER,
        REGCLS_SINGLEUSE,
        &registration));
}

void create_toast()
{
    XmlDocument xml;

    xml.LoadXml(
        LR"(
<toast>
  <visual>
    <binding template='ToastGeneric'>
      <text>Sample App</text>
    </binding>
  </visual>
  <actions>
    <action content='Callback' arguments='args' activationKind='Background' />
  </actions>
</toast>
)");

    ToastNotification toast{ xml };
    auto notifier = ToastNotificationManager::CreateToastNotifier(L"SampleApp");
    toast.Activated([](const auto&, const auto&)
        {
            std::wcout << "ActivATED\n";
        });
    notifier.Show(toast);
}

int main()
{
    try
    {
        init_apartment();

        //if (IsUserAnAdmin()) // Not require when installing to user's roaming profile
        {
            create_shortcut();
            update_registry();
        }

        register_callback();
        printf("\nSample app is running%s\n", IsUserAnAdmin() ? " (Administrator)" : "");
        printf("\nPress 'T' to create toast or any other key to exit\n");

        if (toupper(getchar()) == 'T')
        {
            create_toast();
        }

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //Sleep(1000);
    }
    catch (hresult_error const& e)
    {
        printf("Error: %ls (0x%8X)\n", e.message().c_str(), static_cast<uint32_t>(e.code()));
    }
}
