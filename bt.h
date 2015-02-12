#pragma once
/*
 * bt.h
 *
 *  Created on: Jul 21, 2014
 *      Author: Alan
 */
#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>

void setupBT();
void writeToBT(uint8_t data[], uint8_t size);
