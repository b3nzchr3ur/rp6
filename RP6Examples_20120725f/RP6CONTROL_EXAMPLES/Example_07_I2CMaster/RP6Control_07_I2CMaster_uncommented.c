// Uncommented Version of RP6Control_07_I2CMaster.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6ControlLib.h" 	

#include "RP6I2CmasterTWI.h"

#define I2C_RP6_BASE_ADR 10		

#define I2C_REG_STATUS1 		 0
#define I2C_REG_STATUS2 		 1
#define I2C_REG_MOTION_STATUS 	 2
#define I2C_REG_POWER_LEFT 		 3
#define I2C_REG_POWER_RIGHT 	 4
#define I2C_REG_SPEED_LEFT 		 5
#define I2C_REG_SPEED_RIGHT 	 6
#define I2C_REG_DES_SPEED_LEFT 	 7
#define I2C_REG_DES_SPEED_RIGHT  8
#define I2C_REG_DIST_LEFT_L 	 9
#define I2C_REG_DIST_LEFT_H 	 10
#define I2C_REG_DIST_RIGHT_L     11
#define I2C_REG_DIST_RIGHT_H 	 12
#define I2C_REG_ADC_LSL_L 		 13
#define I2C_REG_ADC_LSL_H 		 14
#define I2C_REG_ADC_LSR_L 		 15
#define I2C_REG_ADC_LSR_H 		 16
#define I2C_REG_ADC_MOTOR_CURL_L 17
#define I2C_REG_ADC_MOTOR_CURL_H 18
#define I2C_REG_ADC_MOTOR_CURR_L 19
#define I2C_REG_ADC_MOTOR_CURR_H 20
#define I2C_REG_ADC_UBAT_L 		 21
#define I2C_REG_ADC_UBAT_H 		 22
#define I2C_REG_ADC_ADC0_L 		 23
#define I2C_REG_ADC_ADC0_H 		 24
#define I2C_REG_ADC_ADC1_L 		 25
#define I2C_REG_ADC_ADC1_H 		 26
#define I2C_REG_RC5_ADR	 		 27
#define I2C_REG_RC5_DATA	 	 28
#define I2C_REG_LEDS	 		 29

#define CMD_POWER_OFF 		0
#define CMD_POWER_ON 		1
#define CMD_CONFIG 			2
#define CMD_SETLEDS 		3
#define CMD_STOP   			4
#define CMD_MOVE_AT_SPEED   5
#define CMD_CHANGE_DIR	    6
#define CMD_MOVE 			7
#define CMD_ROTATE 			8
#define CMD_SET_ACS_POWER	9 
#define CMD_SEND_RC5		10 
#define CMD_SET_WDT			11
#define CMD_SET_WDT_RQ		12

#define ACS_PWR_OFF  0
#define ACS_PWR_LOW  1
#define ACS_PWR_MED  2
#define ACS_PWR_HIGH 3

union {
 	uint8_t byte;
	struct {
		uint8_t batLow:1;
		uint8_t bumperLeft:1;
		uint8_t bumperRight:1;
		uint8_t RC5reception:1;
		uint8_t RC5transmitReady:1;
		uint8_t obstacleLeft:1;
		uint8_t obstacleRight:1;
		uint8_t driveSystemChange:1;
	};
} interrupt_status;

#define INT0_STATUS_CHECK 0
uint8_t block = false;

void task_checkINT0(void)
{
	if(!block && (PIND & EINT1)) 
	{
		block = true;
		I2CTWI_requestRegisterFromDevice(I2C_RP6_BASE_ADR, INT0_STATUS_CHECK, 0, 3);
	}
}

uint8_t messageBuf[8];

