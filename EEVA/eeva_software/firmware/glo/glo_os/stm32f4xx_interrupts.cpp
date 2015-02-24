/****************************************************************************************
* File: sm324xx_interrupts.cpp
*
* Description: Defines interrupt vectors for GloOS. All IRQn's are vectored to the GloOS
*              dispatcher. This file should define all possible IRQn's for the processor.
*              It should mirror "typedef enum IRQn" in stm32f4xx.h.
*
* Created:
****************************************************************************************/

#include "stm32f4xx.h"
#include "glo_os.h"

extern GloOS *glo_os;

//******************************************************************************
//            Cortex-M4 Processor Exception Handlers
//******************************************************************************
extern "C" void NMI_Handler(void){}
extern "C" void HardFault_Handler(void)
{
  while (1);  // Go to infinite while loop
}
extern "C" void MemManage_Handler(void)
{
  while (1);  // Go to infinite while loop
}
extern "C" void BusFault_Handler(void)
{
  while (1);  // Go to infinite while loop
}
extern "C" void UsageFault_Handler(void)
{
  while (1);  // Go to infinite while loop
}
extern "C" void SVC_Handler(void){}
extern "C" void DebugMon_Handler(void){}
extern "C" void PendSV_Handler(void){}
extern "C" void SysTick_Handler(void)
{
  glo_os->dispatcher(SysTick_IRQn);
}
//******************************************************************************
//            IRQ handlers for all STM32F4 Processors
//******************************************************************************
extern "C" void WWDG_IRQHandler(void)
{
  glo_os->dispatcher(WWDG_IRQn);
}
extern "C" void PVD_IRQHandler(void)
{
  glo_os->dispatcher(PVD_IRQn);
}
extern "C" void TAMP_STAMP_IRQHandler(void)
{
  glo_os->dispatcher(TAMP_STAMP_IRQn);
}
extern "C" void RTC_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(RTC_WKUP_IRQn);
}
extern "C" void FLASH_IRQHandler(void)
{
  glo_os->dispatcher(FLASH_IRQn);
}
extern "C" void RCC_IRQHandler(void)
{
  glo_os->dispatcher(RCC_IRQn);
}
extern "C" void EXTI0_IRQHandler(void)
{
  glo_os->dispatcher(EXTI0_IRQn);
}
extern "C" void EXTI1_IRQHandler(void)
{
  glo_os->dispatcher(EXTI1_IRQn);
}
extern "C" void EXTI2_IRQHandler(void)
{
  glo_os->dispatcher(EXTI2_IRQn);
}
extern "C" void EXTI3_IRQHandler(void)
{
  glo_os->dispatcher(EXTI3_IRQn);
}
extern "C" void EXTI4_IRQHandler(void)
{
  glo_os->dispatcher(EXTI4_IRQn);
}
extern "C" void DMA1_Stream0_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream0_IRQn);
}
extern "C" void DMA1_Stream1_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream1_IRQn);
}
extern "C" void DMA1_Stream2_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream2_IRQn);
}
extern "C" void DMA1_Stream3_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream3_IRQn);
}
extern "C" void DMA1_Stream4_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream4_IRQn);
}
extern "C" void DMA1_Stream5_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream5_IRQn);
}
extern "C" void DMA1_Stream6_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream6_IRQn);
}
extern "C" void ADC_IRQHandler(void)
{
  glo_os->dispatcher(ADC_IRQn);
}

//******************************************************************************
//            IRQ handlers for specific processors
//******************************************************************************

