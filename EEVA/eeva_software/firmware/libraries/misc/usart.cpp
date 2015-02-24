/****************************************************************************************
* File: usart.cpp - Universal [A]synchronous Receiver/Transmitter Driver
*
* Description: Provides USART implementation which organizes buses into instances which
*              can be referenced by calling the 'Instance' method.
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
#include "dma_tx.h"
#include "glo_os_references.h"
#include "usart.h"

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
*                                  STATIC CLASS FIELDS
*--------------------------------------------------------------------------------------*/

bool  Usart::init[USART_BUS_COUNT];
Usart Usart::objs[USART_BUS_COUNT];

/*---------------------------------------------------------------------------------------
*                                    CLASS METHODS
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: STATIC Instance
*
* Description: Returns the USART object associated with the bus.  Initializes
*              the bus if it has not been initialized already.
*****************************************************************************/
Usart * Usart::Instance
    (
        usart_bus_t bus // Which bus to get USART object for
    )
{
    assert_param(bus < USART_BUS_COUNT);

    if (init[bus])
    {
        return &objs[bus];
    }

    switch (bus)
    {
        case USART_BUS_1:
            InitBus1();
            objs[bus].bus = bus;
            objs[bus].USARTx = USART1;
            init[bus] = true;
            return (&objs[bus]);
            break;

        case USART_BUS_2:
            InitBus2();
            objs[bus].bus = bus;
            objs[bus].USARTx = USART2;
            init[bus] = true;
            return (&objs[bus]);
            break;
    }

    return NULL;

} // static Usart::Instance()


/*****************************************************************************
* Function: ~Usart - Destructor
*
* Description:
*****************************************************************************/
Usart::~Usart(void)
{
    if (dma_tx != NULL)
    {
        delete dma_tx;
    }

    if (dma_rx != NULL)
    {
        delete dma_rx;
    }

} // Usart::~Usart() - Destructor


/*****************************************************************************
* Function: GetByte
*
* Description: Returns the next byte from the receive buffer.  Since DMA on rx
*              channel is setup in circular mode then will continue to receive
*              new data.  If there are no new bytes in buffer then returns false.
*****************************************************************************/
bool Usart::GetByte
    (
        uint8_t * byte  // Pointer to byte to be returned.
    )
{
    return dma_rx->GetByte(byte);

} // Usart::GetByte()


/*****************************************************************************
* Function: SendBuffer
*
* Description: Delegates request to DMA transfer instance. Returns false if the
*              data will not fit in the buffer. The buffer is cleared by DMA.
*****************************************************************************/
bool Usart::SendBuffer
    (
        uint8_t  const * data, // New data to transfer.
        uint16_t         len   // How many bytes to transfer.
    )
{
    return dma_tx->SendBuffer(data, len);

} // Usart::SendBuffer()


/*****************************************************************************
* Function: set_baudrate
*
* Description: Sets baudrate for the USART bus.  Re-initializes the bus.
*****************************************************************************/
void Usart::set_baudrate( uint32_t baudrate )
{
    USART_InitTypeDef USART_InitStructure;
    // TODO: Only set baudrate
    // TODO: Check validity of baudrate
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // TODO: This is causes modes to change for RX only or TX only ports
    USART_Init(USARTx, &USART_InitStructure);
} // Usart::set_baudrate


/*****************************************************************************
* Function: STATIC InitBus1
*
* Description: Sets up USART and DMA peripherals for bus 1.
*****************************************************************************/
void Usart::InitBus1(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // Enable GPIO clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    // Enable USART clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // Initialize DMA Rx
    Usart * curr = &objs[USART_BUS_1];
    curr->dma_rx = new DmaRx(DMA2_Stream2,
                             DMA_Channel_4,
                             (uint32_t)&USART1->DR,
                             USART1_RX_BUFF_SIZE);
    curr->dma_tx = new DmaTx(DMA2_Stream7,
                             DMA_Channel_4,
                             DMA2_Stream7_IRQn,
                             (uint32_t)&USART1->DR,
                             DMA_IT_TCIF7,
                             DMA_IT_TEIF7,
                             USART1_TX_BUFF_SIZE);

    // TODO: assert dma instances aren't null

    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // Connect Rx to USART.
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
    // Connect Tx to USART.
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

    // Configure USART Tx/Rx as alternate function.
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

    USART_Cmd(USART1, ENABLE);

} // Usart:InitBus1


/*****************************************************************************
* Function: STATIC InitBus2
*
* Description: Sets up USART and DMA peripherals for bus 2.
*****************************************************************************/
void Usart::InitBus2(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // Enable GPIO clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    // Enable USART clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // Initialize DMA Rx and Tx
    Usart * curr = &objs[USART_BUS_2];
    curr->dma_rx = new DmaRx(DMA1_Stream5,
                             DMA_Channel_4,
                             (uint32_t)&USART2->DR,
                             USART2_RX_BUFF_SIZE);
    curr->dma_tx = new DmaTx(DMA1_Stream6,
                             DMA_Channel_4,
                             DMA1_Stream6_IRQn,
                             (uint32_t)&USART2->DR,
                             DMA_IT_TCIF6,
                             DMA_IT_TEIF6,
                             USART2_TX_BUFF_SIZE);

    // TODO: assert dma instances aren't null

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    // Connect Rx to USART.
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    // Connect Tx to USART.
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

    // Configure USART Tx/Rx as alternate function.
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

    USART_Cmd(USART2, ENABLE);

} // Usart:InitBus2


/*****************************************************************************
* Function: USART1_TX_ISR
*
* Description:  Delegate off to DMA handler.
*****************************************************************************/
void Usart::USART1_TX_ISR(void)
{
    objs[USART_BUS_1].dma_tx->HandleISR();

} // static Usart::USART1_TX_ISR()

/*****************************************************************************
* Function: USART2_TX_ISR
*
* Description:  Delegate off to DMA handler.
*****************************************************************************/
void Usart::USART2_TX_ISR(void)
{
    objs[USART_BUS_2].dma_tx->HandleISR();

} // static Usart::USART2_TX_ISR()

