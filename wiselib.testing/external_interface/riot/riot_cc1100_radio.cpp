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
#include "external_interface/riot/riot_cc1100_radio.h"

#define RADIO_NODE_ID 20

double before_send;
//int before_send;

namespace wiselib{
  static const int MAX_REGISTERED_RECEIVERS = 10;
  static const int MAX_MESSAGE_LENGTH = 57;
  static riot_radio_delegate_t receivers[MAX_REGISTERED_RECEIVERS];
// -----------------------------------------------------------------------
int riot_radio_add_receiver(riot_radio_delegate_t& d)
{
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++) {
		if (!receivers[i]) {
			receivers[i] = d;
			return i;
		}
	}
	return -1;
}
// -----------------------------------------------------------------------
void riot_radio_del_receiver(int idx)
{
	receivers[idx] = riot_radio_delegate_t();
}
// -----------------------------------------------------------------------
static void protocol_handler(void* msg, int msg_size,
		packet_info_t* packet_info)
{
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++) {
		if (receivers[i]) {
			receivers[i]((uint8_t) (packet_info->phy_src), msg_size,
					(uint8_t*) msg);
			}
	}
}
// -----------------------------------------------------------------------
void init_riot_cc1100_radio(void)
{
	printf("cc1100 set packet_handler \n");
	cc1100_set_packet_handler(WISELIB_PROTOCOL, protocol_handler); //layer link layer
	if (cc1100_set_address((radio_address_t) RADIO_NODE_ID)) {
		printf("cc1100 address set to... %d\n", (radio_address_t) RADIO_NODE_ID);
	}
	for (int i = 0; i < MAX_REGISTERED_RECEIVERS; i++) {
		receivers[i] = riot_radio_delegate_t();
	}
}
// -----------------------------------------------------------------------
}
