#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>
#include <string.h>

#include "accelgyro.h"
#include "bt.h"
#include "spi.h"
#include "bio.h"

/*
 * Biopotential board - MSP430F5342
 * Outputs 39-byte packets of gyro, accelerometer, and biopotential data at 256 Hz.
 */



/*
 * Sources ACLK, SMCLK, and MCLK from XT2, resulting in 24 Mhz speed. SMCLK and MCLK are then divided in half
 * to achieve 12 Mhz speed.
 * */
void initClocks()
{

    //Set SMCLK and MCLK to XT2CLK
    P5SEL |= BIT2 | BIT3; //select XT2IN and XT2OUT

    // XT2 Drive Enabled, XT2 pin internal, XT2 On 0b11x0xxx0xxxxxxxx, x = 0
    UCSCTL6 = 0x8000;

    // Wait for the XT2 Fault flag to clear
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
        SFRIFG1 &= ~OFIFG;
    }while(UCSCTL7 & XT2OFFG);

    UCSCTL4 = SELM__XT2CLK | SELS__XT2CLK | SELA__XT2CLK; //Source ACLK, MCLK, and SMCLK

    UCSCTL5 = DIVS0 | DIVM0; //Divides the SMCLK and MCLK by 2, so now they run at 12 MHZ
}




/* Starts Timer A, which will trigger the interrupts that get the data at 256 Hz.
 * */
void startTimerA()
{

    TA1CCR0 = 23436; //12 MHz SMCLK freq divided by 2, then divided by 23437 results in 256 Hz.
    TA1CCTL0 = CCIE; //enable interrupts on compare 0
    TA1CTL = MC_1 | ID_1 | TASSEL_2 | TACLR;
    //     up mode|divides by 2 | uses SMCLK | reset timer
}





int main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initClocks(); //MCLK and SMCLK initialized to 12 MHZ, ACLK initialized to 24 MHZ

    setupBT(); //Sets up Bluetooth using UART via USCA0

    initCS(); //sets accel, gyro, and bio CS

    configSPI(); //set up SPI on USCB0

    powerBio(); //power Biopotential Board
    powerAccel(); //power Accelerometer
    powerGyro(); //power Gyro

    startTimerA();

    __enable_interrupt(); //always needed for interrupts



    while(1) {
    	__low_power_mode_0(); //Mode 0 allows SMCLK to keep running
    }

	return 0;
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void TA0_ISR (void)
{


	//send header
	uint8_t header[] = {'G', 'E', 0x15};
	writeToBT(header, 3);

	//accel data
	uint8_t accelD[] = {0,0,0,0,0,0};
	getAccelData(accelD);
	writeToBT(accelD,6);

	//gyro data
	uint8_t gyroD[] = {0,0,0,0,0,0};
	getGyroData(gyroD);
	writeToBT(gyroD,6);

	//biopotential board data
	uint8_t bioD[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	getBioData(bioD);
	writeToBT(bioD,24);

}



