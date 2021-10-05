#include "pch.h"
#include "environment.h"


static constexpr const WCHAR *SELECTED_ITEM_OBJECT_NAME = L"Global\\CompareMenuSelectedItem";

auto Environment::ExtractItems(IShellItemArray *psiItemArray, Item &firstItem, Item &secondItem) -> HRESULT {
    HRESULT hr;

    DWORD itemCount;
    CheckHr(psiItemArray->GetCount(&itemCount));
    if (itemCount < 1 || itemCount > 2) {
        return S_FALSE;
    }

    CheckHr(secondItem.ExtractItemAt(psiItemArray, 0));

    if (itemCount == 1) {
        if (Environment::INSTANCE->selectedItem.name.empty()) {
            return S_FALSE;
        }

        firstItem = Environment::INSTANCE->selectedItem;
    } else {
        CheckHr(firstItem.ExtractItemAt(psiItemArray, 1));
    }

    return S_OK;
}

Environment::Environment() {
    menuIconPath.resize(MAX_PATH);
}

Environment::~Environment() {
    if (_mappingBuffer != nullptr) {
        UnmapViewOfFile(_mappingBuffer);
    }
    if (_hMapFile != nullptr) {
        CloseHandle(_hMapFile);
    }
}

auto Environment::Initialize(HINSTANCE hInstance) -> bool {
    if (GetModuleFileNameW(hInstance, menuIconPath.data(), static_cast<DWORD>(menuIconPath.size())) == 0) {
        return false;
    }
    menuIconPath.append(L",0");

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

auto Environment::LoadSelectedItem() -> void {
    if (_mappingBuffer != nullptr) {
        selectedItem.name = _mappingBuffer;
        selectedItem.isFolder = PathIsDirectoryW(_mappingBuffer) == FILE_ATTRIBUTE_DIRECTORY;
    }
}

auto Environment::FlushSelectedItem() const -> void {
    // include the tailing L'\0'
    CopyMemory(_mappingBuffer, selectedItem.name.c_str(), (selectedItem.name.size() + 1) * sizeof(WCHAR));
}