#if defined (STM32F40_41xxx)
extern "C" void CAN1_TX_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_TX_IRQn);
}
extern "C" void CAN1_RX0_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_RX0_IRQn);
}
extern "C" void CAN1_RX1_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_RX1_IRQn);
}
extern "C" void CAN1_SCE_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_SCE_IRQn);
}
extern "C" void EXTI9_5_IRQHandler(void)
{
  glo_os->dispatcher(EXTI9_5_IRQn);
}
extern "C" void TIM1_BRK_TIM9_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_BRK_TIM9_IRQn);
}
extern "C" void TIM1_UP_TIM10_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_UP_TIM10_IRQn);
}
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_TRG_COM_TIM11_IRQn);
}
extern "C" void TIM1_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_CC_IRQn);
}
extern "C" void TIM2_IRQHandler(void)
{
  glo_os->dispatcher(TIM2_IRQn);
}
extern "C" void TIM3_IRQHandler(void)
{
  glo_os->dispatcher(TIM3_IRQn);
}
extern "C" void TIM4_IRQHandler(void)
{
  glo_os->dispatcher(TIM4_IRQn);
}
extern "C" void I2C1_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_EV_IRQn);
}
extern "C" void I2C1_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_ER_IRQn);
}
extern "C" void I2C2_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_EV_IRQn);
}
extern "C" void I2C2_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_ER_IRQn);
}
extern "C" void SPI1_IRQHandler(void)
{
  glo_os->dispatcher(SPI1_IRQn);
}
extern "C" void SPI2_IRQHandler(void)
{
  glo_os->dispatcher(SPI2_IRQn);
}
extern "C" void USART1_IRQHandler(void)
{
  glo_os->dispatcher(USART1_IRQn);
}
extern "C" void USART2_IRQHandler(void)
{
  glo_os->dispatcher(USART2_IRQn);
}
extern "C" void USART3_IRQHandler(void)
{
  glo_os->dispatcher(USART3_IRQn);
}
extern "C" void EXTI15_10_IRQHandler(void)
{
  glo_os->dispatcher(EXTI15_10_IRQn);
}
extern "C" void RTC_Alarm_IRQHandler(void)
{
  glo_os->dispatcher(RTC_Alarm_IRQn);
}
extern "C" void OTG_FS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_WKUP_IRQn);
}
extern "C" void TIM8_BRK_TIM12_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_BRK_TIM12_IRQn);
}
extern "C" void TIM8_UP_TIM13_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_UP_TIM13_IRQn);
}
extern "C" void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_TRG_COM_TIM14_IRQn);
}
extern "C" void TIM8_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_CC_IRQn);
}
extern "C" void DMA1_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream7_IRQn);
}
extern "C" void FSMC_IRQHandler(void)
{
  glo_os->dispatcher(FSMC_IRQn);
}
extern "C" void SDIO_IRQHandler(void)
{
  glo_os->dispatcher(SDIO_IRQn);
}
extern "C" void TIM5_IRQHandler(void)
{
  glo_os->dispatcher(TIM5_IRQn);
}
extern "C" void SPI3_IRQHandler(void)
{
  glo_os->dispatcher(SPI3_IRQn);
}
extern "C" void UART4_IRQHandler(void)
{
  glo_os->dispatcher(UART4_IRQn);
}
extern "C" void UART5_IRQHandler(void)
{
  glo_os->dispatcher(UART5_IRQn);
}
extern "C" void TIM6_DAC_IRQHandler(void)
{
  glo_os->dispatcher(TIM6_DAC_IRQn);
}
extern "C" void TIM7_IRQHandler(void)
{
  glo_os->dispatcher(TIM7_IRQn);
}
extern "C" void DMA2_Stream0_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream0_IRQn);
}
extern "C" void DMA2_Stream1_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream1_IRQn);
}
extern "C" void DMA2_Stream2_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream2_IRQn);
}
extern "C" void DMA2_Stream3_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream3_IRQn);
}
extern "C" void DMA2_Stream4_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream4_IRQn);
}
extern "C" void ETH_IRQHandler(void)
{
  glo_os->dispatcher(ETH_IRQn);
}
extern "C" void ETH_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(ETH_WKUP_IRQn);
}
extern "C" void CAN2_TX_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_TX_IRQn);
}
extern "C" void CAN2_RX0_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_RX0_IRQn);
}
extern "C" void CAN2_RX1_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_RX1_IRQn);
}
extern "C" void CAN2_SCE_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_SCE_IRQn);
}
extern "C" void OTG_FS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_IRQn);
}
extern "C" void DMA2_Stream5_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream5_IRQn);
}
extern "C" void DMA2_Stream6_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream6_IRQn);
}
extern "C" void DMA2_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream7_IRQn);
}
extern "C" void USART6_IRQHandler(void)
{
  glo_os->dispatcher(USART6_IRQn);
}
extern "C" void I2C3_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_EV_IRQn);
}
extern "C" void I2C3_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_ER_IRQn);
}
extern "C" void OTG_HS_EP1_OUT_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_EP1_OUT_IRQn);
}
extern "C" void OTG_HS_EP1_IN_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_EP1_IN_IRQn);
}
extern "C" void OTG_HS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_WKUP_IRQn);
}
extern "C" void OTG_HS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_IRQn);
}
extern "C" void DCMI_IRQHandler(void)
{
  glo_os->dispatcher(DCMI_IRQn);
}
extern "C" void CRYP_IRQHandler(void)
{
  glo_os->dispatcher(CRYP_IRQn);
}
extern "C" void HASH_RNG_IRQHandler(void)
{
  glo_os->dispatcher(HASH_RNG_IRQn);
}
extern "C" void FPU_IRQHandler(void)
{
  glo_os->dispatcher(FPU_IRQn);
}
#endif /* STM32F40_41xxx */

