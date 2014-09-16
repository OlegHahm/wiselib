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

      typedef uint32_t time_t;

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
         CLOCKS_PER_SECOND = 1000
      };
      // --------------------------------------------------------------------
      RiotClockModel(  )
      {}
      // --------------------------------------------------------------------
      void init()
      {}
      // --------------------------------------------------------------------
      int state()
      {
         return READY;
      }
      // --------------------------------------------------------------------
      time_t time()
      {
         return hwtimer_now();
      }
      // --------------------------------------------------------------------
      int set_time( time_t time )
      {
         return ERR_UNSPEC;
      }
      // --------------------------------------------------------------------
      uint16_t microseconds( time_t time )
      {
         return (uint16_t) (hwtimer_now());
      }
      // --------------------------------------------------------------------
      uint16_t milliseconds( time_t time )
      {
         return (uint16_t) ((hwtimer_now()) / 1000);
      }
      // --------------------------------------------------------------------
      uint32_t seconds( time_t time )
      {
         return (uint32_t) ((hwtimer_now()) / 1000 / 1000);
      }

   private:
	  struct timer {
	    time_t start;
	    time_t interval;
	  };

   };
}


