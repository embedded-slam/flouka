/***************************************************************************************************
 *
 * flouka - a library for embedded statistics collection.
 *
 * Copyright © 2009  Mohamed Galal El-Din, Karim Emad Morsy.
 *
 ***************************************************************************************************
 *
 * This file is part of flouka library.
 *
 * flouka is free software: you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software Foundation, either version 3 of
 * the License, or any later version.
 *
 * flouka is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with flouka. If
 * not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************************************
 *
 * For more information, questions, or inquiries please contact:
 *
 * Mohamed Galal El-Din:    mohamed.g.ebrahim@gmail.com
 * Karim Emad Morsy:        karim.e.morsy@gmail.com
 *
 **************************************************************************************************/

#ifndef FLOUKA_H_
#define FLOUKA_H_


#include <flouka_common.h>

typedef enum flouka_status
{
    FLOUKA_STATUS_SUCCESS = 0,
    FLOUKA_STATUS_FAILURE = 1
} flouka_status_e;

typedef struct flouka flouka_s;
typedef void*(*AllocFuncPtr)(size_t bytesCount);
typedef void (*DeallocFuncPtr)(void* ptr);
typedef void (*LockFuncPtr)();
typedef void (*UnlockFuncPtr)();

/***************************************************************************************************
 *  Name        : flouka_init
 *
 *  Arguments   : flouka_s**        flouka_Pointer_Ptr,
 *                uint32          totalCountersCount,
 *                AllocFuncPtr      allocationFunction_Ptr,
 *                DeallocFuncPtr    deallocationFunction_Ptr
 *                LockFuncPtr       lockFunction_Ptr
 *                UnlockFuncPtr     unlockFunction_Ptr
 *
 *  Description : This function allocates any memory needed by the statistics collector object, as
 *                well as initializing any required data in it.
 *
 *  Returns     : flouka_status_e
 **************************************************************************************************/
flouka_status_e flouka_init(flouka_s**      flouka_Pointer_Ptr,
                            uint32        totalCountersCount,
                            AllocFuncPtr    allocationFunction_Ptr,
                            DeallocFuncPtr  deallocationFunction_Ptr,
                            LockFuncPtr     lockFunction_Ptr,
                            UnlockFuncPtr   unlockFunction_Ptr COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_destroy
 *
 *  Arguments   : flouka_s*     flouka_Ptr
 *
 *  Description : This function release all memory allocated by the statistics collector object.
 *
 *  Returns     : void.
 **************************************************************************************************/
void flouka_destroy(flouka_s* flouka_Ptr COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_assignCounter
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint32      counterID,
 *                const char*   counterName_Ptr,
 *                const char*   counterDescription_Ptr
 *
 *  Description : This function creates a new counter and initialize it, each counter is a 32-bit
 *                unsigned integer value.
 *
 *  Returns     : void
 **************************************************************************************************/
void flouka_assignCounter(flouka_s*     flouka_Ptr,
                          uint32        counterID,
                          const char*   unit_Ptr,
                          const char*   counterName_Ptr,
                          const char*   counterDescription_Ptr COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_getInformationSize
 *
 *  Arguments   : flouka_s*     flouka_Ptr
 *
 *  Description : This function returns the size of the buffer which needs to be allocated for
 *                serializing the Statistics setup info.
 *
 *  Returns     : uint32
 **************************************************************************************************/
uint32 flouka_getInformationSize(flouka_s* flouka_Ptr COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_getInformation
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint8*      informationBuffer_Ptr,
 *                uint32      allocatedInfoBufferSize COMMA()
 *                FILE_AND_LINE_FOR_TYPE()
 *
 *  Description : This function fills the infoBuffer with the statistics setup information.
 *
 *  Returns     : void
 **************************************************************************************************/
void flouka_getInformation(flouka_s*    flouka_Ptr,
                           uint8*     informationBuffer_Ptr,
                           uint32     allocatedInfoBufferSize COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_getStatistics
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint8**     statisticsBufferPointer_Ptr,
 *                uint32*     statisticsBufferSize_Ptr
 *
 *  Description : This function returns two output argument, one is a pointer to the the statistics
 *                buffer to be sent as is to the UI, and the other is the length of the statistics
 *                data in bytes.
 *
 *                This function needs to be called only one time, and then the same pointer with
 *                the same length should be sent to the UI whenever requested, although the
 *                pointer and the length doesn't change during the course of the program, but the
 *                data pointed to by the pointer do change.
 *
 *  Returns     : void
 **************************************************************************************************/
void flouka_getStatistics(flouka_s* flouka_Ptr,
                          uint8** statisticsBufferPointer_Ptr,
                          uint32* statisticsBufferSize_Ptr COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_incrementCounter
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint32      counterID
 *
 *  Description : This function increments the given counter by one.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE void flouka_incrementCounter(flouka_s*   flouka_Ptr,
                                    uint32    counterID COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_decrementCounter
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint32      counterID
 *
 *  Description : This function decrements the given counter by one.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE void flouka_decrementCounter(flouka_s*   flouka_Ptr,
                                    uint32    counterID COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_increaseCounter
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint32      counterID,
 *                uint32      delta
 *
 *  Description : This function increases the given counter by the given delta.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE void flouka_increaseCounter(flouka_s*    flouka_Ptr,
                                   uint32     counterID,
                                   uint32     delta COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_decreaseCounter
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint32      counterID,
 *                uint32      delta
 *
 *  Description : This function decreases the given counter by the given delta.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE void flouka_decreaseCounter(flouka_s*    flouka_Ptr,
                                   uint32     counterID,
                                   uint32     delta COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_setCounter
 *
 *  Arguments   : flouka_s*     flouka_Ptr,
 *                uint32      counterID,
 *                uint32      value
 *
 *  Description : This function sets the given counter to the given value, and it can be used to
 *                initialize the counter.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE void flouka_setCounter(flouka_s* flouka_Ptr,
                              uint32  counterID,
                              uint32  value COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_resetCounter
 *
 *  Arguments   : flouka_s*    flouka_Ptr,
 *                uint32     counterID,
 *                uint32     value
 *
 *  Description : This function resets the given counter to zero.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE void flouka_resetCounter(flouka_s*   flouka_Ptr,
                                uint32    counterID COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

/***************************************************************************************************
 *  Name        : flouka_getCounter
 *
 *  Arguments   : flouka_s*    flouka_Ptr,
 *                uint32     counterID
 *
 *  Description : This function return the current value of the given counter.
 *
 *  Returns     : void
 **************************************************************************************************/
INLINE uint32 flouka_getCounter(flouka_s* flouka_Ptr,
                                  uint32 counterID COMMA()
                                                     FILE_AND_LINE_FOR_TYPE());

#endif /* FLOUKA_H_ */

