#include "pch.h"
#include "item.h"


#define CheckHr(expr) { hr = (expr); if (FAILED(hr)) { return hr; } }

Item::Item() {
    name.resize(MAX_PATH);
}

auto Item::ExtractItemAt(IShellItemArray *itemArray, int i) -> HRESULT {
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