//******************************************************************

#if defined (STM32F427_437xx)
extern "C" void CAN1_TX_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_TX_IRQn);
}
extern "C" void CAN1_RX0_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_RX0_IRQn);
}
extern "C" void CAN1_RX1_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_RX1_IRQn);
}
extern "C" void CAN1_SCE_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_SCE_IRQn);
}
extern "C" void EXTI9_5_IRQHandler(void)
{
  glo_os->dispatcher(EXTI9_5_IRQn);
}
extern "C" void TIM1_BRK_TIM9_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_BRK_TIM9_IRQn);
}
extern "C" void TIM1_UP_TIM10_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_UP_TIM10_IRQn);
}
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_TRG_COM_TIM11_IRQn);
}
extern "C" void TIM1_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_CC_IRQn);
}
extern "C" void TIM2_IRQHandler(void)
{
  glo_os->dispatcher(TIM2_IRQn);
}
extern "C" void TIM3_IRQHandler(void)
{
  glo_os->dispatcher(TIM3_IRQn);
}
extern "C" void TIM4_IRQHandler(void)
{
  glo_os->dispatcher(TIM4_IRQn);
}
extern "C" void I2C1_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_EV_IRQn);
}
extern "C" void I2C1_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_ER_IRQn);
}
extern "C" void I2C2_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_EV_IRQn);
}
extern "C" void I2C2_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_ER_IRQn);
}
extern "C" void SPI1_IRQHandler(void)
{
  glo_os->dispatcher(SPI1_IRQn);
}
extern "C" void SPI2_IRQHandler(void)
{
  glo_os->dispatcher(SPI2_IRQn);
}
extern "C" void USART1_IRQHandler(void)
{
  glo_os->dispatcher(USART1_IRQn);
}
extern "C" void USART2_IRQHandler(void)
{
  glo_os->dispatcher(USART2_IRQn);
}
extern "C" void USART3_IRQHandler(void)
{
  glo_os->dispatcher(USART3_IRQn);
}
extern "C" void EXTI15_10_IRQHandler(void)
{
  glo_os->dispatcher(EXTI15_10_IRQn);
}
extern "C" void RTC_Alarm_IRQHandler(void)
{
  glo_os->dispatcher(RTC_Alarm_IRQn);
}
extern "C" void OTG_FS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_WKUP_IRQn);
}
extern "C" void TIM8_BRK_TIM12_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_BRK_TIM12_IRQn);
}
extern "C" void TIM8_UP_TIM13_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_UP_TIM13_IRQn);
}
extern "C" void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_TRG_COM_TIM14_IRQn);
}
extern "C" void TIM8_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_CC_IRQn);
}
extern "C" void DMA1_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream7_IRQn);
}
extern "C" void FMC_IRQHandler(void)
{
  glo_os->dispatcher(FMC_IRQn);
}
extern "C" void SDIO_IRQHandler(void)
{
  glo_os->dispatcher(SDIO_IRQn);
}
extern "C" void TIM5_IRQHandler(void)
{
  glo_os->dispatcher(TIM5_IRQn);
}
extern "C" void SPI3_IRQHandler(void)
{
  glo_os->dispatcher(SPI3_IRQn);
}
extern "C" void UART4_IRQHandler(void)
{
  glo_os->dispatcher(UART4_IRQn);
}
extern "C" void UART5_IRQHandler(void)
{
  glo_os->dispatcher(UART5_IRQn);
}
extern "C" void TIM6_DAC_IRQHandler(void)
{
  glo_os->dispatcher(TIM6_DAC_IRQn);
}
extern "C" void TIM7_IRQHandler(void)
{
  glo_os->dispatcher(TIM7_IRQn);
}
extern "C" void DMA2_Stream0_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream0_IRQn);
}
extern "C" void DMA2_Stream1_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream1_IRQn);
}
extern "C" void DMA2_Stream2_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream2_IRQn);
}
extern "C" void DMA2_Stream3_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream3_IRQn);
}
extern "C" void DMA2_Stream4_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream4_IRQn);
}
extern "C" void ETH_IRQHandler(void)
{
  glo_os->dispatcher(ETH_IRQn);
}
extern "C" void ETH_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(ETH_WKUP_IRQn);
}
extern "C" void CAN2_TX_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_TX_IRQn);
}
extern "C" void CAN2_RX0_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_RX0_IRQn);
}
extern "C" void CAN2_RX1_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_RX1_IRQn);
}
extern "C" void CAN2_SCE_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_SCE_IRQn);
}
extern "C" void OTG_FS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_IRQn);
}
extern "C" void DMA2_Stream5_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream5_IRQn);
}
extern "C" void DMA2_Stream6_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream6_IRQn);
}
extern "C" void DMA2_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream7_IRQn);
}
extern "C" void USART6_IRQHandler(void)
{
  glo_os->dispatcher(USART6_IRQn);
}
extern "C" void I2C3_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_EV_IRQn);
}
extern "C" void I2C3_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_ER_IRQn);
}
extern "C" void OTG_HS_EP1_OUT_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_EP1_OUT_IRQn);
}
extern "C" void OTG_HS_EP1_IN_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_EP1_IN_IRQn);
}
extern "C" void OTG_HS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_WKUP_IRQn);
}
extern "C" void OTG_HS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_IRQn);
}
extern "C" void DCMI_IRQHandler(void)
{
  glo_os->dispatcher(DCMI_IRQn);
}
extern "C" void CRYP_IRQHandler(void)
{
  glo_os->dispatcher(CRYP_IRQn);
}
extern "C" void HASH_RNG_IRQHandler(void)
{
  glo_os->dispatcher(HASH_RNG_IRQn);
}
extern "C" void FPU_IRQHandler(void)
{
  glo_os->dispatcher(FPU_IRQn);
}
extern "C" void UART7_IRQHandler(void)
{
  glo_os->dispatcher(UART7_IRQn);
}
extern "C" void UART8_IRQHandler(void)
{
  glo_os->dispatcher(UART8_IRQn);
}
extern "C" void SPI4_IRQHandler(void)
{
  glo_os->dispatcher(SPI4_IRQn);
}
extern "C" void SPI5_IRQHandler(void)
{
  glo_os->dispatcher(SPI5_IRQn);
}
extern "C" void SPI6_IRQHandler(void)
{
  glo_os->dispatcher(SPI6_IRQn);
}
extern "C" void SAI1_IRQHandler(void)
{
  glo_os->dispatcher(SAI1_IRQn);
}
extern "C" void DMA2D_IRQHandler(void)
{
  glo_os->dispatcher(DMA2D_IRQn);
}
#endif /* STM32F427_437xx */

