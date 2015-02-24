/****************************************************************************************
* File: dma_rx.h
*
* Description: Header file for dma_rx.cpp
*
* Created:
****************************************************************************************/

#ifndef DMA_RX_INCLUDED_H
#define DMA_RX_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "stm32f4xx.h"

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/

/******************************************************************************
* Class: DmaRx
*
* Description: Wraps a circular buffer that works with direct memory access.  All
*              The user has to do is instantiate this class and then read out the
*              bytes when they are ready.
******************************************************************************/
class DmaRx
{
public: // methods

    // Constructor
    DmaRx
        (
            DMA_Stream_TypeDef * dma_stream,           // DMAy_StreamX where y[1:2] and X[0:7]
            uint32_t             channel,              // Channel associated with stream.
            uint32_t             periph_base_address,  // Base data address of peripheral.
            uint32_t             buff_length           // Length of memory buffer needed.
        );

    // Destructor
    ~DmaRx(void);

    // Get the next available byte from buffer. Returns false if nothing available.
    // Should be called at high enough rate that the circular buffer is not overwritten
    // by the DMA between clearing it.
    bool GetByte
        (
            uint8_t * byte // Pointer to byte to be returned.
        );

private: // fields

    uint8_t  * buff;        // Pointer to dynamically allocated DMA receive buffer.
    uint32_t   buff_length; // Length of receive buffer.
    uint32_t   buff_bottom; // Index to the next available byte in rx buffer.

    DMA_Stream_TypeDef * dma_stream; // DMAy_StreamX where y[1:2] and X[0:7]

}; // DmaRx

#endif  // DMA_RX_INCLUDED_H
