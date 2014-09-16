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
//#include "external_interface/contiki/contiki_radio.h"
#include "external_interface/riot/riot_cc1100_extended_radio.h"
#include "external_interface/riot/riot_os.h"
#include <string.h>
#include <cstdlib>

#define RADIO_NODE_ID 11
double receive_timer;
//int receive_timer;

namespace wiselib {
static const int MAX_REGISTERED_RECEIVERS = 10;
static const int MAX_MESSAGE_LENGTH = 57;

static riot_radio_delegate_t receivers[MAX_REGISTERED_RECEIVERS];
static riot_extended_radio_delegate_t extended_receivers[MAX_REGISTERED_RECEIVERS];

// -----------------------------------------------------------------------
int riot_radio_add_receiver(riot_radio_delegate_t& d) {
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++) {
		if (!receivers[i]) {
			receivers[i] = d;
			return i;
		}
	}
	return -1;
}
// -----------------------------------------------------------------------
int riot_extended_radio_add_receiver(riot_extended_radio_delegate_t& d) {
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++)
		if (!extended_receivers[i]) {
			extended_receivers[i] = d;
			return i;
		}
	return -1;
}
// -----------------------------------------------------------------------
void riot_radio_del_receiver(int idx) {
	receivers[idx] = riot_radio_delegate_t();
}
// -----------------------------------------------------------------------
static void protocol_handler(void* msg, int msg_size,
		packet_info_t* packet_info) {
		receive_timer = ltc4150_get_total_mAh();
		//receive_timer = hwtimer_now();
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++) {
		if (receivers[i]) {
			//thread_print_all();
			receivers[i]((uint8_t) (packet_info->phy_src), msg_size,
					(uint8_t*) msg);
		}
		if (extended_receivers[i]) {
			uint8_t signal_strength_rssi = (uint8_t) packet_info->rssi;
			ExtendedData exData;
			exData.set_link_metric(signal_strength_rssi);
			extended_receivers[i]((uint8_t) (packet_info->phy_src), msg_size,
					(uint8_t*) msg, exData);
			return;
		}
	}
}
// -----------------------------------------------------------------------
void init_riot_cc1100_radio(void) {
	cc1100_set_packet_handler(WISELIB_PROTOCOL, protocol_handler); //layer link layer
	if (cc1100_set_address((radio_address_t) RADIO_NODE_ID)) {
		printf("cc1100 address set to... %d\n", (radio_address_t) RADIO_NODE_ID);
	}
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++) {
		receivers[i] = riot_radio_delegate_t();
		extended_receivers[i] = riot_extended_radio_delegate_t();
	}
}
// -----------------------------------------------------------------------
}

