/****************************************************************************************
* File: debug_printf.h
*
* Description: Provides debug_printf() which is similar to printf() except it uses the
*              debug serial port as the output.
*
* Created:
****************************************************************************************/

#ifndef DEBUG_PRINTF_H_INCLUDED
#define DEBUG_PRINTF_H_INCLUDED

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <cstdint>

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        MACROS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                      PROTOTYPES
*--------------------------------------------------------------------------------------*/

// Prints the given message to the debug serial port.
// The variable argument (...) are provided so this function
// can be used similar to printf().
void debug_printf
    (
        char * format, // Message to print.
        ...            // Variable arguments. (just like printf() uses)
    );

// Prints the given buffer to the debug serial port.
void debug_print_buffer
    (
        uint8_t  const * buffer_to_print, // Bytes to print to serial port
        uint32_t         bytes_to_print   // Number of bytes to print
    );

#endif // DEBUG_PRINTF_H_INCLUDED
