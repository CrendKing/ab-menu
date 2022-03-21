#pragma once


struct ABItem {
    ABItem();
    ABItem(IShellItemArray *itemArray, int i);

    auto ExtractShellItemAt(IShellItemArray *itemArray, int i) -> HRESULT;

    std::wstring name;
    bool isFolder = false;
};
