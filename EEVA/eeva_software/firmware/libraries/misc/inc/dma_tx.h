/****************************************************************************************
* File: dma_tx.h
*
* Description: Header file for dma_tx.cpp
*
* Created:
****************************************************************************************/

#ifndef DMA_TX_INCLUDED_H
#define DMA_TX_INCLUDED_H

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
* Class: DmaTx
*
* Description: Wraps a buffer that works with direct memory access. Once that user places
*              data into the buffer it will be sent over the configured stream.
******************************************************************************/
class DmaTx
{
public: // methods

    // Constructor
    DmaTx
        (
            DMA_Stream_TypeDef * dma_stream,  // DMAy_StreamX where y[1:2] and X[0:7]
            uint32_t             channel,     // Channel associated with stream.
            IRQn                 dma_irq_num, // Interrupt request number
            uint32_t             periph_base_address,  // Base data address of peripheral.
            uint32_t             transfer_complete_bit,// DMA_IT_TCIFx where x is the Stream number
            uint32_t             transfer_error_bit,   // DMA_IT_TEIFx where x is the Stream number
            uint32_t             buff_length           // Length of memory buffer needed.
        );

    // Destructor
    ~DmaTx(void);

    // Copy array at 'data' to the transfer buffer.  Returns false if not enough
    // room in the buffer.  The buffer is cleared by dma transfers set up in the
    // tx DMA ISR.
    bool SendBuffer
        (
            uint8_t  const * data, // New data to transfer.
            uint16_t         len   // How many bytes to transfer.
        );

    void HandleISR(void);

private: // methods

    // Attempts to place data into transfer buffer.  If not enough
    // room then returns false.
    bool CopyToTransferBuffer
        (
            uint8_t  const * data,   // Buffer containing new data.
            uint16_t         len,    // How many bytes to transfer.
            uint16_t       * start,  // Start index of next transfer.
            uint16_t       * end     // Ending index of next transfer.
        );

    // Starts new transfer at specified index of the transfer buffer.
    bool ActivateDMATransfer
        (
            uint16_t start // Index in transfer buffer to start sending from.
        );

private: // fields

    uint8_t  * buff;        // Pointer to dynamically allocated DMA transfer buffer.
    uint32_t   buff_length; // Length of transfer buffer.
    uint32_t   buff_top;    // Index to the last byte in the tx buffer.
    uint32_t   dma_top;     // Index to the last byte to be transferred by current dma cycle.
    bool       dma_active;  // Indicates if a dma transfer is currently active.

    uint32_t transfer_complete_bit;
    uint32_t transfer_error_bit;
    uint32_t error_count;
    IRQn dma_irq_num; // Interrupt number.
    DMA_Stream_TypeDef * dma_stream; // DMAy_StreamX where y[1:2] and X[0:7]

}; // DmaTx

#endif  // DMA_TX_INCLUDED_H
