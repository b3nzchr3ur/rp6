set LANG=C

cd Example_01_LEDs
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_02_UART_01
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_02_UART_02
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_03_Stopwatches
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..


cd Example_04_ACS
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_05_Move_01
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_05_Move_02
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_05_Move_03
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_05_Move_04_StateMachine
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_05_Move_04_StateMachine2
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..


cd Example_05_Move_04
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_05_Move_05
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_06_LightDetection
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_07_LightFollowing
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd Example_08_TV_REMOTE
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6Base_I2CMaster_01
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6Base_I2CMaster_02
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6Base_I2CMaster_03
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6Base_I2CMaster_04
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6Base_I2CSlave
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
make all
if errorlevel 1 goto end
make clean
if errorlevel 1 goto end
cd..

cd RP6Base_SELFTEST
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

