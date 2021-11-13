#include "terminal_cmd.h"

#include "environment.h"


static constexpr const WCHAR *COMPARER_ARGS_LEFT_PLACEHOLDER = L"%LEFT%";
static constexpr const WCHAR *COMPARER_ARGS_RIGHT_PLACEHOLDER = L"%RIGHT%";

auto STDMETHODCALLTYPE CInfoCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    std::wstring infoTitle;

    if (Environment::INSTANCE->selectedItem.name.empty()) {
        infoTitle = L"None selected";
    } else {
        infoTitle.append(Environment::INSTANCE->selectedItem.name).append(L" (");

        if (Environment::INSTANCE->selectedItem.isFolder) {
            infoTitle += L"Folder)";
        } else {
            infoTitle += L"File)";
        }
    }

    return SHStrDupW(infoTitle.c_str(), ppszName);
}

auto STDMETHODCALLTYPE CInfoCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));

    *pCmdState = itemCount == 2 ? ECS_HIDDEN : ECS_DISABLED;

    return S_OK;
}

auto STDMETHODCALLTYPE CInfoCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_SEPARATORAFTER;
    return S_OK;
}

auto STDMETHODCALLTYPE CSelectCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"Select first item", ppszName);
}

auto STDMETHODCALLTYPE CSelectCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));

    if (itemCount != 1) {
        *pCmdState = ECS_HIDDEN;
    } else if (Environment::INSTANCE->selectedItem.name.empty()) {
        *pCmdState = ECS_ENABLED;
    } else {
        Item currItem;
        CheckHr(currItem.ExtractItemAt(psiItemArray, 0));

        *pCmdState = currItem.name == Environment::INSTANCE->selectedItem.name ? ECS_HIDDEN : ECS_ENABLED;
    }

    return S_OK;
}

auto STDMETHODCALLTYPE CSelectCommand::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));

    if (itemCount != 1) {
        return E_UNEXPECTED;
    }

    CheckHr(Environment::INSTANCE->selectedItem.ExtractItemAt(psiItemArray, 0));
    Environment::INSTANCE->FlushSelectedItem();

    return S_OK;
}

auto CDiffCommand::SetCmdIndex(int cmdIndex) -> void {
    _cmdIndex = cmdIndex;
}

auto STDMETHODCALLTYPE CDiffCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(Environment::INSTANCE->comparers[_cmdIndex].name.c_str(), ppszName);
}

auto CDiffCommand::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return SHStrDupW(Environment::INSTANCE->comparers[_cmdIndex].iconPath.c_str(), ppszIcon);
}

auto STDMETHODCALLTYPE CDiffCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    *pCmdState = ECS_ENABLED;
    return S_OK;
}

static auto ReplaceSubString(std::wstring &str, const WCHAR *toFind, const WCHAR *replaceWith) {
    const size_t replaceLen = std::char_traits<WCHAR>::length(toFind);

    std::size_t pos;
    while ((pos = str.find(toFind)) != std::string::npos) {
        str.replace(pos, replaceLen, replaceWith);
    }
}

auto STDMETHODCALLTYPE CDiffCommand::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    HRESULT hr;

    Item firstItem, secondItem;
    CheckHr(Environment::ExtractItems(psiItemArray, firstItem, secondItem));

    if (hr != S_OK) {
        return E_UNEXPECTED;
    }

    std::wstring cmdline = Environment::INSTANCE->comparers[_cmdIndex].cmdline;
    ReplaceSubString(cmdline, COMPARER_ARGS_LEFT_PLACEHOLDER, firstItem.name.c_str());
    ReplaceSubString(cmdline, COMPARER_ARGS_RIGHT_PLACEHOLDER, secondItem.name.c_str());

    STARTUPINFOW si { .cb = sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessW(nullptr, cmdline.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        return E_FAIL;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    Environment::INSTANCE->selectedItem.name.clear();
    Environment::INSTANCE->FlushSelectedItem();

    return S_OK;
}
