#pragma once

#include "item.h"


struct Environment {
    struct Comparer {
        std::wstring name;
        std::wstring cmdline;
        std::wstring iconPath;
    };

    static auto ExtractItems(IShellItemArray *psiItemArray, Item &firstItem, Item &secondItem) -> HRESULT;

    Environment();
    ~Environment();

    auto Initialize(HINSTANCE hInstance) -> bool;
    auto LoadSelectedItem() -> void;
    auto FlushSelectedItem() const -> void;

    static inline Environment *INSTANCE = nullptr;

    std::wstring menuIconPath;
    std::vector<Comparer> comparers;

    Item selectedItem;

private:
    HANDLE _hMapFile = nullptr;
    LPWSTR _mappingBuffer = nullptr;
};

#define CheckHr(expr) { hr = (expr); if (FAILED(hr)) { return hr; } }
