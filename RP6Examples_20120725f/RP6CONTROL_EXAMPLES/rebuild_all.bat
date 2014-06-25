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

cd Example_03_Microphone
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_04_ExternalEEPROM
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..


cd Example_05_IO_ADC
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

cd RP6Control_SELFTEST
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
