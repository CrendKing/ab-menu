#include "pch.h"
#include "environment.h"


class DiffMenuModule : public ATL::CAtlDllModuleT<DiffMenuModule> {
} g_module;

extern "C" auto WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) -> BOOL {
    if (dwReason == DLL_PROCESS_ATTACH) {
        Environment::INSTANCE = new Environment;
        if (!Environment::INSTANCE->Initialize(hInstance)) {
            return FALSE;
        }
    }
    if (dwReason == DLL_PROCESS_DETACH) {
        delete Environment::INSTANCE;
    }

    return g_module.DllMain(dwReason, lpReserved);
}

_Use_decl_annotations_
extern "C" auto STDAPICALLTYPE DllCanUnloadNow() -> HRESULT {
    return g_module.DllCanUnloadNow();
}

_Use_decl_annotations_
extern "C" auto STDAPICALLTYPE DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR *ppv) -> HRESULT {
    return g_module.DllGetClassObject(rclsid, riid, ppv);
}

_Use_decl_annotations_
extern "C" auto STDAPICALLTYPE DllRegisterServer() -> HRESULT {
    return g_module.DllRegisterServer(FALSE);
}

_Use_decl_annotations_
extern "C" auto STDAPICALLTYPE DllUnregisterServer() -> HRESULT {
    return g_module.DllUnregisterServer(FALSE);
}

_Use_decl_annotations_
extern "C" auto STDAPICALLTYPE DllInstall(BOOL bInstall, _In_opt_ PCWSTR pszCmdLine) -> HRESULT {
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = L"user";

    if (pszCmdLine != nullptr) {
        if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0) {
            ATL::AtlSetPerUserRegistration(true);
        }
    }

    if (bInstall) {
        hr = DllRegisterServer();
        if (FAILED(hr)) {
            DllUnregisterServer();
        }
    } else {
        hr = DllUnregisterServer();
    }

    return hr;
}
