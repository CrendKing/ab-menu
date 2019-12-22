#include "pch.h"
#include "environment.h"


static std::array<WCHAR, MAX_PATH> g_quotedPathBuffer {};

std::wstring Environment::modulePath;
std::wstring Environment::comparerPath;
std::wstring Environment::comparerArgs;

auto Environment::Initialize(HINSTANCE hInstance) -> bool {
    modulePath.resize(MAX_PATH);
    if (GetModuleFileNameW(hInstance, modulePath.data(), static_cast<DWORD>(modulePath.size())) == 0) {
        return false;
    }

    return true;
}

auto Environment::QuotePath(PCWSTR path) -> const WCHAR * {
    if (path == nullptr) {
        return path;
    }

    wcscpy_s(g_quotedPathBuffer.data(), g_quotedPathBuffer.size(), path);
    PathQuoteSpacesW(g_quotedPathBuffer.data());
    return g_quotedPathBuffer.data();
}
