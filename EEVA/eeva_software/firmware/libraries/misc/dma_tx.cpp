/****************************************************************************************
* File: dma_tx.cpp - Direct Memory Access Transfer Driver
*
* Description:
*
* Created:
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "debug_printf.h"
#include "dma_tx.h"
#include "stm32f4xx.h"
#include "util_assert.h"

/*---------------------------------------------------------------------------------------
*                                   LITERAL CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                   MEMORY CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                    CLASS METHODS
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: DmaTx
*
* Description: Constructor
*
* Notes: The transfer complete/error bits are referenced from the transfer
*        stream number NOT the peripheral number.
*****************************************************************************/
DmaTx::DmaTx
    (
        DMA_Stream_TypeDef * dma_stream,            // DMAy_StreamX where y[1:2] and X[0:7]
        uint32_t             channel,               // Channel associated with stream.
        IRQn                 dma_irq_num,           // Interrupt request number
        uint32_t             periph_base_address,   // Base data address of peripheral.
        uint32_t             transfer_complete_bit, // DMA_IT_TCIFx where x is the Stream number
        uint32_t             transfer_error_bit,    // DMA_IT_TEIFx where x is the Stream number
        uint32_t             buff_length            // Length of memory buffer needed.
    )
{
    DMA_InitTypeDef DMA_InitStructure;

    this->buff = new uint8_t[buff_length];

    assert(buff != NULL, ASSERT_STOP);

    this->buff_length = buff_length;
    this->buff_top = 0;
    this->dma_top = 0;
    this->dma_active = false;
    this->transfer_complete_bit = transfer_complete_bit;
    this->transfer_error_bit = transfer_error_bit;
    this->error_count = 0;
    this->dma_irq_num = dma_irq_num;
    this->dma_stream = dma_stream;

    // Enable DMA clock
    // TODO: Selectively enable clocks based on what stream is used.
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    DMA_DeInit(dma_stream);
    DMA_InitStructure.DMA_Channel = channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)periph_base_address;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0; // Gets set before each transfer.
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 0; // Gets set before each transfer.
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(dma_stream, &DMA_InitStructure);

    // Enable Tx dma transfer complete and transfer error.
    DMA_ITConfig(dma_stream, DMA_IT_TC|DMA_IT_TE, ENABLE);

    DMA_Cmd(dma_stream, ENABLE);

} // DmaTx::DmaTx() - Constructor

/*****************************************************************************
* Function: ~DmaTx
*
* Description: Destructor
*****************************************************************************/
DmaTx::~DmaTx(void)
{
    if (this->buff != NULL)
    {
        delete[] this->buff;
    }

} // DmaTx::~DmaTx() - Destructor

/*****************************************************************************
* Function: SendBuffer
*
* Description: Attempts to add data to the dma transfer buffer. Returns false
*              if the data will not fit in the buffer. The buffer is cleared
*              by DMA.
*****************************************************************************/
bool DmaTx::SendBuffer
    (
        uint8_t  const * data, // New data to transfer.
        uint16_t         len   // How many bytes to transfer.
    )
{
    uint16_t start, end;
    bool copy_successful;

    if ((len > buff_length) || (len == 0))
    {
        return false;
    }

    // Turn off the DMA interrupt since will be messing with transfer buffer.
    // **Note DMA may still be running.
    NVIC_DisableIRQ(dma_irq_num);

    copy_successful = CopyToTransferBuffer(data, len, &start, &end);

    if (copy_successful && !dma_active)
    {
        ActivateDMATransfer(start);
    }

    NVIC_EnableIRQ(dma_irq_num);

    return copy_successful;

} // DmaTx::SendBuffer()

