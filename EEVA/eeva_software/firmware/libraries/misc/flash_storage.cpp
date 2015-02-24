/****************************************************************************************
* File: 
*
* Description: 
*
* Created: 
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "flash_storage.h"
#include "stm32f4xx.h"

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
*                                   STATIC VARIABLES
*--------------------------------------------------------------------------------------*/

// This is basically a flash memory allocation, reserving the entire 16kB of flash sector 3
// for parameter storage.  The entire sector is used because the entire sector is erased
#pragma location = 0x800C000  
__no_init static volatile const uint32_t flash_data_initialized;
#pragma location = 0x800C004  
__no_init static volatile const uint32_t flash_data[4095];

/*---------------------------------------------------------------------------------------
*                                    CLASS METHODS
*--------------------------------------------------------------------------------------*/
/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
FlashStorage::FlashStorage()
{
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
void FlashStorage::Save(const glo_flash_params_t *params)
{
  uint8_t flash_status = FLASH_COMPLETE;
  uint32_t num_words = sizeof(glo_flash_params_t)/4;
  uint32_t initialization_code = 0x12345678;
  
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR |FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR); 
  flash_status = FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3);
  
  if (flash_status != FLASH_COMPLETE) 
  {
    FLASH_Lock();
    return;
  }
  
  // program the initialization marker
  uint32_t *address = (uint32_t *)&flash_data_initialized;
  flash_status = FLASH_ProgramWord((uint32_t)address, initialization_code);
  if (flash_status != FLASH_COMPLETE) 
  {
    FLASH_Lock();
    return;
  }

  // program the flash
  address = (uint32_t *)flash_data;
  uint32_t *data = (uint32_t *)params;
  for (uint32_t i = 0; i<num_words; i++)
  {
    flash_status = FLASH_ProgramWord((uint32_t)address, data[i]);
    address++;
    if (flash_status != FLASH_COMPLETE) 
    {
      FLASH_Lock();
      return;
    }
  }

  FLASH_Lock();
}
/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
void FlashStorage::Erase(void)
{
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR |FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR); 
  FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3);
  FLASH_Lock();
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
void FlashStorage::Read(glo_flash_params_t *params)
{
  uint32_t num_words = sizeof(glo_flash_params_t)/4;
  
  uint32_t *data = (uint32_t *)params;
  
  for (uint32_t i = 0; i<num_words; i++)
  {
    data[i] = flash_data[i];
  }
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
bool FlashStorage::IsInitialized(void)
{
  if (flash_data_initialized == 0x12345678)
    return true;
  else
    return false;
}
