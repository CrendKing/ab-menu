#pragma once

#include "item.h"


struct Environment {
    struct App {
        std::wstring name;
        std::wstring cmdline;
        std::wstring iconPath;

        auto operator<(const App &other) -> bool { return name < other.name; }
    };

    static auto ExtractItems(IShellItemArray *psiItemArray, ABItem &firstItem, ABItem &secondItem) -> HRESULT;

    Environment();
    ~Environment();

    auto Initialize(HINSTANCE hInstance) -> bool;
    auto LoadSelectedItem() -> void;
    auto FlushSelectedItem() const -> void;

    static inline Environment *INSTANCE = nullptr;

    std::wstring menuIconPath;
    std::vector<App> apps;

    ABItem selectedItem;

private:
    HANDLE _hMapFile = nullptr;
    LPWSTR _mappingBuffer = nullptr;
};

#define CheckHr(expr)     \
    {                     \
        hr = (expr);      \
        if (FAILED(hr)) { \
            return hr;    \
        }                 \
    }
