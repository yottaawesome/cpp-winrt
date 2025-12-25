#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "runtimeobject.lib")

#include <SDKDDKVer.h>
#include <Windows.h>
#include <Psapi.h>
#include <strsafe.h>
#include <ShObjIdl.h>
#include <Shlobj.h>
#include <Pathcch.h>
#include <propvarutil.h>
#include <propkey.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <windows.ui.notifications.h>
#include "NotificationActivationCallback.h"
#include <winrt/base.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.ui.notifications.h>
#include <winrt/windows.data.xml.dom.h>
#include <iostream>

#pragma comment(lib, "Pathcch.lib")

const wchar_t AppId[] = L"Microsoft.Samples.DesktopToasts";

struct CoTaskMemStringTraits
{
    typedef PWSTR Type;

    inline static bool Close(_In_ Type h) throw() { ::CoTaskMemFree(h); return true; }

    inline static Type GetInvalidValue() throw() { return nullptr; }
};
typedef Microsoft::WRL::Wrappers::HandleT<CoTaskMemStringTraits> CoTaskMemString;

// For the app to be activated from Action Center, it needs to provide a COM server to be called
// when the notification is activated.  The CLSID of the object needs to be registered with the
// OS via its shortcut so that it knows who to call later.
class DECLSPEC_UUID("23A5B06E-20BB-4E7E-A0AC-6982ED6A6041") NotificationActivator WrlSealed
    : public Microsoft::WRL::RuntimeClass < Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
    INotificationActivationCallback > WrlFinal
{
public:
    virtual HRESULT STDMETHODCALLTYPE Activate(
        _In_ LPCWSTR /*appUserModelId*/,
        _In_ LPCWSTR /*invokedArgs*/,
        /*_In_reads_(dataCount)*/ const NOTIFICATION_USER_INPUT_DATA* /*data*/,
        ULONG /*dataCount*/) override
    {
        return S_OK;// DesktopToastsApp::GetInstance()->SetMessage(L"NotificationActivator - The user clicked on the toast.");
    }
};
CoCreatableClass(NotificationActivator);

HRESULT InstallShortcut(PCWSTR shortcutPath, PCWSTR exePath)
{
    Microsoft::WRL::ComPtr<IShellLink> shellLink;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));
    if (FAILED(hr))
        return hr;

    hr = shellLink->SetPath(exePath);
    if (FAILED(hr))
        return hr;

    Microsoft::WRL::ComPtr<IPropertyStore> propertyStore;

    hr = shellLink.As(&propertyStore);
    if (FAILED(hr))
        return hr;

    PROPVARIANT propVar;
    propVar.vt = VT_LPWSTR;
    propVar.pwszVal = const_cast<PWSTR>(AppId); // for _In_ scenarios, we don't need a copy
    hr = propertyStore->SetValue(PKEY_AppUserModel_ID, propVar);
    if (FAILED(hr))
        return hr;

    propVar.vt = VT_CLSID;
    propVar.puuid = const_cast<CLSID*>(&__uuidof(NotificationActivator));
    hr = propertyStore->SetValue(PKEY_AppUserModel_ToastActivatorCLSID, propVar);
    if (FAILED(hr))
        return hr;

    hr = propertyStore->Commit();
    if (FAILED(hr))
        return hr;

    Microsoft::WRL::ComPtr<IPersistFile> persistFile;
    hr = shellLink.As(&persistFile);
    if (FAILED(hr))
        return hr;

    hr = persistFile->Save(shortcutPath, TRUE);

    return hr;
}

HRESULT RegisterComServer(PCWSTR exePath)
{
    // We don't need to worry about overflow here as ::GetModuleFileName won't
    // return anything bigger than the max file system path (much fewer than max of DWORD).
    DWORD dataSize = static_cast<DWORD>((::wcslen(exePath) + 1) * sizeof(WCHAR));

    // In this sample, the app UI is registered to launch when the COM callback is needed.
    // Other options might be to launch a background process instead that then decides to launch
    // the UI if needed by that particular notification.
    return HRESULT_FROM_WIN32(::RegSetKeyValue(
        HKEY_CURRENT_USER,
        LR"(SOFTWARE\Classes\CLSID\{23A5B06E-20BB-4E7E-A0AC-6982ED6A6041}\LocalServer32)",
        nullptr,
        REG_SZ,
        reinterpret_cast<const BYTE*>(exePath),
        dataSize));
}

HRESULT RegisterAppForNotificationSupport()
{
    CoTaskMemString appData;
    auto hr = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, appData.GetAddressOf());
    if (FAILED(hr))
        return hr;

    wchar_t shortcutPath[MAX_PATH];
    hr = ::PathCchCombine(shortcutPath, ARRAYSIZE(shortcutPath), appData.Get(), LR"(Microsoft\Windows\Start Menu\Programs\winrt-desktop-toasts.lnk)");
    if (FAILED(hr))
        return hr;

    DWORD attributes = ::GetFileAttributes(shortcutPath);
    bool fileExists = attributes < 0xFFFFFFF;
    if (fileExists)
        return hr;

    wchar_t exePath[MAX_PATH];
    DWORD charWritten = ::GetModuleFileName(nullptr, exePath, ARRAYSIZE(exePath));
    hr = charWritten > 0 ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
    if (FAILED(hr))
        return hr;

    hr = InstallShortcut(shortcutPath, exePath);
    if (FAILED(hr))
        return hr;

    hr = RegisterComServer(exePath);

    return hr;
}

HRESULT RegisterActivator()
{
    // Module<OutOfProc> needs a callback registered before it can be used.
    // Since we don't care about when it shuts down, we'll pass an empty lambda here.
    Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::Create([] {});

    // If a local server process only hosts the COM object then COM expects
    // the COM server host to shutdown when the references drop to zero.
    // Since the user might still be using the program after activating the notification,
    // we don't want to shutdown immediately.  Incrementing the object count tells COM that
    // we aren't done yet.
    Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().IncrementObjectCount();

    return Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().RegisterObjects();
}

int main()
{
    try
    {
        winrt::init_apartment();
        RegisterAppForNotificationSupport();
        RegisterActivator();

        winrt::Windows::Data::Xml::Dom::XmlDocument doc;
        doc.LoadXml(L"<toast></toast>");
        winrt::Windows::UI::Notifications::ToastNotification toast(doc);
        toast.Failed(
            [](const winrt::Windows::UI::Notifications::ToastNotification& s, const winrt::Windows::UI::Notifications::ToastFailedEventArgs& t)
            {
                std::wcout << L"Failed!" << std::endl;
            }
        );

        toast.Activated(
            [](const auto& s, const auto&)
            {
                std::wcout << L"I'm activated!" << std::endl;
            }
        );

        toast.Dismissed(
            [](const winrt::Windows::UI::Notifications::ToastNotification& s, const winrt::Windows::UI::Notifications::ToastDismissedEventArgs& t)
            {
                std::wcout << L"I'm dismissed!" << std::endl;
            });
        winrt::Windows::UI::Notifications::ToastNotifier notifier = winrt::Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier(AppId);
        notifier.Show(toast);

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    catch (const winrt::hresult_error& ex)
    {
        std::wcerr << ex.message().c_str() << std::endl;
    }

    return 0;
}