//******************************************************************

#if defined (STM32F429_439xx)
extern "C" void CAN1_TX_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_TX_IRQn);
}
extern "C" void CAN1_RX0_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_RX0_IRQn);
}
extern "C" void CAN1_RX1_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_RX1_IRQn);
}
extern "C" void CAN1_SCE_IRQHandler(void)
{
  glo_os->dispatcher(CAN1_SCE_IRQn);
}
extern "C" void EXTI9_5_IRQHandler(void)
{
  glo_os->dispatcher(EXTI9_5_IRQn);
}
extern "C" void TIM1_BRK_TIM9_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_BRK_TIM9_IRQn);
}
extern "C" void TIM1_UP_TIM10_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_UP_TIM10_IRQn);
}
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_TRG_COM_TIM11_IRQn);
}
extern "C" void TIM1_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_CC_IRQn);
}
extern "C" void TIM2_IRQHandler(void)
{
  glo_os->dispatcher(TIM2_IRQn);
}
extern "C" void TIM3_IRQHandler(void)
{
  glo_os->dispatcher(TIM3_IRQn);
}
extern "C" void TIM4_IRQHandler(void)
{
  glo_os->dispatcher(TIM4_IRQn);
}
extern "C" void I2C1_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_EV_IRQn);
}
extern "C" void I2C1_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_ER_IRQn);
}
extern "C" void I2C2_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_EV_IRQn);
}
extern "C" void I2C2_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_ER_IRQn);
}
extern "C" void SPI1_IRQHandler(void)
{
  glo_os->dispatcher(SPI1_IRQn);
}
extern "C" void SPI2_IRQHandler(void)
{
  glo_os->dispatcher(SPI2_IRQn);
}
extern "C" void USART1_IRQHandler(void)
{
  glo_os->dispatcher(USART1_IRQn);
}
extern "C" void USART2_IRQHandler(void)
{
  glo_os->dispatcher(USART2_IRQn);
}
extern "C" void USART3_IRQHandler(void)
{
  glo_os->dispatcher(USART3_IRQn);
}
extern "C" void EXTI15_10_IRQHandler(void)
{
  glo_os->dispatcher(EXTI15_10_IRQn);
}
extern "C" void RTC_Alarm_IRQHandler(void)
{
  glo_os->dispatcher(RTC_Alarm_IRQn);
}
extern "C" void OTG_FS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_WKUP_IRQn);
}
extern "C" void TIM8_BRK_TIM12_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_BRK_TIM12_IRQn);
}
extern "C" void TIM8_UP_TIM13_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_UP_TIM13_IRQn);
}
extern "C" void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_TRG_COM_TIM14_IRQn);
}
extern "C" void TIM8_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM8_CC_IRQn);
}
extern "C" void DMA1_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream7_IRQn);
}
extern "C" void FMC_IRQHandler(void)
{
  glo_os->dispatcher(FMC_IRQn);
}
extern "C" void SDIO_IRQHandler(void)
{
  glo_os->dispatcher(SDIO_IRQn);
}
extern "C" void TIM5_IRQHandler(void)
{
  glo_os->dispatcher(TIM5_IRQn);
}
extern "C" void SPI3_IRQHandler(void)
{
  glo_os->dispatcher(SPI3_IRQn);
}
extern "C" void UART4_IRQHandler(void)
{
  glo_os->dispatcher(UART4_IRQn);
}
extern "C" void UART5_IRQHandler(void)
{
  glo_os->dispatcher(UART5_IRQn);
}
extern "C" void TIM6_DAC_IRQHandler(void)
{
  glo_os->dispatcher(TIM6_DAC_IRQn);
}
extern "C" void TIM7_IRQHandler(void)
{
  glo_os->dispatcher(TIM7_IRQn);
}
extern "C" void DMA2_Stream0_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream0_IRQn);
}
extern "C" void DMA2_Stream1_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream1_IRQn);
}
extern "C" void DMA2_Stream2_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream2_IRQn);
}
extern "C" void DMA2_Stream3_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream3_IRQn);
}
extern "C" void DMA2_Stream4_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream4_IRQn);
}
extern "C" void ETH_IRQHandler(void)
{
  glo_os->dispatcher(ETH_IRQn);
}
extern "C" void ETH_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(ETH_WKUP_IRQn);
}
extern "C" void CAN2_TX_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_TX_IRQn);
}
extern "C" void CAN2_RX0_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_RX0_IRQn);
}
extern "C" void CAN2_RX1_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_RX1_IRQn);
}
extern "C" void CAN2_SCE_IRQHandler(void)
{
  glo_os->dispatcher(CAN2_SCE_IRQn);
}
extern "C" void OTG_FS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_IRQn);
}
extern "C" void DMA2_Stream5_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream5_IRQn);
}
extern "C" void DMA2_Stream6_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream6_IRQn);
}
extern "C" void DMA2_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream7_IRQn);
}
extern "C" void USART6_IRQHandler(void)
{
  glo_os->dispatcher(USART6_IRQn);
}
extern "C" void I2C3_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_EV_IRQn);
}
extern "C" void I2C3_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_ER_IRQn);
}
extern "C" void OTG_HS_EP1_OUT_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_EP1_OUT_IRQn);
}
extern "C" void OTG_HS_EP1_IN_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_EP1_IN_IRQn);
}
extern "C" void OTG_HS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_WKUP_IRQn);
}
extern "C" void OTG_HS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_HS_IRQn);
}
extern "C" void DCMI_IRQHandler(void)
{
  glo_os->dispatcher(DCMI_IRQn);
}
extern "C" void CRYP_IRQHandler(void)
{
  glo_os->dispatcher(CRYP_IRQn);
}
extern "C" void HASH_RNG_IRQHandler(void)
{
  glo_os->dispatcher(HASH_RNG_IRQn);
}
extern "C" void FPU_IRQHandler(void)
{
  glo_os->dispatcher(FPU_IRQn);
}
extern "C" void UART7_IRQHandler(void)
{
  glo_os->dispatcher(UART7_IRQn);
}
extern "C" void UART8_IRQHandler(void)
{
  glo_os->dispatcher(UART8_IRQn);
}
extern "C" void SPI4_IRQHandler(void)
{
  glo_os->dispatcher(SPI4_IRQn);
}
extern "C" void SPI5_IRQHandler(void)
{
  glo_os->dispatcher(SPI5_IRQn);
}
extern "C" void SPI6_IRQHandler(void)
{
  glo_os->dispatcher(SPI6_IRQn);
}
extern "C" void SAI1_IRQHandler(void)
{
  glo_os->dispatcher(SAI1_IRQn);
}
extern "C" void LTDC_IRQHandler(void)
{
  glo_os->dispatcher(LTDC_IRQn);
}
extern "C" void LTDC_ER_IRQHandler(void)
{
  glo_os->dispatcher(LTDC_ER_IRQn);
}
extern "C" void DMA2D_IRQHandler(void)
{
  glo_os->dispatcher(DMA2D_IRQn);
}
#endif /* STM32F429_439xx */

