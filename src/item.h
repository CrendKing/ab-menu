#pragma once

#include "pch.h"


struct Item {
    Item();

    auto ExtractItemAt(IShellItemArray *itemArray, int i) -> HRESULT;

    std::wstring name;
    bool isFolder = false;
};
