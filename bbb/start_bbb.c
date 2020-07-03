/*
    Copyright © 2019 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <stdarg.h>
#include <stdint.h>

#include "config.h"
#include "support.h"
#include "tlsf.h"
#include "mmu.h"
#include "devicetree.h"
#include "m68k.h"
#include "hunkloader.h"
#include "duffcopy.h"
#include "emulogo.h"
#include "emufeatures.h"
#include "registerallocator.h"

extern uint64_t mmu_user_L1[512];
extern uint64_t mmu_user_L2[4*512];

void platform_init()
{
}

void platform_post_init()
{
}
