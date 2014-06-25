set LANG=C

cd Example_01_LCD
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_02_Buttons
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_03_ADC_IO1
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_04_SDCARD
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..


cd Example_05_WLAN_CMD
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_06_I2CMaster
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_07_I2CMaster
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_08_I2CMaster
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_09_Move
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_10_Move2
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..


cd Example_11_WIFI_REMOTE_1
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_12_WIFI_REMOTE_2
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_13_Simple_Webserver
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_14_SDCARD_logging
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6M256_SELFTEST
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

goto :EOF
:end
echo AN ERROR OCCURED - EXIT.
:EOF
echo DONE.

