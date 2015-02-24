/****************************************************************************************
* File: usart.h
*
* Description: Header file for usart.cpp
*
* Created:
****************************************************************************************/

#ifndef USART_INCLUDED_H
#define USART_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "dma_rx.h"
#include "dma_tx.h"

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

// USART bus select enumeration
typedef uint8_t usart_bus_t;
enum
{
    USART_BUS_1 = 0,
    USART_BUS_2,
    USART_BUS_COUNT
};

// Transfer and receive buffer sizes for each bus
enum
{
    USART1_TX_BUFF_SIZE = 256,
    USART1_RX_BUFF_SIZE = 50,
    USART2_TX_BUFF_SIZE = 1024,
    USART2_RX_BUFF_SIZE = 250,
};

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/

class Usart
{
public: // methods

    // Returns the USART object for the given bus.  The object will be initialized if
    // it has not already been initialized.
    static Usart * Instance
    (
        usart_bus_t bus // Which bus to get USART object for
    );

    // Destructor
    ~Usart(void);

    // Get the next available byte from the usart receive buffer. Returns false if
    // nothing available.  Should be called at high enough rate that the circular
    // buffer is not overwritten by the rx DMA between clearing it.
    bool GetByte
        (
            uint8_t * byte // Pointer to byte to be returned.
        );

    // Copy array at 'data' to the usart send buffer.  Returns false if not enough
    // room in the buffer.  The buffer is cleared by dma transfers set up in the
    // tx DMA ISR.
    bool SendBuffer
        (
            uint8_t  const * data, // New data to transfer.
            uint16_t         len   // How many bytes to transfer.
        );

    void set_baudrate
        (
            uint32_t baud // Baud rate to set
        );

    static void USART1_TX_ISR(void);
    static void USART2_TX_ISR(void);

private: // methods

    Usart() {} // private constructor to limit instances to 'factory' method
    
    static void InitBus1(void);
    static void InitBus2(void);

private: // fields

    static bool  init[USART_BUS_COUNT];
    static Usart objs[USART_BUS_COUNT];

    DmaRx * dma_rx;
    DmaTx * dma_tx;

    usart_bus_t bus;
    USART_TypeDef* USARTx;

}; // Usart

#endif  // USART_INCLUDED_H
