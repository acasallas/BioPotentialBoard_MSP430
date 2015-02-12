#pragma once
/*
 * accelgyro.h
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */
#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>


//Re-definitions of bits 0-7 for MSB (Most Significant Bit First)
#define	MSB_BIT0	BIT7
#define MSB_BIT1	BIT6
#define MSB_BIT2	BIT5
#define MSB_BIT3	BIT4
#define MSB_BIT4	BIT3
#define MSB_BIT5	BIT2
#define MSB_BIT6	BIT1
#define MSB_BIT7	BIT0

//Definitions for communication with gyro and acccelerometer
#define	WRITE_MODE	0x00
#define	READ_MODE	MSB_BIT0
#define	SING_ADDR	0x00
#define	INCR_ADDR	MSB_BIT1
#define	CTRL_REG1	0x20


void powerAccel();
void powerGyro();

void getAccelData(uint8_t xyz[]);
void getGyroData(uint8_t xyz[]);