void I2C_requestedDataReady(uint8_t dataRequestID)
{
	if(dataRequestID == INT0_STATUS_CHECK) 
	{                                    

        I2CTWI_getReceivedData(messageBuf, 3); 
	
		uint8_t compare = messageBuf[0] ^ interrupt_status.byte;
		interrupt_status.byte = messageBuf[0];
		
		if(compare & 0b01100000) 
		{
			writeString_P("- ACS state changed L: ");
			if(interrupt_status.obstacleLeft)
			{
				writeChar('o');
				setCursorPosLCD(1, 12);
				writeStringLCD_P("LEFT"); 
			}
			else
			{
				writeChar(' ');
				clearPosLCD(1, 12, 4);
			}
			writeString_P(" | R: ");
			if(interrupt_status.obstacleRight)
			{
				writeChar('o');
				setCursorPosLCD(1, 0);
				writeStringLCD_P("RIGHT"); 
			}
			else
			{
				writeChar(' ');	
				clearPosLCD(1, 0, 5);
			}
			if(interrupt_status.obstacleRight && interrupt_status.obstacleLeft)
			{
				externalPort.LEDS = 0b0110;
				writeString_P("   MIDDLE!");
				setCursorPosLCD(1, 7);
				writeStringLCD_P("MID");
			}
			else
			{
				externalPort.LEDS = 0b0000;
				clearPosLCD(1, 7, 3);
			}
			writeChar('\n');
			if(interrupt_status.obstacleLeft)
				externalPort.LED1 = true;
			if(interrupt_status.obstacleRight)
				externalPort.LED4 = true;
			outputExt();
			
			if(interrupt_status.obstacleRight && interrupt_status.obstacleLeft)
			{
				sound(140,10,0);
			}
			else
			{
				if(interrupt_status.obstacleLeft)
					sound(100,5,0);
				if(interrupt_status.obstacleRight)
					sound(120,5,0);
			}
		}
		
		if(compare & 0b00000110)
		{
			writeString_P(" - Bumpers changed: ");
			if(interrupt_status.bumperRight && interrupt_status.bumperLeft)
			{
				writeString_P("MIDDLE!\n");
				sound(200,100,0);
			}
			else
			{
				if(interrupt_status.bumperLeft)
				{
					writeString_P("LEFT!\n");
					sound(200,50,10);
					sound(150,20,0);
				}
				else if(interrupt_status.bumperRight)
				{
					writeString_P("RIGHT!\n");
					sound(200,50,10);
					sound(150,20,0);
				}
				else
				{
					writeString_P("FREE!\n");
				}
			}
		}
		
		if(interrupt_status.RC5reception)
		{
			uint8_t readBuf[2];
			writeString_P("Received RC5 Transmission: ");
			I2CTWI_transmitByte(I2C_RP6_BASE_ADR,I2C_REG_RC5_ADR);
			I2CTWI_readBytes(I2C_RP6_BASE_ADR, readBuf, 2);
			writeString_P("ADR:");writeInteger(readBuf[0],DEC);
			writeString_P(" | DATA:");writeInteger(readBuf[1],DEC);
			writeString_P("\n");
		}
		
		block = false;
	}
}

void task_LCDHeartbeat(void)
{
	static uint8_t heartbeat = false;
	if(getStopwatch1() > 500)
	{
		if(heartbeat)
		{
			clearPosLCD(0, 15, 1);
			heartbeat = false;
		}
		else
		{
			setCursorPosLCD(0, 15);
			writeStringLCD_P("*"); 
			heartbeat = true;
		}
		setStopwatch1(0);
	}
}

void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
	block = false;
}

int main(void)
{
	initRP6Control(); 
	
	initLCD();
    
	writeString_P("\n\nRP6 CONTROL M32 I2C Master Example Program!\n"); 
    writeString_P("\nInterrupts...\n"); 

	I2CTWI_initMaster(100); 
	
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);

	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Example Program 2");
	mSleep(1000);
	setLEDs(0b0000);
    showScreenLCD("ACS Status:", "");

	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);

	startStopwatch1(); 
	
	while(true) 
	{
		task_checkINT0();
        task_I2CTWI();
		task_LCDHeartbeat();
	}
	return 0;
}

