@echo off
setlocal

REM --- Ayarlar ---
set QT_BIN=C:\Qt\6.9.2\mingw_64\bin
set IFW_BIN=C:\Qt\Tools\QtInstallerFramework\4.10\bin

set BUILD_DIR=D:\Qt_Projeler\encoderSimGUI\software\build\Desktop_Qt_6_9_2_MinGW_64_bit-Release\release
set INSTALLER_DIR=D:\Qt_Projeler\encoderSimGUI\installer

REM --- 1) windeployqt ---
"%QT_BIN%\windeployqt.exe" "%BUILD_DIR%\encoderSimGUI.exe"

REM --- 2) installer olustur ---
"%IFW_BIN%\binarycreator.exe" ^
 -c "%INSTALLER_DIR%\config\config.xml" ^
 -p "%INSTALLER_DIR%\packages" ^
 "%INSTALLER_DIR%\EncoderSimGUI_Setup.exe"

echo installer hazir
pause
endlocal