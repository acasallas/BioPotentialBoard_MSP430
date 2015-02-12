/*
 * spi.c
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */

#include "spi.h"


//Sets all CS that communicate through USCI_B0 to 1, which is OFF
void initCS()
{
	//set BIO_CS, GYRO_CS, and ACCEL_CS to output
	P5DIR |= BIT0;
	P6DIR |= BIT2 | BIT3;

	//Set CS's to 1, which if OFF
	P5OUT |= BIT0; //set BIO_CS to 1
	P6OUT |= BIT2 | BIT3; //set GYRO_CS and ACCEL_CS to 1
}

inline void accelCS_On()
{
	P6OUT &= ~BIT2;
}

inline void accelCS_Off()
{
	P6OUT |= BIT2;
}

inline void gyroCS_On()
{
	P6OUT &= ~BIT3;
}

inline void gyroCS_Off()
{
	P6OUT |= BIT3;
}

inline void bioCS_On()
{

	P5OUT &= ~BIT0;
	DELAY_1US(4);
}


//Internal Bio clock runs at 2.048 Mhz, so one period is .5 uS.
//Doc says to wait at least four cycles before turning Bio CS off, so wait 4 uS.
inline void bioCS_Off()
{
	DELAY_1US(4);
	P5OUT |= BIT0;
}

uint8_t SpiWriteWithRead(uint8_t b)
{
    UCB0TXBUF = b;
    while ((UCB0IFG & UCTXIFG) == 0x00) {};       // Wait for Transmit done
    while ((UCB0IFG & UCRXIFG) == 0x00) {};       // Wait for Receive

    volatile uint8_t result = UCB0RXBUF;
    return result;

}

void configSPI()
{
    //Turn on and configure USCI_B0
    P3SEL |= BIT0 | BIT1 | BIT2; //SIMO | MISO | CLK
    UCB0CTL0 =  UCMSB | UCMST | UCMODE_0 | UCSYNC; //mode 3 | msb first | master | 3-wire | synchronous
    UCB0CTL1 = UCSSEL1 | UCSWRST; // use SM Clock | hold clock at reset
    UCB0BR1 = 0; //top clock frequency divider word
    UCB0BR0 = 12; //give frequency of 1 Mhz
    UCB0CTL1 &= ~UCSWRST; //release from reset
}
