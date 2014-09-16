/***************************************************************************
 ** This file is part of the generic algorithm library Wiselib.           **
 ** Copyright (C) 2008,2009 by the Wisebed (www.wisebed.eu) project.      **
 **                                                                       **
 ** The Wiselib is free software: you can redistribute it and/or modify   **
 ** it under the terms of the GNU Lesser General Public License as        **
 ** published by the Free Software Foundation, either version 3 of the    **
 ** License, or (at your option) any later version.                       **
 **                                                                       **
 ** The Wiselib is distributed in the hope that it will be useful,        **
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of        **
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
 ** GNU Lesser General Public License for more details.                   **
 **                                                                       **
 ** You should have received a copy of the GNU Lesser General Public      **
 ** License along with the Wiselib.                                       **
 ** If not, see <http://www.gnu.org/licenses/>.                           **
 ***************************************************************************/

#include "external_interface/riot/riot_types.h"

#include <sys/time.h>

#include "gpioint.h"
#include "hwtimer.h"
#include "kernel.h"
#include "msg.h"

#include "vtimer.h"
#include "timex.h"
#include "thread.h"
#include "board.h"
#include "rtc.h"
#include "cc1100.h"
#include "ltc4150.h"

void board_init_drivers(void) {

	// minimal
	gpioint_init();
	hwtimer_init();
	hwtimer_wait(200000);
	printf("hwtimer....[OK]\n");

	// timers
	rtc_init();

	vtimer_init();
	printf("vtimer....[OK]\n");
	printf("clock system....[OK]\n");

	// power monitor
	ltc4150_init();
	ltc4150_start();
	printf("pwrman....[OK]\n");

	cc1100_init();
	printf("cc1100..[OK]\n");

	// Init done, clear green LED
	LED_GREEN_TOGGLE;
	printf("# Init completed.\n");
}

// --------------------------------------------------------------------------
void riot_os_main();
// --------------------------------------------------------------------------
int main(void) {

	LED_GREEN_TOGGLE;

	// init all Driver
	board_init_drivers();
	printf("riot: Startup\n");
	hwtimer_wait(1000 * 100);
	riot_os_main();
/*
	while (1)
	return 1;
	*/
}
