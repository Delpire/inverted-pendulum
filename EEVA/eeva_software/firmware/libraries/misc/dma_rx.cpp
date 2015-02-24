/****************************************************************************************
* File: dma_rx.cpp - Direct Memory Access Receive Driver
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

#include "stm32f4xx.h"
#include "dma_rx.h"

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
* Function: DmaRx - Constructor
*
* Description:
*****************************************************************************/
DmaRx::DmaRx
    (
        DMA_Stream_TypeDef * dma_stream,  // DMAy_StreamX where y[1:2] and X[0:7]
        uint32_t             channel,     // Channel associated with stream.
        uint32_t             periph_base_address,  // Base data address of peripheral.
        uint32_t             buff_length           // Length of memory buffer needed.
    )
{
    DMA_InitTypeDef DMA_InitStructure;

    this->buff = new uint8_t[buff_length];
    // TODO: assert buff isn't null
    this->buff_length = buff_length;
    this->buff_bottom = 0;
    this->dma_stream = dma_stream;

    // Enable DMA clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    DMA_DeInit(dma_stream);
    DMA_InitStructure.DMA_Channel = channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)periph_base_address;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = buff_length;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(dma_stream, &DMA_InitStructure);

    DMA_Cmd(dma_stream, ENABLE);

} // DmaRx::DmaRx() - Constructor


/*****************************************************************************
* Function: ~DmaRx - Destructor
*
* Description:
*****************************************************************************/
DmaRx::~DmaRx(void)
{
    if (this->buff != NULL)
    {
        delete[] this->buff;
    }

} // DmaRx::~DmaRx() - Destructor


/*****************************************************************************
* Function: GetByte
*
* Description: Returns the next byte from the receive buffer.  Since DMA on rx
*              channel is setup in circular mode then will continue to receive
*              new data.  If there are no new bytes in buffer then returns false.
*****************************************************************************/
bool DmaRx::GetByte
    (
        uint8_t * byte  // Pointer to byte to be returned.
    )
{
    // NDTR = Num data items left to transfer register.
    uint32_t buff_top = buff_length - (uint16_t)dma_stream->NDTR;

    if (buff_top == buff_bottom)
    {
        return false; // no new data
    }

    *byte = buff[buff_bottom];

    // Increment and wrap bottom index for next time.
    buff_bottom = (buff_bottom + 1) % buff_length;

    return true;

} // DmaRx::GetByte()
