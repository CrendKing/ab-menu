#pragma once

#include "default.h"


class ATL_NO_VTABLE __declspec(uuid("59D49316-D3FD-425B-A175-41DEB301AB1F")) CABMenu
    : public ATL::CComObjectRoot
    , public ATL::CComCoClass<CABMenu, &__uuidof(CABMenu)>
    , public IInitializeCommand
    , public IDefaultExplorerCommand
    , public IDefaultEnumExplorerCommand {
public:
    DECLARE_REGISTRY_RESOURCEID(IDS_AB_MENU)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CABMenu)
        COM_INTERFACE_ENTRY(IInitializeCommand)
        COM_INTERFACE_ENTRY(IExplorerCommand)
        COM_INTERFACE_ENTRY(IEnumExplorerCommand)
    END_COM_MAP()

private:
    // IInitializeCommand
    auto STDMETHODCALLTYPE Initialize(__RPC__in_string LPCWSTR pszCommandName, __RPC__in_opt IPropertyBag *ppb) -> HRESULT override;

    // IExplorerCommand
    auto STDMETHODCALLTYPE GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT override;
    auto STDMETHODCALLTYPE GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT override;
    auto STDMETHODCALLTYPE GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT override;
    auto STDMETHODCALLTYPE EnumSubCommands(__RPC__deref_out_opt IEnumExplorerCommand **ppEnum) -> HRESULT override;

    // IEnumExplorerCommand
    auto STDMETHODCALLTYPE Next(_In_ ULONG celt,
                                _Out_writes_to_(celt, *pceltFetched) IExplorerCommand **pUICommand,
                                _Out_opt_ _Deref_out_range_(0, celt) ULONG *pceltFetched) -> HRESULT override;
};
