@echo off
title He Thong Dang Ky Hoc Phan - Console
echo ===================================================
echo [1/2] DANG BIEN DICH CHUONG TRINH...
echo ===================================================

g++ src/main.cpp src/core/HeThong.cpp src/core/dsa/TraCuu.cpp src/core/dsa/TKB.cpp src/core/dsa/DangKi.cpp src/core/dsa/Huy.cpp src/core/dsa/Undo.cpp src/core/dsa/TK_TienTo.cpp src/persistence/DataLoad.cpp src/presentation/CLI.cpp -o main.exe

if %errorlevel% neq 0 (
    echo.
    echo [Loi] Bien dich that bai!
    pause
    exit /b %errorlevel%
)

echo.
echo [2/2] BIEN DICH THANH CONG! DANG KHOI CHAY...
echo ===================================================
echo.

main.exe
pause