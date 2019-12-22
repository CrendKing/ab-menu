#pragma once


struct Environment {
    static auto Initialize(HINSTANCE hInstance) -> bool;
    static auto QuotePath(PCWSTR path) -> const WCHAR *;

    static constexpr const WCHAR *COMPARER_ARGS_LEFT_PLACEHOLDER = L"%LEFT%";
    static constexpr const WCHAR *COMPARER_ARGS_RIGHT_PLACEHOLDER = L"%RIGHT%";

    static std::wstring modulePath;
    static std::wstring comparerPath;
    static std::wstring comparerArgs;
};
