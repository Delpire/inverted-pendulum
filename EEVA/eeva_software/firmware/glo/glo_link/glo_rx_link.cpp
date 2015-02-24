//-------------------------------------------------------------------------
//  Implementation of GloRxLink.
//  Copyright: Dale Schinstock, 2013
//-------------------------------------------------------------------------

#include <string.h>

#include "glo_rx_link.h"
#include "glo_objects.h"
#include "util_assert.h"

// Constructor
GloRxLink::GloRxLink
    (
        Usart * port
    ) :
    port(port),
    parse_state(-1),
    object_id(0),
    num_body_bytes(0),
    body_index(0),
    instance(0),
    num_messages_received(0)
{  
    resetParse();
}

//*****************************************************************************
void GloRxLink::parse(void)   // not reentrant or thread safe
{                                       
    const uint8_t msg_frame[3] = {'<', '{', '('};
    
    uint8_t in_byte = 0; // new byte read in from port
    bool object_pending = false; // true if received new glo object
    
    if (port == NULL)
    {
        assert_always_msg(ASSERT_CONTINUE, "Parse failed due to null port.");
        return;
    }
        
    while (port->GetByte(&in_byte))
    {
        switch (parse_state) 
        {
            case -1:  // looking for first frame byte
                if (in_byte == msg_frame[0]) { advanceParse(); }
                else { resetParse(); }
                break;
            case 0:  // looking for 2nd frame byte
                if (in_byte == msg_frame[1]) { advanceParse(); }
                else { resetParse(); }
                break;
            case 1:  // looking for 3rd frame byte
                if (in_byte == msg_frame[2]) { advanceParse(); }
                else { resetParse(); }
                break;
            case 2:  // pulling out id
                object_id = in_byte;
                advanceParse();
                break;
            case 3:  // pulling out byte one of instance
                instance = in_byte;
                advanceParse();
                break;
            case 4:  // pulling out byte two of instance
                instance += (uint16_t)(in_byte)<<8;
                advanceParse();
                break;
            case 5:  // pulling out length of data
                num_body_bytes = in_byte;
                advanceParse();
                break;
            case 6:  // pulling out body
                glo_object_data[body_index] = in_byte;
                body_index++;
                
                // if this is last byte then advance parsing state
                if (body_index >= num_body_bytes) { advanceParse(); }
                break;
            case 7: // looking for CR (0x0D) -- should add CRC check for serial
                if (in_byte == 0x0D) { advanceParse(); }
                else { resetParse(); }
                break;
            case 8: // looking for LF (0x0A) 
                if (in_byte == 0x0A)
                {
                    object_pending = true;
                }
                
                resetParse();

                break;
            default: // safety reset
                resetParse();
                break;  
        }
        
            if (object_pending)
            {
                object_pending = false;
                handleReceivedObject();
            }
    }
}

//*****************************************************************************
void GloRxLink::handleReceivedObject(void)
{
    if (object_id == 254) // an object is requested
    {
        // TODO put requested item in queue
    }
    else if (object_id == 255)
    {
        // TODO Ack/Nack
    }
    else // a new object has been received
    {
        // To keep this generic and avoid updating this for every new glo object
        // we would have to only access the glo meta data.  In order to have one
        // central write callback (ie to do bound checking, system updates, etc)
        // there would have to be a separate 'on receive' callback that would
        // cast the data and publish it to the object. Since that's essentially  
        // the same thing, in my opinion, its easier to just do it here.
        // **NOTE: If more than one instance post fix ",instance"
        switch (object_id)
        {
            case GLO_ID_DRIVING_COMMAND:
                glo_driving_command.publish((glo_driving_command_t *)glo_object_data);
                break;
            case GLO_ID_CAPTURE_COMMAND:
                glo_capture_command.publish((glo_capture_command_t *)glo_object_data);
                break;
            default:
                assert_always_msg(ASSERT_CONTINUE, "Received unhandled glo object with id: %d", object_id);
                break;
        }

        num_messages_received++;

        // Acknowledge by sending the object back.  Costly, but simple.
        //this->send(object_id, instance);
    }
}
