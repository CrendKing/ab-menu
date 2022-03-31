#include "menu.h"

#include "commands.h"
#include "environment.h"


static constexpr const WCHAR *REGISTRY_AB_MENU_KEY_PREFIX = LR"(SOFTWARE\AB Menu)";

OBJECT_ENTRY_AUTO(__uuidof(CABMenu), CABMenu)

auto STDMETHODCALLTYPE CABMenu::Initialize(__RPC__in_string LPCWSTR pszCommandName, __RPC__in_opt IPropertyBag *ppb) -> HRESULT {
    if (Environment::INSTANCE->apps.empty()) {
        HKEY registryKey;

        RegCreateKeyExW(HKEY_CURRENT_USER, REGISTRY_AB_MENU_KEY_PREFIX, 0, nullptr, 0, KEY_QUERY_VALUE, nullptr, &registryKey, nullptr);

        LSTATUS regRet = ERROR_SUCCESS;
        for (int i = 0; regRet == ERROR_SUCCESS; ++i) {
            std::array<WCHAR, 1024> regValueName;
            std::array<BYTE, 1024> regValueValue;
            DWORD regNameSize = static_cast<DWORD>(regValueName.size());
            DWORD regValueSize = static_cast<DWORD>(regValueValue.size());

            regRet = RegEnumValueW(registryKey, i, regValueName.data(), &regNameSize, nullptr, nullptr, regValueValue.data(), &regValueSize);
            if (regRet == ERROR_SUCCESS && regValueSize > 0) {
                const WCHAR *regValueStr = reinterpret_cast<const WCHAR *>(regValueValue.data());
                int cmdArgc;
                const std::wstring iconPath = std::format(L"{},0", CommandLineToArgvW(regValueStr, &cmdArgc)[0]);
                Environment::INSTANCE->apps.emplace_back(regValueName.data(), regValueStr, iconPath);
            }
        }

        RegCloseKey(registryKey);

        Environment::INSTANCE->LoadSelectedItem();
    }

    return S_OK;
}

auto STDMETHODCALLTYPE CABMenu::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"AB Menu", ppszName);
}

auto STDMETHODCALLTYPE CABMenu::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return SHStrDupW(Environment::INSTANCE->menuIconPath.c_str(), ppszIcon);
}

auto STDMETHODCALLTYPE CABMenu::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    *pCmdState = Environment::INSTANCE->apps.empty() ? ECS_DISABLED : ECS_ENABLED;
    return S_OK;
}

auto STDMETHODCALLTYPE CABMenu::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_HASSUBCOMMANDS;
    return S_OK;
}

auto STDMETHODCALLTYPE CABMenu::EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT {
    AddRef();
    _subMenuIndex = 0;
    *ppEnum = this;
    return S_OK;
}

auto STDMETHODCALLTYPE CABMenu::Next(_In_ ULONG celt,
                                     _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                     _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT {
    if (_subMenuIndex > 3) {
        return E_FAIL;
    }

    ULONG copied = 0;
    for (ULONG i = 0; i < celt; ++i) {
        if (_subMenuIndex > 3) {
            break;
        }

        switch (_subMenuIndex) {
        case 0:
            CFirstInfoCommand::CreateInstance(&pUICommand[i]);
            break;
        case 1:
            CSecondInfoCommand::CreateInstance(&pUICommand[i]);
            break;
        case 2:
            CSelectCommand::CreateInstance(&pUICommand[i]);
            break;
        case 3:
            CAppSelectionCommand::CreateInstance(&pUICommand[i]);
            break;
        }

        copied += 1;
        _subMenuIndex += 1;
    }

    if (pceltFetched != nullptr) {
        *pceltFetched = copied;
    }

    return S_OK;
}
