#include "pch.h"
#include "sub_cmd.h"
#include "environment.h"


#define CheckHr(expr) { hr = (expr); if (FAILED(hr)) { return hr; } }

CSubCommand::Item CSubCommand::_selectedItem;

auto CSubCommand::Item::ExtractItemAt(IShellItemArray *itemArray, int i) -> HRESULT {
    HRESULT hr;

    IShellItem *shellItem;
    CheckHr(itemArray->GetItemAt(i, &shellItem));

    LPWSTR displayName;
    CheckHr(shellItem->GetDisplayName(SIGDN_FILESYSPATH, &displayName));
    name = displayName;
    CoTaskMemFree(displayName);

    SFGAOF itemAttribs;
    CheckHr(shellItem->GetAttributes(SFGAO_FOLDER, &itemAttribs));
    isFolder = itemAttribs & SFGAO_FOLDER;

    return S_OK;
}

auto STDMETHODCALLTYPE CSubCommand::GetToolTip(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszInfotip) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CSubCommand::GetCanonicalName(__RPC__out GUID *pguidCommandName) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CSubCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_DEFAULT;
    return S_OK;
}

auto STDMETHODCALLTYPE CSubCommand::EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CInfoSubCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    std::wstring infoTitle;

    if (_selectedItem.name.empty()) {
        infoTitle = L"None selected";
    } else {
        infoTitle.append(_selectedItem.name).append(L" (");

        if (_selectedItem.isFolder) {
            infoTitle += L"Folder)";
        } else {
            infoTitle += L"File)";
        }
    }

    return SHStrDupW(infoTitle.c_str(), ppszName);
}

auto STDMETHODCALLTYPE CInfoSubCommand::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CInfoSubCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));

    *pCmdState = itemCount == 2 ? ECS_HIDDEN : ECS_DISABLED;

    return S_OK;
}

auto STDMETHODCALLTYPE CInfoSubCommand::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE CInfoSubCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_SEPARATORAFTER;
    return S_OK;
}

auto STDMETHODCALLTYPE CSelectSubCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"Select first item", ppszName);
}

auto STDMETHODCALLTYPE CSelectSubCommand::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return SHStrDupW((Environment::modulePath + L",0").c_str(), ppszIcon);
}

auto STDMETHODCALLTYPE CSelectSubCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));

    if (itemCount != 1) {
        *pCmdState = ECS_HIDDEN;
    } else if (_selectedItem.name.empty()) {
        *pCmdState = ECS_ENABLED;
    } else {
        Item currItem;
        CheckHr(currItem.ExtractItemAt(psiItemArray, 0));

        *pCmdState = currItem.name == _selectedItem.name ? ECS_HIDDEN : ECS_ENABLED;
    }

    return S_OK;
}

auto STDMETHODCALLTYPE CSelectSubCommand::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));

    if (itemCount != 1) {
        return E_UNEXPECTED;
    }

    CheckHr(_selectedItem.ExtractItemAt(psiItemArray, 0));

    return S_OK;
}

auto CDiffSubCommand::ExtractItems(IShellItemArray *psiItemArray, Item &firstItem, Item &secondItem) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));
    if (itemCount < 1 || itemCount > 2) {
        return S_FALSE;
    }

    CheckHr(secondItem.ExtractItemAt(psiItemArray, 0));

    if (itemCount == 1) {
        if (_selectedItem.name.empty()) {
            return S_FALSE;
        }

        firstItem = _selectedItem;
    } else {
        CheckHr(firstItem.ExtractItemAt(psiItemArray, 1));
    }

    return S_OK;
}

auto STDMETHODCALLTYPE CDiffSubCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"Compare", ppszName);
}

auto CDiffSubCommand::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return SHStrDupW((Environment::comparerPath + L",0").c_str(), ppszIcon);
}

auto STDMETHODCALLTYPE CDiffSubCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    Item firstItem, secondItem;
    CheckHr(ExtractItems(psiItemArray, firstItem, secondItem));

    *pCmdState = hr == S_OK && firstItem.name != secondItem.name && firstItem.isFolder == secondItem.isFolder ? ECS_ENABLED : ECS_HIDDEN;
    return S_OK;
}

auto ReplaceSubString(std::wstring &str, const WCHAR *toFind, const WCHAR *replaceWith) {
    const size_t replaceLen = wcslen(toFind);

    std::size_t pos;
    while ((pos = str.find(toFind)) != std::string::npos) {
        str.replace(pos, replaceLen, replaceWith);
    }
}

auto STDMETHODCALLTYPE CDiffSubCommand::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    HRESULT hr;

    Item firstItem, secondItem;
    CheckHr(ExtractItems(psiItemArray, firstItem, secondItem));

    if (hr != S_OK) {
        return E_UNEXPECTED;
    }

    std::wstring cmdline = Environment::comparerArgs;
    ReplaceSubString(cmdline, Environment::COMPARER_ARGS_LEFT_PLACEHOLDER, Environment::QuotePath(firstItem.name.c_str()));
    ReplaceSubString(cmdline, Environment::COMPARER_ARGS_RIGHT_PLACEHOLDER, Environment::QuotePath(secondItem.name.c_str()));

    cmdline.insert(0, L" ");
    cmdline.insert(0, Environment::comparerPath);

    STARTUPINFOW si = { .cb = sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessW(nullptr, cmdline.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        _selectedItem.name.clear();
        return S_OK;
    }

    return E_FAIL;
}
