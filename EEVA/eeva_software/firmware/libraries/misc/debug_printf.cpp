/****************************************************************************************
* File: debug_printf.c
*
* Description: Provides thread-safe printf capabilities to the debug serial port.
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <cstdio>
#include <cstring>
#include <cstdarg>

#include "glo_os_references.h"
#include "telemetry_send_task.h"
#include "telemetry_text_size.h"
#include "glo_objects.h"
#include "debug_printf.h"
#include "usart.h"

/*---------------------------------------------------------------------------------------
*                                   LITERAL CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
                                    MEMORY CONSTANTS
---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                      VARIABLES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: debug_printf
*
* Description:  Prints the given message to the debug serial port.
*               The variable argument (...) are provided so this function
*               can be used similar to printf().
*****************************************************************************/
void debug_printf
    (
        char * format, // Message to print.
        ...            // Variable arguments. (just like printf() uses)
    )
{
    glo_debug_message_t debug_message;

    va_list args;
    va_start(args, format);
    vsnprintf(debug_message.text, telemetry_text_size, format, args);
    va_end(args);

    glo_debug_message.publish(&debug_message);
    
    send_task->send(glo_debug_message.get_id());
        
} // debug_printf()
