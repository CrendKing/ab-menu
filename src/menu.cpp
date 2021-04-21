#include "pch.h"
#include "menu.h"
#include "environment.h"
#include "sub_cmd.h"


static constexpr const WCHAR *REGISTRY_COMPARER_PATH_VALUE_NAME = L"ComparerPath";
static constexpr const WCHAR *REGISTRY_COMPARER_ARGS_VALUE_NAME = L"ComparerArgs";

OBJECT_ENTRY_AUTO(__uuidof(CCompareMenu), CCompareMenu)

auto STDMETHODCALLTYPE CCompareMenu::Initialize(__RPC__in_string LPCWSTR pszCommandName, __RPC__in_opt IPropertyBag *ppb) -> HRESULT {
    ATL::CComVariant variant;

    if (SUCCEEDED(ppb->Read(REGISTRY_COMPARER_PATH_VALUE_NAME, &variant, nullptr))) {
        Environment::INSTANCE->comparerPath = variant.bstrVal;
    } else {
        Environment::INSTANCE->comparerPath.clear();
    }

    if (SUCCEEDED(ppb->Read(REGISTRY_COMPARER_ARGS_VALUE_NAME, &variant, nullptr))) {
        Environment::INSTANCE->comparerArgs = variant.bstrVal;
    } else {
        Environment::INSTANCE->comparerArgs.clear();
    }

    Environment::INSTANCE->LoadSelectedItem();

    return S_OK;
}

auto STDMETHODCALLTYPE CCompareMenu::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"Compare Two", ppszName);
}

auto STDMETHODCALLTYPE CCompareMenu::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return SHStrDupW((Environment::INSTANCE->modulePath + L",0").c_str(), ppszIcon);
}

auto STDMETHODCALLTYPE CCompareMenu::GetToolTip(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszInfotip) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CCompareMenu::GetCanonicalName(__RPC__out GUID *pguidCommandName) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CCompareMenu::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    *pCmdState = Environment::INSTANCE->comparerPath.empty() || Environment::INSTANCE->comparerArgs.empty() ? ECS_DISABLED : ECS_ENABLED;
    return S_OK;
}

auto STDMETHODCALLTYPE CCompareMenu::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CCompareMenu::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_HASSUBCOMMANDS;
    return S_OK;
}

auto STDMETHODCALLTYPE CCompareMenu::EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT {
    AddRef();
    _subCmdIndex = 0;
    *ppEnum = this;
    return S_OK;
}

auto STDMETHODCALLTYPE CCompareMenu::Next(_In_ ULONG celt,
                                          _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                          _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT {
    if (_subCmdIndex > 2) {
        return E_FAIL;
    }

    ULONG copied = 0;
    for (ULONG i = 0; i < celt; ++i) {
        if (_subCmdIndex > 2) {
            break;
        }

        switch (_subCmdIndex) {
        case 0:
            CInfoSubCommand::CreateInstance(&pUICommand[i]);
            break;
        case 1:
            CSelectSubCommand::CreateInstance(&pUICommand[i]);
            break;
        case 2:
            CDiffSubCommand::CreateInstance(&pUICommand[i]);
            break;
        }

        copied += 1;
        _subCmdIndex += 1;
    }

    if (pceltFetched != nullptr) {
        *pceltFetched = copied;
    }

    return S_OK;
}

auto STDMETHODCALLTYPE CCompareMenu::Skip(ULONG celt) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CCompareMenu::Reset() -> HRESULT {
    _subCmdIndex = 0;
    return S_OK;
}

auto STDMETHODCALLTYPE CCompareMenu::Clone(__RPC__deref_out_opt IEnumExplorerCommand **ppenum) -> HRESULT {
    return E_NOTIMPL;
}
