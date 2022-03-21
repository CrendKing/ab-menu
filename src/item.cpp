#include "item.h"

#include "environment.h"


ABItem::ABItem() {
    name.reserve(MAX_PATH);
}

ABItem::ABItem(IShellItemArray *itemArray, int i) {
    ExtractShellItemAt(itemArray, i);
}

auto ABItem::ExtractShellItemAt(IShellItemArray *itemArray, int i) -> HRESULT {
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
