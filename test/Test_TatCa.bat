@echo off
setlocal enabledelayedexpansion
title Chay tat ca test - He Thong Dang Ky Hoc Phan
cd /d "%~dp0.."

set LOI=0
set DS=Test_1_TraCuu Test_2_TKB Test_3_DangKi Test_4_Huy Test_5_Undo Test_6_TienTo

for %%T in (%DS%) do (
    call "%~dp0%%T.bat" nopause
    set KQ_%%T=!errorlevel!
    echo.
    echo ###################################################
    echo.
)

echo ===================================================
echo             TONG KET 6 CHUC NANG
echo ===================================================
for %%T in (%DS%) do (
    if "!KQ_%%T!"=="0" (
        echo   [ OK   ] %%T
    ) else if "!KQ_%%T!"=="2" (
        echo   [ LOI BIEN DICH ] %%T
        set /a LOI+=1
    ) else (
        echo   [ FAIL ] %%T
        set /a LOI+=1
    )
)
echo ===================================================
if "!LOI!"=="0" (
    echo   TAT CA 6 CHUC NANG DEU DAT
) else (
    echo   CO !LOI! CHUC NANG CHUA DAT - xem chi tiet [FAIL] o phia tren
)
echo ===================================================
pause
exit /b !LOI!
