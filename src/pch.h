#pragma once

#include <codeanalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)

#include <array>
#include <mutex>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS    // some CString constructors will be explicit

#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <shlobj.h>

#pragma warning(pop)

#include "resource.h"

#pragma warning(push)
#pragma warning(disable: 26812)
