// -----------------------------------------------------------------------
// Copyright: Dale Schinstock, 2013
//------------------------------------------------------------------------

#ifndef GLO_RX_LINK_INCLUDED_H
#define GLO_RX_LINK_INCLUDED_H

#include <stdint.h>

#include "usart.h"

class GloRxLink
{
  public: // methods
      
    // Constructor
    explicit GloRxLink(Usart * port);

    // Looks for newly transmitted object information and progresses through 
    // state machine.  Needs be called by a periodic task that runs at least 
    // fast enough to handle data stream rate.
    void parse(void);
    
    void setPort(Usart * new_port) { this->port = new_port; }
    
  private: // methods

      void handleReceivedObject(void);
      
      void advanceParse(void) { parse_state++; }

      void resetParse(void) { body_index = 0; parse_state = -1; }
      
  private: // fields

    Usart * port; // port that data gets received over

    int8_t parse_state;
    uint8_t object_id;
    uint8_t num_body_bytes;
    uint8_t body_index;
    uint16_t instance;
    uint8_t glo_object_data[255];

    uint32_t num_messages_received;

};

#endif  // GLO_RX_LINK_INCLUDED_H

