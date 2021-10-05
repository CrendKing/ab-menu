cd /d "%~dp0"

del CompareMenu.zip
7z a CompareMenu.zip README.md LICENSE install.bat uninstall.bat config.reg

cd x64\Release\
7z a ..\..\CompareMenu.zip compare_menu_64.dll

cd ..\..\Win32\Release\
7z a ..\..\CompareMenu.zip compare_menu_32.dll
