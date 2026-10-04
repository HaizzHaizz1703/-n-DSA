@echo off
title He Thong Dang Ky Hoc Phan - Web Server
echo ===================================================
echo [1/2] DANG BIEN DICH WEB SERVER C++...
echo ===================================================

:: Tự động gom các file source và biên dịch ra web_server.exe kèm thư viện mạng Winsock (-lws2_32)
g++ src/presentation/WebServer.cpp src/core/HeThong.cpp src/core/dsa/TraCuu.cpp src/core/dsa/TKB.cpp src/core/dsa/DangKi.cpp src/core/dsa/Huy.cpp src/core/dsa/Undo.cpp src/core/dsa/TK_TienTo.cpp src/persistence/DataLoad.cpp -o web_server.exe -lws2_32

if %errorlevel% neq 0 (
    echo.
    echo [Loi] Bien dich that bai! Vui long dam bao ban da dat file "httplib.h" trong thu muc src/presentation/
    pause
    exit /b %errorlevel%
)

echo.
echo [2/2] BIEN DICH THANH CONG! DANG KHOI CHAY SERVER...
echo ===================================================
echo.

web_server.exe
pause