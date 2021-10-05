#pragma once

#include "default.h"


class ATL_NO_VTABLE CComparerCommand
    : public ATL::CComObjectRoot
    , public ATL::CComCoClass<CComparerCommand>
    , public IDefaultExplorerCommand
    , public IDefaultEnumExplorerCommand {
public:
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CComparerCommand)
        COM_INTERFACE_ENTRY(IExplorerCommand)
        COM_INTERFACE_ENTRY(IEnumExplorerCommand)
    END_COM_MAP()

private:
    // IExplorerCommand
    auto STDMETHODCALLTYPE GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT override;
    auto STDMETHODCALLTYPE GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT override;
    auto STDMETHODCALLTYPE EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT override;

    // IEnumExplorerCommand
    auto STDMETHODCALLTYPE Next(_In_ ULONG celt,
                                _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT override;
};
