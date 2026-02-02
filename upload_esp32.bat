@echo off
REM Manual ESP32 upload helper - run this, THEN hold BOOT and tap RESET on the board
REM Edit COM port and speed below if needed (COM6, 57600)

set PORT=COM6
set SPEED=57600
set BIN=.pio\build\sender\firmware.bin
set ADDR=0x10000

echo.
echo ========================================
echo  ESP32 manual upload
echo  Port: %PORT%  Speed: %SPEED%
echo ========================================
echo.
echo 1. Close Serial Monitor if open.
echo 2. Click Upload in PlatformIO, OR run this script.
echo 3. When you see "Connecting...":
echo    - HOLD the BOOT (IO0) button on the ESP32
echo    - PRESS and RELEASE the RESET (EN) button
echo    - RELEASE the BOOT button after 1 second
echo.
pause

"%USERPROFILE%\.platformio\penv\Scripts\platformio.exe" run -e sender -t upload --upload-port %PORT%

echo.
echo If it failed: try changing PORT to your COM port (e.g. COM3) in this script.
echo Or in platformio.ini add: upload_speed = 9600
pause
