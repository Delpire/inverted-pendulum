/****************************************************************************************
* File: glo_objects.cpp
*
* Description: Defines glo objects that are declared in glo_objects.h and provides
*              function for registering all 'write' callbacks.  There is actually nothing
*              that must be called here to use glo objects unless a callback needs to be
*              registered. Including this file in the project will declare all the glo
*              objects.
*
* Created:
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_write_callbacks.h"
#include "glo_types.h"

/*---------------------------------------------------------------------------------------
*                                      VARIABLES
*--------------------------------------------------------------------------------------*/

// Automatically define glo objects without double maintenance.
#define DEFINE_GLO_OBJECTS
#include "glo_objects.h"
#undef DEFINE_GLO_OBJECTS

// Array of glo objects meta data.
GloObjectBase * gloObjectPtrs[NUM_GLO_OBJECTS];

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: register_glo_object_callbacks
*
* Description: Registers 'write/publish' callbacks with individual glo objects.
*****************************************************************************/
void register_glo_object_callbacks(void)
{
    glo_driving_command.register_callback(driving_command_callback);
    glo_flash_params.register_callback(flash_params_callback);
} 
