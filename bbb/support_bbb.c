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

static inline void waitSerOUT(void *io_base) {
	(void)io_base;
}

static inline void putByte(void *io_base, char chr) {
	(void)io_base;
	(void)chr;
}

void kprintf(const char * restrict format, ...) {
	(void)format;
}

void vkprintf(const char * restrict format, va_list args) {
	(void)format;
	(void)args;
}

void setup_serial(void) {
}
