#pragma once
/*
 * bio.h
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */
#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>

#include "spi.h"

//Bio Opcodes
#define RESET	0x06
#define START	0x08
#define STOP	0x0A
#define RDATAC	0x10
#define SDATAC	0x11
#define RDATA	0x12
#define RREG	0x20
#define WREG	0x40



void powerBio();
void getBioData(uint8_t data[]);
