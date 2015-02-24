/****************************************************************************************
* File: glo_object_template.h
*
* Description: Defines a base class for glo objects and defines a template class for 
*              glo objects. The template class does a class for multi-instance glo
*              objects and then a specialization for single instance objects.
*
* Created:
****************************************************************************************/

#ifndef GLO_OBJECT_TEMPLATE_H
#define GLO_OBJECT_TEMPLATE_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

#include "glo_types.h"
#include "glo_os.h"
#include "glo_os_references.h"
#include "stm32f4xx.h"

struct gloMetaDataType{
  const uint8_t id;
  const uint8_t numBytes;
  const uint16_t numInstance;
  const void *dataPtr;
  double timeStamp;

  // constructor
  gloMetaDataType(uint8_t id, uint8_t numBytes, uint16_t numInstance, void *dataPtr):
     id(id), numBytes(numBytes), numInstance(numInstance), dataPtr(dataPtr), timeStamp(0) {}
};

/*****************************************************************************
* Class: GloObjectBase
*
* Description: Used to give common meta data and functionality to all glo objects
*****************************************************************************/

class GloObjectBase
{
  public:
    // constructor
    GloObjectBase(uint8_t id, uint8_t numBytes, uint16_t numInstance, void *dataPtr):
      md(id,numBytes,numInstance, dataPtr) {};

    uint8_t  get_id(void) const { return md.id; }

    uint16_t get_num_instances(void) const { return md.numInstance; }

    uint8_t get_num_bytes(void) const { return md.numBytes; }
    
    const void * get_data_pointer(void) const { return md.dataPtr; }

    double get_timestamp(void) const
    {
        double curr_timestamp;
        uint32_t disabled = __get_PRIMASK();
        __disable_irq();
        curr_timestamp = md.timeStamp;
        if (!disabled)
            __enable_irq();
        return curr_timestamp;
    }

    // *timestamp is both and input and output
    // if *timestamp doesn't match the objects then returns true and 
    // sets *timestamp to the objects timestamp
    bool is_new(double *timestamp) const  
    {                                     
        bool is_new;
        double curr_timestamp;
        uint32_t disabled = __get_PRIMASK();
        __disable_irq();                     
        curr_timestamp = md.timeStamp;
        if (!disabled)
            __enable_irq();
        is_new = (*timestamp < curr_timestamp);
        *timestamp = curr_timestamp;
        return is_new;
    }

  protected:
    gloMetaDataType md;

};

/*---------------------------------------------------------------------------------------
*                                      VARIABLES
*--------------------------------------------------------------------------------------*/
extern GloObjectBase * gloObjectPtrs[];

/*****************************************************************************
* Template Class: GloObjectTemplate
*
* Description: Builds a glo object with and array (multi-instance) of data based
*              on the data type.
*****************************************************************************/
template <typename objectDataType,uint16_t numInstance>
class GloObjectTemplate : public GloObjectBase
{
  public:
    //constructor
    GloObjectTemplate(uint8_t id);

    //destructor
    virtual ~GloObjectTemplate();

    //methods
    // returns -1 if instance invalid and timestamp if valid
    double read(objectDataType *copy, uint16_t instance); 
    uint8_t publish(objectDataType *dataStruct, uint16_t instance);

    // Function pointer type to publish callback function.
    typedef bool (*write_callback_t)(objectDataType &, uint16_t);

    // Register a function to be called everytime new data is published.
    void register_callback(write_callback_t callback) { this->callback = callback; }

  private:

    //properties -- note (md) metadata is inherited from GloObjectBase
    objectDataType data[numInstance];

    // Function pointer to function to call when publishing new data.
    write_callback_t callback;

};

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

// Constructor
template <typename objectDataType,uint16_t numInstance>
GloObjectTemplate<objectDataType,numInstance>::
GloObjectTemplate(uint8_t id):
GloObjectBase(id,sizeof(objectDataType), numInstance,(void *)&(data))
{
  gloObjectPtrs[id] = this;
  callback = NULL;
}

// Destructor
template <typename objectDataType,uint16_t numInstance>
GloObjectTemplate<objectDataType,numInstance>::~GloObjectTemplate()
{
}

