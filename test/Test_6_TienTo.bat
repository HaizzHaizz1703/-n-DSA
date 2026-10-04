@echo off
title Chuc nang 6 - Goi y tien to - Trie
:: Chay tu bat ky dau cung duoc: tu dong chuyen ve thu muc goc du an
cd /d "%~dp0.."
if not exist test\bin mkdir test\bin

echo ===================================================
echo [1/2] DANG BIEN DICH: Chuc nang 6 - Goi y tien to - Trie
echo ===================================================
g++ -std=c++14 test/test_tk_tientto.cpp src/core/HeThong.cpp src/core/dsa/TraCuu.cpp src/core/dsa/TKB.cpp src/core/dsa/DangKi.cpp src/core/dsa/Huy.cpp src/core/dsa/Undo.cpp src/core/dsa/TK_TienTo.cpp src/persistence/DataLoad.cpp -o test/bin/test_tk_tientto.exe
if %errorlevel% neq 0 (
    echo.
    echo [Loi] Bien dich that bai! Kiem tra lai ma nguon hoac cai dat g++.
    if /i not "%~1"=="nopause" pause
    exit /b 2
)

echo.
echo [2/2] DANG CHAY TEST...
echo.
test\bin\test_tk_tientto.exe
set KQ=%errorlevel%

echo.
if "%KQ%"=="0" (
    echo ==^> [OK] Chuc nang 6 - Goi y tien to - Trie: DAT
) else (
    echo ==^> [FAIL] Chuc nang 6 - Goi y tien to - Trie: CO TEST KHONG DAT
)
if /i not "%~1"=="nopause" pause
exit /b %KQ%
