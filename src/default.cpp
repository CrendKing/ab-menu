#include "default.h"


auto STDMETHODCALLTYPE IDefaultExplorerCommand::GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::GetToolTip(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszInfotip) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::GetCanonicalName(__RPC__out GUID *pguidCommandName) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT {
    *pFlags = ECF_DEFAULT;
    return S_OK;
}

auto STDMETHODCALLTYPE IDefaultExplorerCommand::EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultEnumExplorerCommand::Next(_In_ ULONG celt,
                                                         _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                                         _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultEnumExplorerCommand::Skip(ULONG celt) -> HRESULT {
    return E_NOTIMPL;
}

auto STDMETHODCALLTYPE IDefaultEnumExplorerCommand::Reset() -> HRESULT {
    _subMenuIndex = 0;
    return S_OK;
}

auto STDMETHODCALLTYPE IDefaultEnumExplorerCommand::Clone(__RPC__deref_out_opt IEnumExplorerCommand **ppenum) -> HRESULT {
    return E_NOTIMPL;
}
