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
#include "rtc.h"

namespace wiselib
{
   /** \brief Riot Implementation of \ref clock_concept "Clock Concept"
    *  \ingroup clock_concept
    *  \ingroup settable_clock_concept
    *  \ingroup clock_time_translation_concept
    *  \ingroup riot_facets
    *
    * Riot implementation of the \ref clock_concept "Clock Concept" ...
    */
   template<typename OsModel_P>
   class RiotClockModel
   {
   public:
      typedef OsModel_P OsModel;

      typedef RiotClockModel<OsModel> self_type;
      typedef self_type* self_pointer_t;

      //typedef uint32_t time_t;

      // --------------------------------------------------------------------
      enum ErrorCodes
      {
         SUCCESS = OsModel::SUCCESS,
         ERR_UNSPEC = OsModel::ERR_UNSPEC
      };
      // --------------------------------------------------------------------
      enum
      {
         READY = OsModel::READY,
         NO_VALUE = OsModel::NO_VALUE,
         INACTIVE = OsModel::INACTIVE
      };
      // --------------------------------------------------------------------
      enum
      {
         CLOCKS_PER_SECOND = 1
      };
      // --------------------------------------------------------------------
      RiotClockModel(  )
      {}
      // --------------------------------------------------------------------
      void init()
      {
    	  rtc_init();
    	  rtc_enable();

      }
      // --------------------------------------------------------------------
      int state()
      {
         return READY;
      }
      // --------------------------------------------------------------------
      time_t time()
      {
    	 rtc_get_localtime(&now);
         return (time_t) (mktime(&now));
      }
      // --------------------------------------------------------------------
      int set_time( time_t time )
      {
    	  now = *localtime(&time);
    	  rtc_set_localtime(&now);
		  printf("\n Date Set to: %02u.%02u.%04u %02u:%02u", now.tm_mday, now.tm_mon,(now.tm_year +1900), now.tm_hour, now.tm_min);
         return SUCCESS;
      }
      // --------------------------------------------------------------------
      uint16_t microseconds( time_t time )
      {
       	 rtc_get_localtime(&now);
      	 return (uint32_t) (mktime(&now) / 1000 / 1000);
      }
      // --------------------------------------------------------------------
      uint16_t milliseconds( time_t time )
      {
      	 rtc_get_localtime(&now);
     	 return (uint32_t) (mktime(&now) / 1000);
      }
      // --------------------------------------------------------------------
      uint32_t seconds( time_t time )
      {
     	 rtc_get_localtime(&now);
    	  return (uint32_t) (mktime(&now));
      }

   private:
	  struct timer {
	    time_t start;
	    time_t interval;
	  };
	  struct tm now;

   };
}


