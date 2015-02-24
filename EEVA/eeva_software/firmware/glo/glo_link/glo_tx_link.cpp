//-------------------------------------------------------------------------
//  Implementation of GloTxLink.
//  Copyright: Dale Schinstock, 2013
//-------------------------------------------------------------------------

#include <string.h>

#include "glo_tx_link.h"
#include "glo_objects.h"
#include "util_assert.h"

// Constructor
GloTxLink::GloTxLink
    (
        Usart * port
    ) :
    port(port),
    num_messages_sent(0),
    num_messages_sent_failed(0)
{  
}

//*****************************************************************************
bool GloTxLink::send
    (
        uint8_t  tx_id,        // ID number associated with glo object to send.
        uint16_t tx_instance   // Instance number associated with glo object to send.
    )
{
    if (port == NULL)
    {
        num_messages_sent_failed++;
        // Don't assert anything in this method since a failed assert will cause a 
        // message to be sent and most likely retrigger the assert.
        // If this is constantly failing it should be obvious, otherwise it's just 
        // during setup (ie if an assert gets thrown before port is setup)
        //assert_always_msg(ASSERT_CONTINUE, "Transmit failed due to null port.");
        return false;
    }
    
    uint32_t num_data_bytes = (uint32_t)gloObjectPtrs[tx_id]->get_num_bytes();    
            
    uint32_t data_pointer = (uint32_t)gloObjectPtrs[tx_id]->get_data_pointer();
    
    // Need to subtract one from instance number since it's indexed off 1.
    uint32_t instance_offset = (uint32_t)(tx_instance-1)*num_data_bytes;
    
    uint32_t instance_ptr = data_pointer + instance_offset;

    send_buffer[0] = '<'; 
    send_buffer[1] = '{'; 
    send_buffer[2] = '(';
    send_buffer[3] = tx_id;
    send_buffer[4] = (uint8_t)((tx_instance<<8)>>8);
    send_buffer[5] = (uint8_t)(tx_instance>>8);
    send_buffer[6] = num_data_bytes;
    send_buffer[num_data_bytes + 7] = 0x0D;    //TODO: CRC 
    send_buffer[num_data_bytes + 8] = 0x0A;    //CR LF
 
    memcpy((void *)&(send_buffer[7]), (void const *)instance_ptr, num_data_bytes);
       
    uint32_t packet_size = num_data_bytes + 9; 
    
    // block here until there is room in the buffer
    while (!port->SendBuffer(send_buffer, (uint16_t)packet_size));

    num_messages_sent++;
    
    return true;

    // should we add a timeout here?
    // numSentFailed++;
    // return(0);
}