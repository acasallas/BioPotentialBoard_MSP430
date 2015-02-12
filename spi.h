#pragma once
/*
 * spi.h
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */
#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>


//delay based on 24 Mhz clock
#define DELAY_1MS(x)    __delay_cycles(24L*1000L*x)
#define DELAY_1US(x)	__delay_cycles(24L*x)

void initCS();
inline void accelCS_On();
inline void accelCS_Off();
inline void gyroCS_On();
inline void gyroCS_Off();
inline void bioCS_On();
inline void bioCS_Off();
uint8_t SpiWriteWithRead(uint8_t b);
void configSPI();