//******************************************************************

#if defined (STM32F401xx) || defined (STM32F411xE)
extern "C" void EXTI9_5_IRQHandler(void)
{
  glo_os->dispatcher(EXTI9_5_IRQn);
}
extern "C" void TIM1_BRK_TIM9_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_BRK_TIM9_IRQn);
}
extern "C" void TIM1_UP_TIM10_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_UP_TIM10_IRQn);
}
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_TRG_COM_TIM11_IRQn);
}
extern "C" void TIM1_CC_IRQHandler(void)
{
  glo_os->dispatcher(TIM1_CC_IRQn);
}
extern "C" void TIM2_IRQHandler(void)
{
  glo_os->dispatcher(TIM2_IRQn);
}
extern "C" void TIM3_IRQHandler(void)
{
  glo_os->dispatcher(TIM3_IRQn);
}
extern "C" void TIM4_IRQHandler(void)
{
  glo_os->dispatcher(TIM4_IRQn);
}
extern "C" void I2C1_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_EV_IRQn);
}
extern "C" void I2C1_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C1_ER_IRQn);
}
extern "C" void I2C2_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_EV_IRQn);
}
extern "C" void I2C2_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C2_ER_IRQn);
}
extern "C" void SPI1_IRQHandler(void)
{
  glo_os->dispatcher(SPI1_IRQn);
}
extern "C" void SPI2_IRQHandler(void)
{
  glo_os->dispatcher(SPI2_IRQn);
}
extern "C" void USART1_IRQHandler(void)
{
  glo_os->dispatcher(USART1_IRQn);
}
extern "C" void USART2_IRQHandler(void)
{
  glo_os->dispatcher(USART2_IRQn);
}
extern "C" void EXTI15_10_IRQHandler(void)
{
  glo_os->dispatcher(EXTI15_10_IRQn);
}
extern "C" void RTC_Alarm_IRQHandler(void)
{
  glo_os->dispatcher(RTC_Alarm_IRQn);
}
extern "C" void OTG_FS_WKUP_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_WKUP_IRQn);
}
extern "C" void DMA1_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA1_Stream7_IRQn);
}
extern "C" void SDIO_IRQHandler(void)
{
  glo_os->dispatcher(SDIO_IRQn);
}
extern "C" void TIM5_IRQHandler(void)
{
  glo_os->dispatcher(TIM5_IRQn);
}
extern "C" void SPI3_IRQHandler(void)
{
  glo_os->dispatcher(SPI3_IRQn);
}
extern "C" void DMA2_Stream0_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream0_IRQn);
}
extern "C" void DMA2_Stream1_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream1_IRQn);
}
extern "C" void DMA2_Stream2_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream2_IRQn);
}
extern "C" void DMA2_Stream3_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream3_IRQn);
}
extern "C" void DMA2_Stream4_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream4_IRQn);
}
extern "C" void OTG_FS_IRQHandler(void)
{
  glo_os->dispatcher(OTG_FS_IRQn);
}
extern "C" void DMA2_Stream5_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream5_IRQn);
}
extern "C" void DMA2_Stream6_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream6_IRQn);
}
extern "C" void DMA2_Stream7_IRQHandler(void)
{
  glo_os->dispatcher(DMA2_Stream7_IRQn);
}
extern "C" void USART6_IRQHandler(void)
{
  glo_os->dispatcher(USART6_IRQn);
}
extern "C" void I2C3_EV_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_EV_IRQn);
}
extern "C" void I2C3_ER_IRQHandler(void)
{
  glo_os->dispatcher(I2C3_ER_IRQn);
}
extern "C" void FPU_IRQHandler(void)
{
  glo_os->dispatcher(FPU_IRQn);
}
extern "C" void SPI4_IRQHandler(void)
{
  glo_os->dispatcher(SPI4_IRQn);
}

#if defined (STM32F411xE)
extern "C" void SPI5_IRQHandler(void)
{
  glo_os->dispatcher(SPI5_IRQn);
}
#endif /* STM32F411xE */

#endif /* STM32F401xx || STM32F411xE */
