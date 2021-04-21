#pragma once

#include "item.h"


struct Environment {
    static inline std::unique_ptr<Environment> INSTANCE;

    Environment();
    ~Environment();

    auto Initialize(HINSTANCE hInstance) -> bool;
    auto QuotePath(PCWSTR path) const -> const WCHAR *;
    auto LoadSelectedItem() -> void;
    auto FlushSelectedItem() const -> void;

    std::wstring modulePath;
    std::wstring comparerPath;
    std::wstring comparerArgs;

    Item selectedItem;

private:
    HANDLE _hMapFile = nullptr;
    LPWSTR _mappingBuffer = nullptr;
};