// returns -1 if instance invalid and timestamp if valid
template <typename objectDataType,uint16_t numInstance>
double GloObjectTemplate<objectDataType,numInstance>::read(objectDataType *copy, uint16_t instance)
{
  double time_stamp;
  
  if(instance > (md.numInstance-1))
     return(-1.0);

  uint32_t disabled = __get_PRIMASK();   // note: if disabled then PRIMASK should
                                         // not change after this read
  __disable_irq();  // disable all interrupts
  memcpy( (void *)copy, (void const *)&(data[instance-1]), md.numBytes);
  time_stamp = md.timeStamp;
  if (!disabled)
    __enable_irq(); //  enable interrupts
  return(time_stamp);
}

template <typename objectDataType,uint16_t numInstance>
uint8_t GloObjectTemplate<objectDataType,numInstance>::publish(objectDataType *dataStruct, uint16_t instance)
{
  uint32_t instancePtr;
  if((instance > md.numInstance)||(instance == 0))
     return(0);

  instancePtr = (uint32_t)md.dataPtr + (uint32_t)(instance-1)*md.numBytes;

  uint32_t disabled = __get_PRIMASK();   // note: if disabled then PRIMASK should
                                         // not change after this read
  __disable_irq();  // disable all interrupts

  // flag of whether or not object should be updated with new data.
  bool should_update = true;
  
  if (callback)
  {
    should_update = callback(*dataStruct, instance);
  }
  
  if (should_update)
  {
    memcpy( (void *)instancePtr, (void const *)dataStruct, md.numBytes);
    md.timeStamp = glo_os->get_time();
  }

  if (!disabled)
  {
    __enable_irq(); //  enable interrupts
  }
  
  return(1);
}

/*****************************************************************************
* Template Class: GloObjectTemplate specialization
*
* Description: Builds a glo object with a single instance of data based
*              on the data type.
*****************************************************************************/
template <typename objectDataType>
class GloObjectTemplate<objectDataType,1> : public GloObjectBase
{
  public:
    //constructor
    GloObjectTemplate(uint8_t id);

    //destructor
    virtual ~GloObjectTemplate();

    //methods
    double read(objectDataType *copy);        // returns time_stamp
    void publish(objectDataType *dataStruct);

    // Function pointer type to publish callback function.
    typedef bool (*write_callback_t)(objectDataType &);

    // Register a function to be called everytime new data is published.
    void register_callback(write_callback_t callback) { this->callback = callback; }

  private:

    //properties -- note (md) metadata is inherited from GloObjectBase
    objectDataType data;

    // Function pointer to function to call when publishing new data.
    write_callback_t callback;

};

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

// Constructor
template <typename objectDataType>
GloObjectTemplate<objectDataType,1>::
GloObjectTemplate(uint8_t id):
GloObjectBase(id,sizeof(objectDataType),1,(void *)&(data))
{
  gloObjectPtrs[id] = this;
  callback = NULL;
}

// Destructor
template <typename objectDataType>
GloObjectTemplate<objectDataType,1>::~GloObjectTemplate()
{
}

// returns time_stamp
template <typename objectDataType>
double  GloObjectTemplate<objectDataType,1>::read(objectDataType *copy)
{
  double time_stamp;
  
  uint32_t disabled = __get_PRIMASK();   // note: if disabled then PRIMASK should
                                         // not change after this read
  __disable_irq();  // disable all interrupts
  memcpy( (void *)copy, (void const *)&(data), md.numBytes);
  time_stamp = md.timeStamp;
  if (!disabled)
    __enable_irq(); //  enable interrupts

  return(time_stamp);
}

template <typename objectDataType>
void GloObjectTemplate<objectDataType,1>::publish(objectDataType *dataStruct)
{
  uint32_t disabled = __get_PRIMASK();   // note: if disabled then PRIMASK should
                                         // not change after this read
  __disable_irq();  // disable all interrupts

  // flag of whether or not object should be updated with new data.
  bool should_update = true;
  
  if (callback)
  {
      should_update = callback(*dataStruct);
  }
  
  if (should_update)
  {
      memcpy( (void *)&(data), (void const *)dataStruct, md.numBytes);
      md.timeStamp = glo_os->get_time();
  }

  if (!disabled)
  {
    __enable_irq(); //  enable interrupts
  }
}

#endif  //GLO_OBJECT_TEMPLATE_H