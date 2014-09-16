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
#ifndef CONNECTOR_RIOT_TIMER_H
#define CONNECTOR_RIOT_TIMER_H

#include "external_interface/riot/riot_types.h"
#include "util/delegates/delegate.hpp"


extern "C" {
#include "hwtimer.h"
#include "vtimer.h"
#include "timex.h"
#include "flags.h"
#include "msg.h"
#include "thread.h"
#include "rtc.h"
#include "transceiver.h"
#include "mutex.h"
#include "kernel.h"

//#define VTIMER_STACK_SIZE	400
//s#define PRIORITY_VTIMER	22
}

namespace wiselib {

typedef delegate1<void, void*> riot_timer_delegate_t;
// -----------------------------------------------------------------------
struct RiotTimerItem {
	riot_timer_delegate_t cb;
	void *ptr;
	struct vtimer_t vt;
};
// -----------------------------------------------------------------------
void init_riot_timer(void);
void riot_timer_thread(void);
RiotTimerItem* get_riot_timer_item(void);

// -----------------------------------------------------------------------
/** \brief Riot Implementation of \ref timer_concept "Timer Concept"
 *  \ingroup timer_concept
 *
 * Riot implementation of the \ref timer_concept "Timer Concept" ...
 */
template<typename OsModel_P>
class RiotTimerModel {
public:
	typedef OsModel_P OsModel;
	typedef RiotTimerModel<OsModel> self_type;
	typedef self_type* self_pointer_t;
	typedef struct vtimer *swt;
	typedef uint32_t millis_t;

// --------------------------------------------------------------------
	enum ErrorCodes {
		SUCCESS = OsModel::SUCCESS,
		ERR_UNSPEC = OsModel::ERR_UNSPEC,
		ERR_NOTIMPL = OsModel::ERR_NOTIMPL
	};
// -----------------------------------------------------------------
	template<typename T, void(T::*TMethod)(void*)>
	int set_timer(millis_t millis, T *obj_pnt, void *userdata) {

		//Get one of 'MAX_REGISTERED_TIMER's Timer Item
		RiotTimerItem *riot_timer_item = get_riot_timer_item();

		// no free timer item left
		if (!riot_timer_item) {
			return ERR_UNSPEC;
		}
		timex_t timer_duration = timex_set(0, millis * 1000);

		//set struct for callback pointer and userdata
		riot_timer_item->cb = riot_timer_delegate_t::from_method < T, TMethod > (obj_pnt);
		riot_timer_item->ptr = userdata;

		//Set VTimer to send 'riot_timer_item' to 'pid_timer' after 'timer_duration'
		if (!vtimer_set_msg(&(riot_timer_item->vt), timer_duration, pid_timer,
				(void*) riot_timer_item)) {
			return SUCCESS;
		} else {
			printf("Error Setting Timer.");
			return ERR_UNSPEC;
		}
	}
	;
// -----------------------------------------------------------------------
	int riotTimerPid(void) {
		return pid_timer;
	}
	;
// -----------------------------------------------------------------------
	RiotTimerModel(void) {
		//create Thread for vtimer callback function
		pid_timer = thread_create(riot_timer_stack,
				sizeof(riot_timer_stack), PRIORITY_MAIN - 1, CREATE_STACKTEST,
				riot_timer_thread, "fw_timer_thread");
	}
	;
// -----------------------------------------------------------------------
private:
	char riot_timer_stack[5000];
	int pid_timer;
};
// -----------------------------------------------------------------------
}
#endif
