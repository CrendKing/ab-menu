#pragma once

#include <codeanalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)

#include <algorithm>
#include <array>
#include <format>
#include <mutex>
#include <string>
#include <vector>

#include "min_windows.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <shellapi.h>
#include <shlobj.h>

#pragma warning(pop)

#include "resource.h"

#pragma warning(push)
#pragma warning(disable: 26812)
