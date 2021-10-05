#include "pch.h"
#include "comparer_cmd.h"
#include "environment.h"
#include "terminal_cmd.h"


auto STDMETHODCALLTYPE CComparerCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return SHStrDupW(L"Compare", ppszName);
}

auto STDMETHODCALLTYPE CComparerCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    HRESULT hr;

    Item firstItem, secondItem;
    CheckHr(Environment::ExtractItems(psiItemArray, firstItem, secondItem));

    *pCmdState = hr == S_OK && firstItem.name != secondItem.name && firstItem.isFolder == secondItem.isFolder ? ECS_ENABLED : ECS_HIDDEN;
    return S_OK;
}

auto STDMETHODCALLTYPE CComparerCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_HASSUBCOMMANDS;
    return S_OK;
}

auto STDMETHODCALLTYPE CComparerCommand::EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT {
    AddRef();
    _subMenuIndex = 0;
    *ppEnum = this;
    return S_OK;
}

auto STDMETHODCALLTYPE CComparerCommand::Next(_In_ ULONG celt,
                                                   _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                                   _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT {
    if (_subMenuIndex >= Environment::INSTANCE->comparers.size()) {
        return E_FAIL;
    }

    ULONG copied = 0;
    for (ULONG i = 0; i < celt; ++i) {
        if (_subMenuIndex >= Environment::INSTANCE->comparers.size()) {
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
