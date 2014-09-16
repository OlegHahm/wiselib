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
#include "riot_timer.h"

#define RCV_BUFFER_SIZE     (3)
#define MAX_REGISTERED_TIMER       (10)


namespace wiselib {

// -----------------------------------------------------------------------
static RiotTimerItem timer_items[MAX_REGISTERED_TIMER];
// -----------------------------------------------------------------------
RiotTimerItem* get_riot_timer_item(void) {
	//Search for unused Timer Items
	for (int i = 0; i < MAX_REGISTERED_TIMER; i++) {
		if (!timer_items[i].cb) {
			return &timer_items[i];
		}
	}
	return 0;
}

// -----------------------------------------------------------------------
void init_riot_timer(void) {
	//init Timer Items
	for (int i = 0; i < MAX_REGISTERED_TIMER; i++)
		timer_items[i].cb = riot_timer_delegate_t();
}

// -----------------------------------------------------------------------
void riot_timer_thread(void) {
	msg_t m;
	msg_t msg_q[RCV_BUFFER_SIZE];
	msg_init_queue(msg_q, RCV_BUFFER_SIZE);
	while (1) {
		msg_receive(&m);
		RiotTimerItem *t = (RiotTimerItem*) m.content.ptr;
		t->cb(t->ptr);
		//clear callback entry so that it can be re-used
		t->cb = riot_timer_delegate_t();
	}
}
}
// -----------------------------------------------------------------------