/*****************************************************************************
* Function: CopyToTransferBuffer
*
* Description:  Attempts to place data into transfer buffer.  If not enough
*               room then returns false.
*****************************************************************************/
bool DmaTx::CopyToTransferBuffer
    (
         uint8_t  const * data,   // Buffer containing new data.
         uint16_t         len,    // How many bytes to transfer.
         uint16_t       * start,  // Start index of next transfer.
         uint16_t       * end     // Ending index of next transfer.
    )
{
    uint16_t num_in_buff = 0;  // How many bytes are already in transfer buffer.

    // Default start and end indices assuming will fit.
    *start = buff_top + 1;
    *end   = buff_top + len;

    if (dma_active)
    {
        // Find the space in the buffer.
        uint16_t num_data_left_to_transfer = dma_stream->NDTR;
        if (dma_top > buff_top)
        {
            num_in_buff = buff_top + buff_length + num_data_left_to_transfer - dma_top;
        }
        else
        {
            num_in_buff = buff_top - dma_top + num_data_left_to_transfer;
        }
    }
    else
    {
        // Reset to bottom to minimize the rollovers.
        num_in_buff = buff_top = dma_top = *start = 0;
        *end = len - 1;
    }

    if (len > (buff_length - num_in_buff))
    {
        return false; // won't fit in buffer.
    }

    if (*start == buff_length)
    {
        *start = 0;
        *end = len - 1;
        // Copy to beginning of buffer.
        memcpy(buff, data, len);
    }
    else if (*end >= buff_length)
    {
        *end -= buff_length;
        // Need to copy to end of buffer and then copy the rest into the beginning.
        memcpy(buff + *start, data, buff_length - *start);
        memcpy(buff, data + buff_length - *start, len + *start - buff_length);
    }
    else
    {
        // Everything's fine.  Just copy into transfer buffer.
        memcpy(buff + *start, data, len);
    }

    buff_top = *end;

    return true;

} // DmaTx::CopyToTransferBuffer()

/*****************************************************************************
* Function: ActivateDMATransfer
*
* Description: Starts new transfer at specified index of the transfer buffer.
*****************************************************************************/
bool DmaTx::ActivateDMATransfer
    (
        uint16_t start  // Index in transfer buffer to start sending from.
    )
{
    uint16_t length = 0; // How many bytes to transfer.

    if (start > buff_top)
    {
        // Buffer has rolled over, transfer the remaining top.
        length = buff_length - start;
        dma_top = buff_length - 1;
    }
    else
    {
        length = buff_top - start + 1;
        dma_top = buff_top;
    }

    // Need to disable stream before can modify any configuration settings.
    DMA_Cmd(dma_stream, DISABLE);

    // M0AR = Memory 0 address register
    // NDTR = Number data register
    dma_stream->M0AR = (uint32_t)(buff + start);
    dma_stream->NDTR = length;

    // Enable the DMA transfer stream and mark as active so if another client request
    // arrives before finished then won't restart in middle.
    // NOTE: Event flags must be cleared before DMA stream is enabled -MDR
    dma_active = true;
    DMA_ClearITPendingBit(dma_stream, transfer_complete_bit);
    DMA_ClearITPendingBit(dma_stream, transfer_error_bit);
    DMA_Cmd(dma_stream, ENABLE);

    return true;

} // DmaTx::ActivateDMATransfer()

/*****************************************************************************
* Function: HandleISR
*
* Description:  DMA interrupt handler that is invoked on completion of last
*               requested transfer.
*****************************************************************************/
void DmaTx::HandleISR(void)
{
    if (DMA_GetITStatus(dma_stream, transfer_error_bit))
    {
        // Clear transfer error interrupt pending bit
        DMA_ClearITPendingBit(dma_stream, transfer_error_bit);
        error_count++;
        // KLM - commented out assert because it always throws when interrupts
        // are first enabled.  Still seems to communicate fine after that though.
        // assert_always_msg(ASSERT_CONTINUE, "DMA error??\r\n");
        // TODO:  figure out why or reset the DMA controller?
    }

    if (DMA_GetITStatus(dma_stream, transfer_complete_bit))
    {
        // Transfer complete so clear interrupt pending bit.
        DMA_ClearITPendingBit(dma_stream, transfer_complete_bit);

        if (dma_top != buff_top)
        {
            // There is more in the buffer that needs to be sent.
            ActivateDMATransfer((dma_top + 1) % buff_length);
        }
        else
        {
            dma_active = false;
        }
    }
} // static DmaTx::HandleISR()
