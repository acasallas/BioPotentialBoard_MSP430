/*
 * accelgyro.c
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */

#include "accelgyro.h"

/*
 * Gets WHO_AM_I register, then turns on device via CTRL_REG1. Does not modify any other CTRL registries.
 */
void powerAccel()
{
	//First, verify we get the expected value from the WHO_AM_I. This is as per the Microsoft code. There
	//is a loop until the WHO_AM_I responds as expected, perhaps to ensure the device is running.
    char buffer_8 = 0x00;

    do {
    	accelCS_On();
    	buffer_8 = SpiWriteWithRead(0x8F); //READ MODE, Register: Who Am I
    	buffer_8 = SpiWriteWithRead(0x00); //Receive Who Am I response
    	accelCS_Off();
    } while( buffer_8 != 0x32 );

    //Next Message
	accelCS_On();
	SpiWriteWithRead(0x20); //WRITE MODE, Register: CTRL_REG1
	SpiWriteWithRead(MSB_BIT2 | MSB_BIT5 | MSB_BIT6 | MSB_BIT7); // Normal Power | x on | y on | z on
	accelCS_Off();
}

void powerGyro()
{
	//First, verify we get the expected value from the WHO_AM_I. This is as per the Microsoft code. There
	//is a loop until the WHO_AM_I responds as expected, perhaps to ensure the device is running.
    char buffer_8 = 0x00;

    do {
    	gyroCS_On();
    	buffer_8 = SpiWriteWithRead(0x8F); //READ MODE, Register: Who Am I
    	buffer_8 = SpiWriteWithRead(0x00); //Receive Who Am I response
    	gyroCS_Off();
    } while( buffer_8 != 0xD3 );

    //Next Message
	gyroCS_On();
	SpiWriteWithRead(WRITE_MODE | SING_ADDR | CTRL_REG1); //WRITE MODE, Register: CTRL_REG1
	SpiWriteWithRead(MSB_BIT4 | MSB_BIT5 | MSB_BIT6 | MSB_BIT7); // Normal Power | x on | y on | z on
	gyroCS_Off();

	//TODO: set FIFO to bypass
}


void getAccelData(uint8_t xyz[])
{
	accelCS_On();

	SpiWriteWithRead(MSB_BIT0 | MSB_BIT1 | 0x28); //READ MODE, increment mode, OUT_X_L
	xyz[1] = SpiWriteWithRead(0); //read low x coordinate
	xyz[0] = SpiWriteWithRead(0); //read upper x coordinate
	xyz[3] = SpiWriteWithRead(0); //read low y coordinate
	xyz[2] = SpiWriteWithRead(0); //read upper y coordinate
	xyz[5] = SpiWriteWithRead(0); //read low z coordinate
	xyz[4] = SpiWriteWithRead(0); //read upper z coordinate

	accelCS_Off();
}

void getGyroData(uint8_t xyz[])
{
	gyroCS_On();

	SpiWriteWithRead(MSB_BIT0 | MSB_BIT1 | 0x28); //READ MODE, increment mode, OUT_X_L
	xyz[1] = SpiWriteWithRead(0); //read low x coordinate
	xyz[0] = SpiWriteWithRead(0); //read upper x coordinate
	xyz[3] = SpiWriteWithRead(0); //read low y coordinate
	xyz[2] = SpiWriteWithRead(0); //read upper y coordinate
	xyz[5] = SpiWriteWithRead(0); //read low z coordinate
	xyz[4] = SpiWriteWithRead(0); //read upper z coordinate

	gyroCS_Off();
}
