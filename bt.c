/*
 * bt.c
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */
#include "bt.h"


/*
 * Function activates Bluetooth and sets it up to communicate by UART via USCI_A1 at a 921600 baud rate.
 */
void setupBT()
{

	//powers on Bluetooth via BT_POWER_EN pin
	P6DIR |= BIT4;
	P6OUT |= BIT4;

	//Activate UCATXD/ UCARXD pins
	P4SEL |= (BIT4 | BIT5);
	P4DIR |= BIT6 | BIT7; //activate CTS (Is this necessary?)
	P4OUT &= ~(BIT6 + BIT7); //turn on CTS (Is this necessary?)

	UCA1CTL0 = 0; //Not using SPI on this one
	UCA1CTL1 = UCSSEL__ACLK | UCSWRST; //Use ACLK | hold in reset
	//nint(8 * 24 Mhz / 921600) = 208 = 8 * [26] + [0]
	UCA1BRW = 26; //Combo of this and line below sets baud rate to 921600
	UCA1MCTL = UCBRS_0; // Low frequency mode, set modulator to 0 for Baud rate of 921600
	UCA1CTL1 &= ~UCSWRST; //Release from reset


}


/*
 * Function takes an array of chars, the size of the array, and sends through BT
 */
void writeToBT(uint8_t data[], uint8_t size)
{
	int n = 0;

	for (n=0; n < size; n++)
	{
		UCA1TXBUF = data[n];
		while ((UCA1IFG & UCTXIFG) == 0x00) {}; //Wait for data to be transmitted
	}
}


