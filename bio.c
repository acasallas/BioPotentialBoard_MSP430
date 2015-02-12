/*
 * bio.c
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */

#include "bio.h"



/* Releases PWDN and RESET pins, runs RESET procedure, stops Continuous Data Read,
 * sets data rate to 1k SPS, then sets Biopotential device to Single-Shot mode.
 * */
void powerBio()
{
	//Note: Internal Bio clock should be running at 2.048 Mhz, so period (Tclk) should be .488 us

	//Set BIO_PWDN to high
	P5DIR |= BIT7;
	P5OUT |= BIT7;

	//Set RESET to high
	P6DIR |= BIT5;
	P6OUT |= BIT5;

	//wait Tpor
	DELAY_1MS(150); //wait for device to power up. Tpor is 2^18 cycles, about 131.072 ms

	P6OUT &= ~BIT5; //pulse RESET low
	DELAY_1US(5); //wait Treset, which is 2 cycles, about 1 us.
	P6OUT |= BIT5; //pulse RESET high

	DELAY_1US(20); //18 Tclk needed for reset...set a delay for more than double to be safe.

	bioCS_On();
	SpiWriteWithRead(SDATAC); //stop continuous read (which is default)
	bioCS_Off();

	//configure Bio for single shot mode
	bioCS_On();
	SpiWriteWithRead( WREG | 0x01 ); //OpCode 1: WRITE to register 0x01, CONFIG1
	SpiWriteWithRead(0x00); //OpCode 2: Write to 1 register
	SpiWriteWithRead(0x04); //1k SPS
	bioCS_Off();

	//configure Bio for single shot mode
	bioCS_On();
	SpiWriteWithRead( WREG | 0x17 ); //OpCode 1: WRITE to register 0x17
	SpiWriteWithRead(0x00); //OpCode 2: Write to 1 register
	SpiWriteWithRead(0x08); //single shot mode
	bioCS_Off();

	P1DIR &= ~BIT7;

}


/* Biopotential board has been set to run in single-shot mode. This function sends the START opCode to get a
 * snapshot of data, throws away the first 3 header bytes, then saves the next 24 data bytes onto
 * the data[] array.
 * */
void getBioData(uint8_t data[]) {

	bioCS_On();
	SpiWriteWithRead(START); //send START opCode
	bioCS_Off();

	while (P1IN & BIT7) {}; //wait until DRDY goes down

	bioCS_On();
	SpiWriteWithRead(RDATA);
	uint8_t header[3];

	int n = 0;

	for (n=0; n < 3; n++) {
		header[n] = SpiWriteWithRead(0x00);
	}

	for (n=0; n < 24; n++) {
		data[n] = SpiWriteWithRead(0x00);
	}
	bioCS_Off();


}
