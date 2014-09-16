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


#include "util/delegates/delegate.hpp"
#include "util/base_classes/extended_radio_base.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include "board.h"
#include "cc1100.h"
#include "ltc4150.h"
#include "thread.h"
#include "ps.h"
}

enum SpecialNodeIds {
  WISELIB_PROTOCOL = 2,
  WISELIB_PRIORITY = 3
};
//extern int receive_timer;
extern double receive_timer;


namespace wiselib
{

class ExtendedData {
public:

    ExtendedData() {
    }

    uint8_t link_metric() const {
        return link_metric_;
    };

    void set_link_metric(uint8_t lm) {
        link_metric_ = lm;
    };

private:
    uint8_t link_metric_;
};


   typedef delegate3<void, uint8_t, uint8_t, uint8_t*> riot_radio_delegate_t;
   typedef riot_radio_delegate_t radio_delegate_t;

   typedef delegate4<void, uint8_t, uint8_t, uint8_t*,
 		  const ExtendedData&> riot_extended_radio_delegate_t;
   int riot_extended_radio_add_receiver( riot_extended_radio_delegate_t& ex_delegate );


   // -----------------------------------------------------------------------


   void init_riot_cc1100_radio( void );
   int riot_radio_add_receiver( riot_radio_delegate_t& delegate );
   void riot_radio_del_receiver( int idx );

   // -----------------------------------------------------------------------
   /** \brief Riot Implementation of \ref radio_concept "Radio concept"
    *  \ingroup radio_concept
    *
    * Riot implementation of the \ref radio_concept "Radio concept" ...
    */
   template<typename OsModel_P>
   class RiotExtendedRadioModel
{
public:

      typedef OsModel_P OsModel;

      typedef RiotExtendedRadioModel<OsModel> self_type;
      typedef self_type* self_pointer_t;
      typedef uint8_t node_id_t;
      typedef uint8_t block_data_t;
      typedef uint8_t size_t;
      typedef uint8_t message_id_t;

      // -------------------------------------------------------------------
      double message_receive_time()
      {
    	  return receive_timer;
      }
      // --------------------------------------------------------------------
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
    			int node_id = cc1100_get_address();
    			printf("Change node_id to... %i\n", node_id);
    	         return SUCCESS;
    		}
    		else
    			return -1;

      }

      // --------------------------------------------------------------------
      int send( node_id_t id, size_t len, block_data_t *data )
      {
    	  printf("sending....");
//          int result = cc1100_send_csmaca(id, WISELIB_PROTOCOL, 1,(char*)data, len);
          int result = cc1100_send(id, WISELIB_PROTOCOL, 1,(char*)data, len);
    	  if (result > 0) {
    		  printf(" to %i\r\n", id);
    		  //thread_print_all();
    		  return SUCCESS;
    	  } else {
    		  if (result == RADIO_CS_TIMEOUT) {
    			  printf("CS timeout reached, air was never free!\r\n");
    		  }
    		  return ERR_UNSPEC;
    	  }
    	  return SUCCESS;
      }
      // -------------------------------------------------------------------
      void receive( struct abc_conn* )
      {

      }


      // --------------------------------------------------------------------
      template<class T, void (T::*TMethod)(node_id_t, size_t, block_data_t*)>
      int reg_recv_callback(T *obj_pnt) {
    	  riot_radio_delegate_t delegate =
        		  riot_radio_delegate_t::template from_method<T, TMethod>( obj_pnt );
          return riot_radio_add_receiver( delegate );
      }
      // --------------------------------------------------------------------

      template<class T, void (T::*TMethod)(node_id_t, size_t, block_data_t*, const ExtendedData&) >
      int reg_recv_ex_callback(T *obj_pnt) {
          riot_extended_radio_delegate_t ex_delegate =
        		  riot_extended_radio_delegate_t::template from_method<T, TMethod>( obj_pnt );
          return riot_extended_radio_add_receiver( ex_delegate );

      }
      // --------------------------------------------------------------------
      int unreg_recv_callback( int idx )
      {
    	 riot_radio_del_receiver( idx );
         return SUCCESS;
      }
   };
}
#endif
