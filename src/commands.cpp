#include "commands.h"

#include "environment.h"


static constexpr const WCHAR *ARGS_FIRST_PLACEHOLDER = L"%FIRST%";
static constexpr const WCHAR *ARGS_SECOND_PLACEHOLDER = L"%SECOND%";

static auto ConstructInfoTitle(const std::wstring &prefix, const ABItem &item) -> std::wstring {
    std::wstring ret;
    ret.append(prefix)
        .append(item.name)
        .append(item.isFolder ? L" (Folder)" : L" (File)");
    return ret;
}

auto STDMETHODCALLTYPE CFirstInfoCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    const ABItem firstItem(psiItemArray, 1);
    return SHStrDupW(ConstructInfoTitle(L"First: ", firstItem).c_str(), ppszName);
}

auto STDMETHODCALLTYPE CFirstInfoCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;
    DWORD selectedItemCount;
    CheckHr(psiItemArray->GetCount(&selectedItemCount));

    *pCmdState = selectedItemCount == 2 ? ECS_DISABLED : ECS_HIDDEN;

    return S_OK;
}

auto STDMETHODCALLTYPE CFirstInfoCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_DEFAULT;
    return S_OK;
}

auto STDMETHODCALLTYPE CSecondInfoCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    std::wstring infoTitle;

    HRESULT hr;
    DWORD selectedItemCount;
    CheckHr(psiItemArray->GetCount(&selectedItemCount));

    if (selectedItemCount == 2) {
        const ABItem secondItem(psiItemArray, 0);
        infoTitle = ConstructInfoTitle(L"Second: ", secondItem);
    } else if (Environment::INSTANCE->selectedItem.name.empty()) {
        infoTitle = L"None selected";
    } else {
        infoTitle = ConstructInfoTitle(L"First: ", Environment::INSTANCE->selectedItem);
    }

    return SHStrDupW(infoTitle.c_str(), ppszName);
}

auto STDMETHODCALLTYPE CSecondInfoCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    *pCmdState = ECS_DISABLED;
    return S_OK;
}

auto STDMETHODCALLTYPE CSecondInfoCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
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
        const ABItem currItem(psiItemArray, 0);
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

    CheckHr(Environment::INSTANCE->selectedItem.ExtractShellItemAt(psiItemArray, 0));
    Environment::INSTANCE->FlushSelectedItem();

    return S_OK;
}

auto CDiffCommand::SetCmdIndex(int cmdIndex) -> void {
    _cmdIndex = cmdIndex;
}

auto STDMETHODCALLTYPE CDiffCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(Environment::INSTANCE->apps[_cmdIndex].name.c_str(), ppszName);
}

auto CDiffCommand::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return SHStrDupW(Environment::INSTANCE->apps[_cmdIndex].iconPath.c_str(), ppszIcon);
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

    ABItem firstItem, secondItem;
    CheckHr(Environment::ExtractItems(psiItemArray, firstItem, secondItem));

    if (hr != S_OK) {
        return E_UNEXPECTED;
    }

    std::wstring cmdline = Environment::INSTANCE->apps[_cmdIndex].cmdline;
    ReplaceSubString(cmdline, ARGS_FIRST_PLACEHOLDER, firstItem.name.c_str());
    ReplaceSubString(cmdline, ARGS_SECOND_PLACEHOLDER, secondItem.name.c_str());

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

auto STDMETHODCALLTYPE CAppSelectionCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"Execute", ppszName);
}

auto STDMETHODCALLTYPE CAppSelectionCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    ABItem firstItem, secondItem;
    CheckHr(Environment::ExtractItems(psiItemArray, firstItem, secondItem));

    *pCmdState = hr == S_OK && firstItem.name != secondItem.name && firstItem.isFolder == secondItem.isFolder ? ECS_ENABLED : ECS_HIDDEN;
    return S_OK;
}

auto STDMETHODCALLTYPE CAppSelectionCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_HASSUBCOMMANDS;
    return S_OK;
}

auto STDMETHODCALLTYPE CAppSelectionCommand::EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT {
    AddRef();
    _subMenuIndex = 0;
    *ppEnum = this;
    return S_OK;
}

auto STDMETHODCALLTYPE CAppSelectionCommand::Next(_In_ ULONG celt, _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand, _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched)
    -> HRESULT {
    if (_subMenuIndex >= Environment::INSTANCE->apps.size()) {
        return E_FAIL;
    }

    ULONG copied = 0;
    for (ULONG i = 0; i < celt; ++i) {
        if (_subMenuIndex >= Environment::INSTANCE->apps.size()) {
            break;
        }

        CDiffCommand::CreateInstance(&pUICommand[i]);
        reinterpret_cast<CDiffCommand *>(pUICommand[i])->SetCmdIndex(_subMenuIndex);

        copied += 1;
        _subMenuIndex += 1;
    }

    if (pceltFetched != nullptr) {
        *pceltFetched = copied;
    }

    return S_OK;
}
