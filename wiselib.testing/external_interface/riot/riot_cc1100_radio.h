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
#ifndef CONNECTOR_RIOT_CC1100_RADIOMODEL_H
#define CONNECTOR_RIOT_CC1100_RADIOMODEL_H

#include "external_interface/riot/riot_types.h"
#include "util/delegates/delegate.hpp"

extern "C" {
#include "board.h"
#include "cc1100.h"
#include "ltc4150.h"
#include "thread.h"
#include "ps.h"
#include "hwtimer.h"

}

enum SpecialNodeIds {
  WISELIB_PROTOCOL = 2,
  WISELIB_PRIORITY = 3
};

//extern int before_send;
extern double before_send;

namespace wiselib
{

   typedef delegate3<void, uint8_t, uint8_t, uint8_t*> riot_radio_delegate_t;
   typedef riot_radio_delegate_t radio_delegate_t;
   // -----------------------------------------------------------------------
   void init_riot_cc1100_radio( void );
   int riot_radio_add_receiver( riot_radio_delegate_t& delegate );
   void riot_radio_del_receiver( int idx );
   void riot_notify_receivers( struct abc_conn *c );
   // -----------------------------------------------------------------------
   // -----------------------------------------------------------------------
   // -----------------------------------------------------------------------
   /** \brief Riot Implementation of \ref radio_concept "Radio concept"
    *  \ingroup radio_concept
    *
    * Riot implementation of the \ref radio_concept "Radio concept" ...
    */
   template<typename OsModel_P>
   class RiotRadioModel
   {
   public:
      typedef OsModel_P OsModel;

      typedef RiotRadioModel<OsModel> self_type;
      typedef self_type* self_pointer_t;

      typedef uint8_t node_id_t;
      typedef uint8_t block_data_t;
      typedef uint8_t size_t;
      typedef uint8_t message_id_t;
      // --------------------------------------------------------------------
      ///*
      double get_before_send()
      {
    	  return before_send;
      }
      //*/
      /*
	  int get_before_send()
      {
    	  return before_send;
      }
//*/
      enum ErrorCodes
      {
         SUCCESS = OsModel::SUCCESS,
         ERR_UNSPEC = OsModel::ERR_UNSPEC,
         ERR_NOTIMPL = OsModel::ERR_NOTIMPL

      };
      // --------------------------------------------------------------------
      enum SpecialNodeIds {
         BROADCAST_ADDRESS = CC1100_BROADCAST_ADDRESS,///< All nodes in communication range
         NULL_NODE_ID      = -1   ///< Unknown/No node id

      };
      // --------------------------------------------------------------------
      enum Restrictions {
         // max is 58
         MAX_MESSAGE_LENGTH = 57 ///< Maximal number of bytes in payload
      };
      // --------------------------------------------------------------------
      int enable_radio()
      {
    	  return SUCCESS;
      }
// --------------------------------------------------------------------
      int disable_radio()
      {
         return SUCCESS;
      }
      // --------------------------------------------------------------------
      int set_channel(int channel) {
          if ((channel >= 0) && (channel <= 24)) {
        	  cc1100_set_channel(channel);
              return cc1100_get_channel;
          } else {
              return -1;
          }
      }
      // --------------------------------------------------------------------
      node_id_t id()
      {
    	  return cc1100_get_address();
      }
      // --------------------------------------------------------------------
      int set_node_id(int id)
      {
    		if (cc1100_set_address(id)) {
    	         return SUCCESS;
    		}
    		else
    			return -1;
      }

      // --------------------------------------------------------------------
      int send( node_id_t id, size_t length, block_data_t *data )
      {
          int result = cc1100_send(id, WISELIB_PROTOCOL, 1,(char*)data, length);
    	  if (result > 0) {
    		  return SUCCESS;
    	  } else {
				if (result == RADIO_CS_TIMEOUT) {
    			  printf("CS timeout reached, air was never free!\r\n");
				}
				else {
					printf("Error Code %i \r\n", result);
				}
    		 return ERR_UNSPEC;
    	  }
    	  return SUCCESS;
      }
      // --------------------------------------------------------------------
	  
      template<class T, void (T::*TMethod)(node_id_t, size_t, block_data_t*)>
      int reg_recv_callback( T *obj_pnt );
      // --------------------------------------------------------------------
      int unreg_recv_callback( int idx )
      {
         riot_radio_del_receiver( idx );
         return SUCCESS;
      }
   };
   // -----------------------------------------------------------------------
   template<typename OsModel_P>
   template<class T,
            void (T::*TMethod)(
                      typename RiotRadioModel<OsModel_P>::node_id_t,
                      typename RiotRadioModel<OsModel_P>::size_t,
                      typename RiotRadioModel<OsModel_P>::block_data_t*)>
   int
   RiotRadioModel<OsModel_P>::
   reg_recv_callback( T *obj_pnt )
   {
      riot_radio_delegate_t delegate =
    		  riot_radio_delegate_t::from_method<T, TMethod>( obj_pnt );
      return riot_radio_add_receiver( delegate );
   }
}

#endif
