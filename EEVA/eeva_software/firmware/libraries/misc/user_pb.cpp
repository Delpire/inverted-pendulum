//-------------------------------------------------------------------------
//  
//-------------------------------------------------------------------------
#include "user_pb.h"
#include "stm32f4xx.h"

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
UserPB::UserPB(void)
{
  // GPIOE Periph clock enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  // Configure the pin
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
 }

/*****************************************************************************
* Function:
*
* Description: 
* 
*****************************************************************************/
bool UserPB::read(void)
{
  return !(bool)(GPIOE->IDR & 0x01);
}
