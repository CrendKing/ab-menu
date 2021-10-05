#pragma once


class ATL_NO_VTABLE IDefaultExplorerCommand : public IExplorerCommand {
private:
    auto STDMETHODCALLTYPE GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT override;
    auto STDMETHODCALLTYPE GetToolTip(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszInfotip) -> HRESULT override;
    auto STDMETHODCALLTYPE GetCanonicalName(__RPC__out GUID *pguidCommandName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT override;
    auto STDMETHODCALLTYPE Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT override;
    auto STDMETHODCALLTYPE GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT override;
    auto STDMETHODCALLTYPE EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT override;
};

class ATL_NO_VTABLE IDefaultEnumExplorerCommand : public IEnumExplorerCommand {
protected:
    ULONG _subMenuIndex;

private:
    auto STDMETHODCALLTYPE Next(_In_ ULONG celt,
                                _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT override;
    auto STDMETHODCALLTYPE Skip(ULONG celt) -> HRESULT override;
    auto STDMETHODCALLTYPE Reset() -> HRESULT override;
    auto STDMETHODCALLTYPE Clone(__RPC__deref_out_opt IEnumExplorerCommand **ppenum) -> HRESULT override;
};
