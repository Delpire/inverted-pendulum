// -----------------------------------------------------------------------
// Copyright: Dale Schinstock, 2013
//------------------------------------------------------------------------

#ifndef GLO_TX_LINK_INCLUDED_H
#define GLO_TX_LINK_INCLUDED_H

#include <stdint.h>

#include "usart.h"

class GloTxLink
{
  public: // methods
      
    // Constructor
    explicit GloTxLink(Usart * port);

    // Copies glo object information and data that is associated with ID and instance
    // into transfer buffer. Returns true if successful.
    bool send
        (
            uint8_t  id,        // ID number associated with glo object to send.
            uint16_t instance   // Instance number associated with glo object to send.
        );
    
    void setPort(Usart * new_port) { this->port = new_port; }
    
  private: //methods

      
  private: // fields

    Usart * port; // port that data gets sent/received over

    uint32_t num_messages_sent;
    uint32_t num_messages_sent_failed;
      
    uint8_t send_buffer[264]; //size = 255(obj size) +3(start) +1(objID) +2(instance) +1(numBytes) +2(end bytes)
    
};

#endif  // GLO_TX_LINK_INCLUDED_H

