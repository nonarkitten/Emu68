/*
    Copyright © 2019 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdarg.h>

#include "support.h"



static void waitSerOUT(void *io_reg) {
	while(*((uint8_t*)io_reg + 0x68) == 63) continue;
}

static void putByte(void *io_reg, char chr) {
	waitSerOUT(io_reg);
	if(chr == '\n') {
		*(uint8_t*)io_reg = '\r';
		waitSerOUT(io_reg);
	}
	
	*(uint8_t*)io_reg = chr;
	
}

void kprintf(const char * restrict format, ...) {
    va_list v;
    va_start(v, format);
    vkprintf_pc(putByte, (void*)0x44E09000, format, v);
    va_end(v);
}

void vkprintf(const char * restrict format, va_list args) {
    vkprintf_pc(putByte, (void*)0x44E09000, format, args);
}

void setup_serial(void) {
	// assume uboot has done this for us
}
