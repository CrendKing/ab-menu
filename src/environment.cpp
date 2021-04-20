#include "pch.h"
#include "environment.h"


static constexpr const WCHAR *SELECTED_ITEM_OBJECT_NAME = L"Global\\CompareMenuSelectedItem";

static std::array<WCHAR, MAX_PATH> g_quotedPathBuffer {};

std::wstring Environment::modulePath;
std::wstring Environment::comparerPath;
std::wstring Environment::comparerArgs;
Item Environment::selectedItem;
HANDLE Environment::_hMapFile = nullptr;
LPWSTR Environment::_mappingBuffer = nullptr;

auto Environment::Initialize(HINSTANCE hInstance) -> bool {
    modulePath.resize(MAX_PATH);
    if (GetModuleFileNameW(hInstance, modulePath.data(), static_cast<DWORD>(modulePath.size())) == 0) {
        return false;
    }

    _hMapFile = OpenFileMappingW(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, SELECTED_ITEM_OBJECT_NAME);
    if (_hMapFile == nullptr) {
        _hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, MAX_PATH * sizeof(WCHAR), SELECTED_ITEM_OBJECT_NAME);
        if (_hMapFile == nullptr) {
            return false;
        }
    }

    _mappingBuffer = static_cast<LPWSTR>(MapViewOfFile(_hMapFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, MAX_PATH * sizeof(WCHAR)));
    if (_mappingBuffer == nullptr) {
        return false;
    }

    return true;
}

auto Environment::Destroy() -> void {
    if (_mappingBuffer != nullptr) {
        UnmapViewOfFile(_mappingBuffer);
    }
    if (_hMapFile != nullptr) {
        CloseHandle(_hMapFile);
    }
}

auto Environment::QuotePath(PCWSTR path) -> const WCHAR * {
    if (path == nullptr) {
        return path;
    }

    wcscpy_s(g_quotedPathBuffer.data(), g_quotedPathBuffer.size(), path);
    PathQuoteSpacesW(g_quotedPathBuffer.data());
    return g_quotedPathBuffer.data();
}

auto Environment::LoadSelectedItem() -> void {
    if (_mappingBuffer != nullptr) {
        selectedItem.name = _mappingBuffer;
        selectedItem.isFolder = PathIsDirectoryW(_mappingBuffer);
    }
}

auto Environment::FlushSelectedItem() -> void {
    if (selectedItem.name.empty()) {
        CopyMemory(_mappingBuffer, L"\0", sizeof(WCHAR));
    } else {
        CopyMemory(_mappingBuffer, selectedItem.name.c_str(), selectedItem.name.size() * sizeof(WCHAR));
    }
}
