@echo off
title Benchmark
color 0F

echo [1/2] Dang bien dich...
g++ benchmark/benchmark.cpp src/core/HeThong.cpp src/core/dsa/TraCuu.cpp src/core/dsa/TKB.cpp src/core/dsa/DangKi.cpp src/core/dsa/Huy.cpp src/core/dsa/Undo.cpp src/core/dsa/TK_TienTo.cpp src/persistence/DataLoad.cpp -o run_benchmark.exe

if %errorlevel% neq 0 (
    color 0C
    echo [Loi] Bien dich that bai!
    pause
    exit /b %errorlevel%
)

echo [2/2] Khoi chay he thong...
echo.
run_benchmark.exe

echo.
pause