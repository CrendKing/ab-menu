#pragma once

#include "item.h"


struct Environment {
    static auto Initialize(HINSTANCE hInstance) -> bool;
    static auto Destroy() -> void;

    static auto QuotePath(PCWSTR path) -> const WCHAR *;
    static auto LoadSelectedItem() -> void;
    static auto FlushSelectedItem() -> void;

    static constexpr const WCHAR *COMPARER_ARGS_LEFT_PLACEHOLDER = L"%LEFT%";
    static constexpr const WCHAR *COMPARER_ARGS_RIGHT_PLACEHOLDER = L"%RIGHT%";

    static std::wstring modulePath;
    static std::wstring comparerPath;
    static std::wstring comparerArgs;

    static Item selectedItem;

private:
    static HANDLE _hMapFile;
    static LPWSTR _mappingBuffer;
};
