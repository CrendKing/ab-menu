#pragma once

#include "default.h"


class ATL_NO_VTABLE CInfoCommand
    : public ATL::CComObjectRoot
    , public ATL::CComCoClass<CInfoCommand>
    , public IDefaultExplorerCommand {
public:
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CInfoCommand)
        COM_INTERFACE_ENTRY(IExplorerCommand)
    END_COM_MAP()

private:
    auto STDMETHODCALLTYPE GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT override;
    auto STDMETHODCALLTYPE GetFlags(__RPC__out EXPCMDFLAGS *pFlags) -> HRESULT override;
};

class ATL_NO_VTABLE CSelectCommand
    : public ATL::CComObjectRoot
    , public ATL::CComCoClass<CSelectCommand>
    , public IDefaultExplorerCommand {
public:
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CSelectCommand)
        COM_INTERFACE_ENTRY(IExplorerCommand)
    END_COM_MAP()

private:
    auto STDMETHODCALLTYPE GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT override;
    auto STDMETHODCALLTYPE Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT override;
};

class ATL_NO_VTABLE CDiffCommand
    : public ATL::CComObjectRoot
    , public ATL::CComCoClass<CDiffCommand>
    , public IDefaultExplorerCommand {
public:
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CDiffCommand)
        COM_INTERFACE_ENTRY(IExplorerCommand)
    END_COM_MAP()

public:
    auto SetCmdIndex(int cmdIndex) -> void;

private:
    auto STDMETHODCALLTYPE GetTitle(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszName) -> HRESULT override;
    auto STDMETHODCALLTYPE GetIcon(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__deref_out_opt_string LPWSTR *ppszIcon) -> HRESULT override;
    auto STDMETHODCALLTYPE GetState(__RPC__in_opt IShellItemArray *psiItemArray, BOOL fOkToBeSlow, __RPC__out EXPCMDSTATE *pCmdState) -> HRESULT override;
    auto STDMETHODCALLTYPE Invoke(__RPC__in_opt IShellItemArray *psiItemArray, __RPC__in_opt IBindCtx *pbc) -> HRESULT override;

    int _cmdIndex;
};
