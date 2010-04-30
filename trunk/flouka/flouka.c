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

/***************************************************************************************************
 *
 *                                       I N C L U D E S
 *
 **************************************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flouka.h"

/***************************************************************************************************
 *
 *                                         M A C R O S
 *
 **************************************************************************************************/

/*This macro is used to verify that the statistics collector is initialized properly.*/
#define FLOUKA_INITIALIZATION_PATTEREN    (0x12341234)

#define FLOUKA_COUNTER_MAXIMUM_VALUE      (0xFFFFFFFFLU)
#define FLOUKA_COUNTER_MINIMUM_VALUE      (0x0LU)


/**************************************************************************************************/
#define FLOUKA_ENCODE_PARAMETER(dest_Ptr, param)                                                   \
{                                                                                                  \
    ASSERT((1 == sizeof(*dest_Ptr)),                                                               \
           "FLOUKA:  Invalid encoding pointer received, expected byte/char pointer", \
           __FILE__,                                                                               \
           __LINE__);                                                                              \
    memcpy((dest_Ptr), &(param), sizeof(param));                                                   \
    (dest_Ptr) += sizeof(param);                                                                   \
}
/**************************************************************************************************/
#define FLOUKA_ENCODE_STRING(dest_Ptr, string_Ptr)                                                 \
{                                                                                                  \
    ASSERT((1 == sizeof(*dest_Ptr)),                                                               \
           "FLOUKA:  Invalid encoding pointer received, expected byte/char pointer", \
           __FILE__,                                                                               \
           __LINE__);                                                                              \
    ASSERT((1 == sizeof(*string_Ptr)),                                                             \
           "FLOUKA:  Invalid string pointer received, expected byte/char pointer",   \
           __FILE__,                                                                               \
           __LINE__);                                                                              \
    strcpy((char*)(dest_Ptr), (string_Ptr));                                                       \
    (dest_Ptr) += strlen(string_Ptr) + 1;                                                          \
}
/**************************************************************************************************/

/***************************************************************************************************
 *
 *                                          T Y P E S
 *
 **************************************************************************************************/

/***************************************************************************************************
 * Structure Name:
 * flouka_StatisticsGroupInfo_s
 *
 * Structure Description:
 * This structure holds all the information related to the statistics group, this is an internal
 * structure (used inside the library functions only) that is not seen by the user of the library.
 **************************************************************************************************/
typedef struct flouka_StatisticsGroupInfo
{
    /*Unique integer identifier to identify the group, and it is supplied by the user*/
    uint32 groupID;
    /*String representing the group name*/
    const char* groupName_Ptr;
    /*String representing the group description*/
    const char* groupDescription_Ptr;
#ifdef DEBUG
    /*Indicates whether the group has been assigned or not*/
    bool isAssigned;
#endif /*DEBUG*/
} flouka_StatisticsGroupInfo_s;

/***************************************************************************************************
 * Structure Name:
 * flouka_StatisticsSubGroupInfo_s
 *
 * Structure Description:
 * This structure holds all the information related to the statistics sub group, this is an internal
 * structure (used inside the library functions only) that is not seen by the user of the library.
 **************************************************************************************************/
typedef struct flouka_StatisticsSubGroupInfo
{
    /*Unique integer identifier to identify the sub group, and it is supplied by the user*/
    uint32 subgroupID;
    /*Unique integer identifier to identify the parent group, and it is supplied by the user*/
    uint32 groupID;
    /*String representing the group name*/
    const char* subgroupName_Ptr;
    /*String representing the group description*/
    const char* subgroupDescription_Ptr;
#ifdef DEBUG
    /*Indicates whether the group has been assigned or not*/
    bool isAssigned;
#endif /*DEBUG*/
} flouka_StatisticsSubGroupInfo_s;

/***************************************************************************************************
 * Structure Name:
 * flouka_StatisticsCounterInfo_s
 *
 * Structure Description:
 * This structure holds all the information related to the statistics counter, this is an internal
 * structure that is used by the library function only.
 **************************************************************************************************/
typedef struct flouka_StatisticsCounterInfo
{
    /*Unique integer identifier to identify the counter, and it is supplied by the user*/
    uint32 counterID;
    /*The subgourpID to which this counter belongs*/
    uint32 subgroupID;
    /*The unit of the counter (ex. bytes, errors, N/A, etc...)*/
    const char* unit_Ptr;
    /*String representing the counter name*/
    const char* counterName_Ptr;
    /*String representing the counter description*/
    const char* counterDescription_Ptr;
#ifdef DEBUG
    /*Indicates whether the counter has been assigned or not*/
    bool isAssigned;
#endif /*DEBUG*/
} flouka_StatisticsCounterInfo_s;

/***************************************************************************************************
 * Structure Name:
 * flouka_StatisticsInformationSizes_s
 *
 * Structure Description:
 * This structure holds all the information sized related to the statistics counter, this is an
 * internal structure that is used by the library function only.
 **************************************************************************************************/
typedef struct flouka_StatisticsInformationSizes
{
    /*Holds the number of assigned groups*/
    uint32 assignedGroupsCount;
    /*Holds the number of assigned sub groups*/
    uint32 assignedSubGroupsCount;
    /*Holds the number of assigned counters*/
    uint32 assignedCountersCount;
} flouka_StatisticsInformationSizes_s;

/***************************************************************************************************
 * Structure Name:
 * flouka_StatisticsInformation_s
 *
 * Structure Description:
 * This structure represents the statistics collector class type, it is used to instantiate  one or
 * more statistics collector(s), and it is responsible for managing all the groups and counter.
 **************************************************************************************************/
typedef struct flouka_StatisticsInformation
{
    /**/
    flouka_StatisticsInformationSizes_s sizes;
    /*Points to the list of group information structures*/
    flouka_StatisticsGroupInfo_s* groupInfoList_Ptr;
    /*Points to the list of sub group information structures*/
    flouka_StatisticsSubGroupInfo_s* subgroupInfoList_Ptr;
    /*Points to the list of counter information structures*/
    flouka_StatisticsCounterInfo_s* counterInfoList_Ptr;
} flouka_StatisticsInformation_s;

/***************************************************************************************************
 * Structure Name:
 * flouka_s
 *
 * Structure Description:
 * This structure represents the statistics collector class type, it is used to instantiate  one or
 * more statistics collector(s), and it is responsible for managing all the groups and counter.
 **************************************************************************************************/
struct flouka
{
    /*Holds the meta data related to the statistics counters*/
    flouka_StatisticsInformation_s information;
    /*Points to the function that will be used to release the allocated memory*/
    DeallocFuncPtr deallocationFunction_Ptr;
    /*Used to protect the object from multiple access during group/counter assignment only*/
    LockFuncPtr lockFunction_Ptr;
    /*Used to protect the object from multiple access during group/counter assignment only*/
    UnlockFuncPtr unlockFunction_Ptr;
    /*points to the list of counters*/
    uint32* counterValuesList_Ptr;
    /*Holds the total number of supported groups*/
    uint32 totalGroupsCount;
    /*Holds the total number of supported sub groups*/
    uint32 totalSubGroupsCount;
    /*Holds the total number of supported counters*/
    uint32 totalCountersCount;
#ifdef DEBUG
    uint32 initializationPattern;
#endif /**/
};

/***************************************************************************************************
 *
 *                      I N T E R N A L   F U N C T I O N   D E F I N I T I O N S
 *
 **************************************************************************************************/

uint8* StatisticsGroupInfo_serialize(flouka_StatisticsGroupInfo_s* groupInfo_Ptr,
                                       uint8* serializationBuffer_Ptr)
{
    /*
     * Steps done in this function:
     * ============================
     * 1. Encode groupID.
     * 2. Encode groupName_Ptr.
     * 3. Encode groupDescription_Ptr.
     * 4. Return the new serializationBuffer_Ptr, after advancing it by the size of bytes encoded.
     */
    FLOUKA_ENCODE_PARAMETER(serializationBuffer_Ptr, groupInfo_Ptr->groupID);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, groupInfo_Ptr->groupName_Ptr);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, groupInfo_Ptr->groupDescription_Ptr);

    return (serializationBuffer_Ptr);
}

uint32 StatisticsGroupInfo_getSerializedSize(flouka_StatisticsGroupInfo_s* groupInfo_Ptr)
{
    uint32 serializedSize = 0;

    /*
     * Steps done in this function:
     * ============================
     * 1. Calculate the number of bytes needed to serialize groupInfo_Ptr.
     */
    serializedSize += sizeof(groupInfo_Ptr->groupID);
    serializedSize += (strlen(groupInfo_Ptr->groupName_Ptr) + 1);
    serializedSize += (strlen(groupInfo_Ptr->groupDescription_Ptr) + 1);
    return (serializedSize);
}

uint8* StatisticsSubGroupInfo_serialize(flouka_StatisticsSubGroupInfo_s* subGroupInfo_Ptr,
                                          uint8* serializationBuffer_Ptr)
{

    /*
     * Steps done in this function:
     * ============================
     * 1. Encode subgroupID.
     * 2. Encode groupID.
     * 3. Encode subgroupName_Ptr.
     * 4. Encode subgroupDescription_Ptr.
     * 5. Return the new serializationBuffer_Ptr, after advancing it by the size of bytes encoded.
     */

    FLOUKA_ENCODE_PARAMETER(serializationBuffer_Ptr, subGroupInfo_Ptr->subgroupID);
    FLOUKA_ENCODE_PARAMETER(serializationBuffer_Ptr, subGroupInfo_Ptr->groupID);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, subGroupInfo_Ptr->subgroupName_Ptr);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, subGroupInfo_Ptr->subgroupDescription_Ptr);

    return (serializationBuffer_Ptr);
}

uint32 StatisticsSubGroupInfo_getSerializedSize(flouka_StatisticsSubGroupInfo_s* subGroupInfo_Ptr)
{
    uint32 serializedSize = 0;

    /*
     * Steps done in this function:
     * ============================
     * 1. Calculate the number of bytes needed to serialize subGroupInfo_Ptr.
     */
    serializedSize += sizeof(subGroupInfo_Ptr->subgroupID);
    serializedSize += sizeof(subGroupInfo_Ptr->groupID);
    serializedSize += (strlen(subGroupInfo_Ptr->subgroupName_Ptr) + 1);
    serializedSize += (strlen(subGroupInfo_Ptr->subgroupDescription_Ptr) + 1);

    return (serializedSize);
}

uint8* StatisticsCounterInfo_serialize(flouka_StatisticsCounterInfo_s* counterInfo_Ptr,
                                         uint8* serializationBuffer_Ptr)
{
    /*
     * Steps done in this function:
     * ============================
     * 1. Encode counterID.
     * 2. Encode subgroupID.
     * 3. Encode unit_Ptr.
     * 4. Encode counterName_Ptr.
     * 5. Encode counterDescription_Ptr.
     * 6. Return the new serializationBuffer_Ptr, after advancing it by the size of bytes encoded.
     */

    FLOUKA_ENCODE_PARAMETER(serializationBuffer_Ptr, counterInfo_Ptr->counterID);
    FLOUKA_ENCODE_PARAMETER(serializationBuffer_Ptr, counterInfo_Ptr->subgroupID);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, counterInfo_Ptr->unit_Ptr);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, counterInfo_Ptr->counterName_Ptr);
    FLOUKA_ENCODE_STRING (serializationBuffer_Ptr, counterInfo_Ptr->counterDescription_Ptr);

    return (serializationBuffer_Ptr);
}

uint32 StatisticsCounterInfo_getSerializedSize(flouka_StatisticsCounterInfo_s* counterInfo_Ptr)
{
    uint32 serializedSize = 0;
    /*
     * Steps done in this function:
     * ============================
     * 1. Calculate the number of bytes needed to serialize counterInfo_Ptr.
     */
    serializedSize += sizeof(counterInfo_Ptr->counterID);
    serializedSize += sizeof(counterInfo_Ptr->subgroupID);
    serializedSize += (strlen(counterInfo_Ptr->unit_Ptr) + 1);
    serializedSize += (strlen(counterInfo_Ptr->counterName_Ptr) + 1);
    serializedSize += (strlen(counterInfo_Ptr->counterDescription_Ptr) + 1);
    return (serializedSize);
}

void StatisticsInformation_serialize(flouka_StatisticsInformation_s* statisticsInfo_Ptr,
                                     uint8* serializationBuffer_Ptr,
                                     uint32 maxGroupsCount,
                                     uint32 maxSubGroupsCount,
                                     uint32 maxCountersCount)
{

    uint32 i;

    FLOUKA_ENCODE_PARAMETER(serializationBuffer_Ptr, statisticsInfo_Ptr->sizes);

    for(i = 0; i < maxGroupsCount; i++)
    {
        serializationBuffer_Ptr
                        = StatisticsGroupInfo_serialize(&(statisticsInfo_Ptr->groupInfoList_Ptr[i]),
                                                        serializationBuffer_Ptr);
    }

    for(i = 0; i < maxSubGroupsCount; i++)
    {
        serializationBuffer_Ptr
                        = StatisticsSubGroupInfo_serialize(&(statisticsInfo_Ptr->subgroupInfoList_Ptr[i]),
                                                           serializationBuffer_Ptr);
    }

    for(i = 0; i < maxCountersCount; i++)
    {
        serializationBuffer_Ptr
                        = StatisticsCounterInfo_serialize(&(statisticsInfo_Ptr->counterInfoList_Ptr[i]),
                                                          serializationBuffer_Ptr);
    }
}

uint32 StatisticsInformation_getSerializedSize(flouka_StatisticsInformation_s* statisticsInfo_Ptr,
                                                 uint32 maxGroupsCount,
                                                 uint32 maxSubGroupsCount,
                                                 uint32 maxCountersCount)
{
    uint32 i;
    uint32 serializedSize = 0;

    serializedSize += sizeof(statisticsInfo_Ptr->sizes.assignedGroupsCount);
    serializedSize += sizeof(statisticsInfo_Ptr->sizes.assignedSubGroupsCount);
    serializedSize += sizeof(statisticsInfo_Ptr->sizes.assignedCountersCount);

    for(i = 0; i < maxGroupsCount; i++)
    {
        serializedSize
                        += StatisticsGroupInfo_getSerializedSize(&(statisticsInfo_Ptr->groupInfoList_Ptr[i]));
    }

    for(i = 0; i < maxSubGroupsCount; i++)
    {
        serializedSize
                        += StatisticsSubGroupInfo_getSerializedSize(&(statisticsInfo_Ptr->subgroupInfoList_Ptr[i]));
    }

    for(i = 0; i < maxCountersCount; i++)
    {
        serializedSize
                        += StatisticsCounterInfo_getSerializedSize(&(statisticsInfo_Ptr->counterInfoList_Ptr[i]));
    }
    return (serializedSize);
}

/***************************************************************************************************
 *
 *                     I N T E R F A C E   F U N C T I O N   D E F I N I T I O N S
 *
 **************************************************************************************************/

flouka_status_e flouka_init(flouka_s** flouka_Pointer_Ptr,
                            uint32 totalGroupsCount,
                            uint32 totalSubGroupsCount,
                            uint32 totalCountersCount,
                            AllocFuncPtr allocationFunction_Ptr,
                            DeallocFuncPtr deallocationFunction_Ptr,
                            LockFuncPtr lockFunction_Ptr,
                            UnlockFuncPtr unlockFunction_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    uint32 i;
    flouka_s* flouka_Ptr;
    flouka_status_e status;

    status = FLOUKA_STATUS_SUCCESS;
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (Must be NULL).
     * 2. Validate the number of groups (non-zero).
     * 3. Validate the number of sub groups (non-zero).
     * 4. Validate the number of counters (non-zero).
     * 5. Validate the allocation function pointer (not NULL).
     * 6. Validate the deallocation function pointer (not NULL).
     * 7. Validate the lock function pointer (not NULL).
     * 8. Validate the unlock function pointer (not NULL).
     */
    ASSERT((NULL == *flouka_Pointer_Ptr),
                    "FLOUKA:  *flouka_Ptr pointer is not NULL, it is expected to initialize a NULL pointer",
                    fileName,
                    lineNumber);
    ASSERT((totalGroupsCount> 0),
                    "FLOUKA:  Total number of groups cannot be zero",
                    fileName,
                    lineNumber);
    ASSERT((totalSubGroupsCount> 0),
                    "FLOUKA:  Total number of sub groups cannot be zero",
                    fileName,
                    lineNumber);
    ASSERT((totalCountersCount> 0),
                    "FLOUKA:  Total number of counters cannot be zero",
                    fileName,
                    lineNumber);
    ASSERT((NULL != allocationFunction_Ptr),
                    "FLOUKA:  allocation function cannot be NULL",
                    fileName,
                    lineNumber);
    ASSERT((NULL != deallocationFunction_Ptr),
                    "FLOUKA:  deallocation function cannot be NULL",
                    fileName,
                    lineNumber);
    ASSERT((NULL != lockFunction_Ptr),
                    "FLOUKA:  lock function cannot be NULL",
                    fileName,
                    lineNumber);
    ASSERT((NULL != unlockFunction_Ptr),
                    "FLOUKA:  unlock function cannot be NULL",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Allocate the space needed for the statistics collector object using the given function.
     * 2. Allocate memory for the internal members.
     * 3. Save the passed parameters (e.g. totalGroupsCount).
     * 4. Initialize all groups and counter to not-assigned.
     *
     * Note 1:
     * flouka_Ptr is a double pointer, so that this function can change what it points to, the
     * reason for this is to hide the details of the internal memory needed from the caller.
     *
     * Note 2:
     * allocationFunction_Ptr was not saved to the object, because it is only used during the
     * initialization (inside this function), and never used afterwards, unlike the
     * deallocationFunction_Ptr which is needed during the destroy function.
     */

    flouka_Ptr = (flouka_s*) allocationFunction_Ptr(sizeof(*flouka_Ptr));
    flouka_Ptr->information.groupInfoList_Ptr
                    = (flouka_StatisticsGroupInfo_s*) allocationFunction_Ptr(totalGroupsCount
                                    * sizeof(*flouka_Ptr->information.groupInfoList_Ptr));
    flouka_Ptr->information.subgroupInfoList_Ptr
                    = (flouka_StatisticsSubGroupInfo_s*) allocationFunction_Ptr(totalSubGroupsCount
                                    * sizeof(*flouka_Ptr->information.subgroupInfoList_Ptr));
    flouka_Ptr->information.counterInfoList_Ptr
                    = (flouka_StatisticsCounterInfo_s*) allocationFunction_Ptr(totalCountersCount
                                    * sizeof(*flouka_Ptr->information.counterInfoList_Ptr));
    flouka_Ptr->information.sizes.assignedGroupsCount = 0;
    flouka_Ptr->information.sizes.assignedSubGroupsCount = 0;
    flouka_Ptr->information.sizes.assignedCountersCount = 0;

    flouka_Ptr->counterValuesList_Ptr = (uint32*) allocationFunction_Ptr(totalCountersCount
                    * sizeof(*flouka_Ptr->counterValuesList_Ptr));
    flouka_Ptr->deallocationFunction_Ptr = deallocationFunction_Ptr;
    flouka_Ptr->lockFunction_Ptr = lockFunction_Ptr;
    flouka_Ptr->unlockFunction_Ptr = unlockFunction_Ptr;
    flouka_Ptr->totalGroupsCount = totalGroupsCount;
    flouka_Ptr->totalSubGroupsCount = totalSubGroupsCount;
    flouka_Ptr->totalCountersCount = totalCountersCount;

    for(i = 0; i < totalGroupsCount; i++)
    {
#ifdef DEBUG
        flouka_Ptr->information.groupInfoList_Ptr[i].isAssigned = FALSE;
#endif /*DEBUG*/
        flouka_Ptr->information.groupInfoList_Ptr[i].groupName_Ptr = "";
        flouka_Ptr->information.groupInfoList_Ptr[i].groupDescription_Ptr = "";
    } /*for*/

    for(i = 0; i < totalSubGroupsCount; i++)
    {
#ifdef DEBUG
        flouka_Ptr->information.subgroupInfoList_Ptr[i].isAssigned = FALSE;
#endif /*DEBUG*/
        flouka_Ptr->information.subgroupInfoList_Ptr[i].subgroupName_Ptr = "";
        flouka_Ptr->information.subgroupInfoList_Ptr[i].subgroupDescription_Ptr = "";
    } /*for*/

    for(i = 0; i < totalCountersCount; i++)
    {
#ifdef DEBUG
        flouka_Ptr->information.counterInfoList_Ptr[i].isAssigned = FALSE;
#endif /*DEBUG*/
        flouka_Ptr->information.counterInfoList_Ptr[i].unit_Ptr = "";
        flouka_Ptr->information.counterInfoList_Ptr[i].counterName_Ptr = "";
        flouka_Ptr->information.counterInfoList_Ptr[i].counterDescription_Ptr = "";
    } /*for*/
#ifdef DEBUG
    flouka_Ptr->initializationPattern = FLOUKA_INITIALIZATION_PATTEREN;
#endif /*DEBUG*/
    *flouka_Pointer_Ptr = flouka_Ptr;
    return status;
}

void flouka_destroy(flouka_s* flouka_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    DeallocFuncPtr deallocationFunctionPointer;
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not null).
     * 2. Validate the flouka_Ptr (already initialized).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((FLOUKA_INITIALIZATION_PATTEREN != flouka_Ptr->initializationPattern),
                    "FLOUKA:  Invalid statistics counter pointer passed (either not initialized pointer, or incorrect, non-null pointer)",
                    fileName,
                    lineNumber);
    /*
     * Steps done in this function:
     * ============================
     * 1. Get the pointer to the deallocation function.
     * 2. Deallocate all the internal member.
     * 3. Deallocate the flouka_Ptr itself.
     * 4. Set the flouka_Ptr to NULL to prevent invalid access.
     */
    deallocationFunctionPointer = flouka_Ptr->deallocationFunction_Ptr;
    deallocationFunctionPointer(flouka_Ptr->information.groupInfoList_Ptr);
    deallocationFunctionPointer(flouka_Ptr->information.subgroupInfoList_Ptr);
    deallocationFunctionPointer(flouka_Ptr->information.counterInfoList_Ptr);
    deallocationFunctionPointer(flouka_Ptr->counterValuesList_Ptr);
    deallocationFunctionPointer((void*) flouka_Ptr);
    flouka_Ptr = NULL;
}

void flouka_assignGroup(flouka_s* flouka_Ptr,
                        uint32 groupID,
                        const char* groupName_Ptr,
                        const char* groupDescription_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the total number of assigned groups (less than maximum).
     * 3. Validate the given group ID (less than maximum).
     * 4. Validate the group assignment status (not assigned).
     * 5. Validate the groupName_Ptr (not NULL).
     * 6. Validate the groupName_Ptr (non empty string ("")).
     * 7. Validate the groupDescription_Ptr (not NULL).
     * 8. Validate the groupDescription_Ptr (non empty string ("")).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedGroupsCount < flouka_Ptr->totalGroupsCount),
                    "FLOUKA:  Maximum number of groups exceeded initialized value",
                    fileName,
                    lineNumber);
    ASSERT((groupID < flouka_Ptr->totalGroupsCount),
                    "FLOUKA:  groupID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((FALSE == flouka_Ptr->information.groupInfoList_Ptr[groupID].isAssigned),
                    "FLOUKA:  groupID is already assigned" ,
                    fileName,
                    lineNumber);
    ASSERT((NULL != groupName_Ptr),
                    "FLOUKA:  NULL was passed as the group name pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != groupName_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the group name pointer",
                    fileName,
                    lineNumber);
    ASSERT((NULL != groupDescription_Ptr),
                    "FLOUKA:  NULL was passed as the group description pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != groupDescription_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the group description pointer",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Lock access to prevent data corruption when calling this function from multiple threads.
     * 2. Assign the group name.
     * 3. Assign the group description.
     * 4. Set the group as assigned.
     * 5. Increment the number of assigned groups.
     * 6. Unlock access.
     *
     */
    flouka_Ptr->lockFunction_Ptr();

    flouka_Ptr->information.groupInfoList_Ptr[groupID].groupName_Ptr = groupName_Ptr;
    flouka_Ptr->information.groupInfoList_Ptr[groupID].groupDescription_Ptr = groupDescription_Ptr;
#ifdef DEBUG
    flouka_Ptr->information.groupInfoList_Ptr[groupID].isAssigned = TRUE;
#endif /*DEBUG*/
    flouka_Ptr->information.sizes.assignedGroupsCount++;

    flouka_Ptr->unlockFunction_Ptr();
}

void flouka_assignSubGroup(flouka_s* flouka_Ptr,
                           uint32 subgroupID,
                           uint32 groupID,
                           const char* subgroupName_Ptr,
                           const char* subgroupDescription_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the total number of assigned sub groups (less than maximum).
     * 3. Validate the given sub group ID (less than maximum).
     * 4. Validate the given group ID (less than maximum).
     * 5. Validate the sub group assignment status (not assigned).
     * 6. Validate the group assignment status (assigned).
     * 7. Validate the subgroupName_Ptr (not NULL).
     * 8. Validate the subgroupName_Ptr (non empty string ("")).
     * 9. Validate the subgroupDescription_Ptr (not NULL).
     * 10. Validate the subgroupDescription_Ptr (non empty string ("")).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedSubGroupsCount < flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA:  Maximum number of sub groups exceeded initialized value",
                    fileName,
                    lineNumber);
    ASSERT((subgroupID < flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA:  subgroupID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((groupID < flouka_Ptr->totalGroupsCount),
                    "FLOUKA:  groupID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((FALSE == flouka_Ptr->information.subgroupInfoList_Ptr[subgroupID].isAssigned),
                    "FLOUKA:  subgroupID is already assigned" ,
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.groupInfoList_Ptr[groupID].isAssigned),
                    "FLOUKA:  groupID is not assigned yet" ,
                    fileName,
                    lineNumber);
    ASSERT((NULL != subgroupName_Ptr),
                    "FLOUKA:  NULL was passed as the sub group name pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != subgroupName_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the sub group name pointer",
                    fileName,
                    lineNumber);
    ASSERT((NULL != subgroupDescription_Ptr),
                    "FLOUKA:  NULL was passed as the sub group description pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != subgroupDescription_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the sub group description pointer",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Lock access to prevent data corruption when calling this function from multiple threads.
     * 2. Assign the parent group id.
     * 3. Assign the sub group name.
     * 4. Assign the sub group description.
     * 5. Set the sub group as assigned.
     * 6. Increment the number of assigned sub groups.
     * 7. Unlock access.
     *
     */
    flouka_Ptr->lockFunction_Ptr();

    flouka_Ptr->information.subgroupInfoList_Ptr[subgroupID].groupID = groupID;
    flouka_Ptr->information.subgroupInfoList_Ptr[subgroupID].subgroupName_Ptr = subgroupName_Ptr;
    flouka_Ptr->information.subgroupInfoList_Ptr[subgroupID].subgroupDescription_Ptr
                    = subgroupDescription_Ptr;

#ifdef DEBUG
    flouka_Ptr->information.subgroupInfoList_Ptr[subgroupID].isAssigned = TRUE;
#endif /*DEBUG*/
    flouka_Ptr->information.sizes.assignedSubGroupsCount++;

    flouka_Ptr->unlockFunction_Ptr();
}

void flouka_assignCounter(flouka_s* flouka_Ptr,
                          uint32 counterID,
                          uint32 subgroupID,
                          const char* unit_Ptr,
                          const char* counterName_Ptr,
                          const char* counterDescription_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the total number of assigned counters (less than maximum).
     * 3. Validate the given counter ID (less than maximum).
     * 4. Validate the given sub group ID (less than maximum).
     * 5. Validate the sub group assignment status (assigned).
     * 6. Validate the counter assignment status (not assigned).
     * 7. Validate the counterName_Ptr (not NULL).
     * 8. Validate the counterName_Ptr (non empty string ("")).
     * 9. Validate the counterDescription_Ptr (not NULL).
     * 10.Validate the counterDescription_Ptr (non empty string ("")).
     * 11.Validate the unit_Ptr (not NULL).
     * 12.Validate the unit_Ptr (non empty string ("")).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedCountersCount < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  Maximum number of counters exceeded initialized value",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((subgroupID < flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA:  subgroupID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.subgroupInfoList_Ptr[subgroupID].isAssigned),
                    "FLOUKA:  subroupID is not assigned yet",
                    fileName,
                    lineNumber);
    ASSERT((FALSE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is already assigned",
                    fileName,
                    lineNumber);
    ASSERT((NULL != counterName_Ptr),
                    "FLOUKA:  NULL was passed as the counter name pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != counterName_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the counter name pointer",
                    fileName,
                    lineNumber);
    ASSERT((NULL != counterDescription_Ptr),
                    "FLOUKA:  NULL was passed as the counter description pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != counterDescription_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the counter description pointer",
                    fileName,
                    lineNumber);
    ASSERT((NULL != unit_Ptr),
                    "FLOUKA:  NULL was passed as the counter unit pointer",
                    fileName,
                    lineNumber);
    ASSERT(('\0' != unit_Ptr[0]),
                    "FLOUKA:  Empty string (\"\") was passed as the counter unit pointer",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Lock access to prevent data corruption when calling this function from multiple threads.
     * 2. Set the parent sub group id.
     * 3. Assign the counter unit.
     * 4. Assign the counter name.
     * 5. Assign the counter description.
     * 6. Set the counter as assigned.
     * 7. Increment the number of assigned counters.
     * 8. Unlock access.
     */
    flouka_Ptr->lockFunction_Ptr();

    flouka_Ptr->information.counterInfoList_Ptr[counterID].counterID = counterID;
    flouka_Ptr->information.counterInfoList_Ptr[counterID].subgroupID = subgroupID;
    flouka_Ptr->information.counterInfoList_Ptr[counterID].unit_Ptr = unit_Ptr;

    flouka_Ptr->information.counterInfoList_Ptr[counterID].counterName_Ptr = counterName_Ptr;
    flouka_Ptr->information.counterInfoList_Ptr[counterID].counterDescription_Ptr
                    = counterDescription_Ptr;
#ifdef DEBUG
    flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned = TRUE;
#endif /*DEBUG*/
    flouka_Ptr->information.sizes.assignedCountersCount++;

    flouka_Ptr->unlockFunction_Ptr();
}

uint32 flouka_getInformationSize(flouka_s* flouka_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedGroupsCount == flouka_Ptr->totalGroupsCount),
                    "FLOUKA: assigned groups are less than the total, you have to assign all groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedSubGroupsCount == flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA: assigned sub groups are less than the total, you have to assign all sub groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedCountersCount == flouka_Ptr->totalCountersCount),
                    "FLOUKA: assigned counters are less than the total, you have to assign all counters",
                    fileName,
                    lineNumber);

    return (StatisticsInformation_getSerializedSize(&(flouka_Ptr->information),
                                                    flouka_Ptr->totalGroupsCount,
                                                    flouka_Ptr->totalSubGroupsCount,
                                                    flouka_Ptr->totalCountersCount));
}

void flouka_getInformation(flouka_s* flouka_Ptr,
                           uint8* informationBuffer_Ptr,
                           uint32 allocatedInfoBufferSize COMMA() FILE_AND_LINE_FOR_TYPE())
{
    uint32 infoSize;

    infoSize = flouka_getInformationSize(flouka_Ptr COMMA() FILE_AND_LINE_FOR_CALL()) + LENGTH_HEADER_SIZE;

    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((allocatedInfoBufferSize >= infoSize),
                    "FLOUKA: Information buffer allocated is smaller than expected)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedGroupsCount == flouka_Ptr->totalGroupsCount),
                    "FLOUKA: assigned groups are less than the total, you have to assign all groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedSubGroupsCount == flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA: assigned sub groups are less than the total, you have to assign all sub groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedCountersCount == flouka_Ptr->totalCountersCount),
                    "FLOUKA: assigned counters are less than the total, you have to assign all counters",
                    fileName,
                    lineNumber);

    FLOUKA_ENCODE_PARAMETER(informationBuffer_Ptr, infoSize);

    StatisticsInformation_serialize(&(flouka_Ptr->information),
                                    informationBuffer_Ptr,
                                    flouka_Ptr->totalGroupsCount,
                                    flouka_Ptr->totalSubGroupsCount,
                                    flouka_Ptr->totalCountersCount);

}

uint32 flouka_getStatisticsSize(flouka_s* flouka_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedGroupsCount == flouka_Ptr->totalGroupsCount),
                    "FLOUKA: assigned groups are less than the total, you have to assign all groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedSubGroupsCount == flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA: assigned sub groups are less than the total, you have to assign all sub groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedCountersCount == flouka_Ptr->totalCountersCount),
                    "FLOUKA: assigned counters are less than the total, you have to assign all counters",
                    fileName,
                    lineNumber);

    return ((flouka_Ptr->totalCountersCount) * sizeof(*(flouka_Ptr->counterValuesList_Ptr)));
}

void flouka_getStatistics(flouka_s* flouka_Ptr,
                          uint8** statisticsBufferPointer_Ptr,
                          uint32* statisticsBufferSize_Ptr COMMA() FILE_AND_LINE_FOR_TYPE())
{
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedGroupsCount == flouka_Ptr->totalGroupsCount),
                    "FLOUKA: assigned groups are less than the total, you have to assign all groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedSubGroupsCount == flouka_Ptr->totalSubGroupsCount),
                    "FLOUKA: assigned sub groups are less than the total, you have to assign all sub groups",
                    fileName,
                    lineNumber);
    ASSERT((flouka_Ptr->information.sizes.assignedCountersCount == flouka_Ptr->totalCountersCount),
                    "FLOUKA: assigned counters are less than the total, you have to assign all counters",
                    fileName,
                    lineNumber);

    *statisticsBufferSize_Ptr = (flouka_Ptr->totalCountersCount)
                    * (sizeof(*(flouka_Ptr->counterValuesList_Ptr)));

    *statisticsBufferPointer_Ptr = (uint8*) flouka_Ptr->counterValuesList_Ptr;
}

INLINE void flouka_incrementCounter(flouka_s* flouka_Ptr,
                                    uint32 counterID COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     * 4. Validate the counter value (no overflow).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    ASSERT(((flouka_Ptr->counterValuesList_Ptr[counterID] + 1) < FLOUKA_COUNTER_MAXIMUM_VALUE),
                    "FLOUKA:  Counter reached the maximum possible value and will wrap around, comment this line if that is OK",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Increment the counter value by one.
     */
    flouka_Ptr->counterValuesList_Ptr[counterID]++;
}

INLINE void flouka_decrementCounter(flouka_s*   flouka_Ptr,
                                    uint32    counterID COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     * 4. Validate the counter value (no underflow).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    ASSERT(((flouka_Ptr->counterValuesList_Ptr[counterID] - 1) > FLOUKA_COUNTER_MINIMUM_VALUE),
                    "FLOUKA:  Counter reached the minimum possible value and will wrap around, comment this line if that is OK",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. decrement the counter value by one.
     */
    flouka_Ptr->counterValuesList_Ptr[counterID]--;
}

INLINE void flouka_increaseCounter(flouka_s*    flouka_Ptr,
                                   uint32     counterID,
                                   uint32     delta COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     * 4. Validate the counter value (no overflow).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized" ,
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    ASSERT(((FLOUKA_COUNTER_MAXIMUM_VALUE - flouka_Ptr->counterValuesList_Ptr[counterID])> delta),
                    "FLOUKA:  Overflow occurred and counter will wrap around, comment this line if that is OK",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Increment the counter value by the given delta.
     */
    flouka_Ptr->counterValuesList_Ptr[counterID] += delta;
}

INLINE void flouka_decreaseCounter(flouka_s*    flouka_Ptr,
                                   uint32     counterID,
                                   uint32     delta COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     * 4. Validate the counter value (no underflow).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized" ,
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    ASSERT(((flouka_Ptr->counterValuesList_Ptr[counterID] - FLOUKA_COUNTER_MINIMUM_VALUE)> delta),
                    "FLOUKA:  Underflow occurred and counter will wrap around, comment this line if that is OK",
                    fileName,
                    lineNumber);

    /*
     * Steps done in this function:
     * ============================
     * 1. Decrement the counter value by the given delta.
     */
    flouka_Ptr->counterValuesList_Ptr[counterID] -= delta;
}

INLINE void flouka_setCounter(flouka_s* flouka_Ptr,
                              uint32 counterID,
                              uint32 value COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    /*
     * Steps done in this function:
     * ============================
     * 1. Set the counter to the given value.
     */
    flouka_Ptr->counterValuesList_Ptr[counterID] = value;

}

INLINE void flouka_resetCounter(flouka_s* flouka_Ptr,
                                uint32 counterID COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    /*
     * Steps done in this function:
     * ============================
     * 1. Reset the counter.
     */
    flouka_Ptr->counterValuesList_Ptr[counterID] = FLOUKA_COUNTER_MINIMUM_VALUE;
}

INLINE uint32 flouka_getCounter(flouka_s* flouka_Ptr,
                                  uint32 counterID COMMA() FILE_AND_LINE_FOR_TYPE())
{
    /*
     * Assertions done in this function:
     * =================================
     * 1. Validate the flouka_Ptr (not NULL).
     * 2. Validate the given counter ID (less than maximum).
     * 3. Validate the counter assignment status (assigned).
     */
    ASSERT((NULL != flouka_Ptr),
                    "FLOUKA:  Invalid statistics counter pointer passed (NULL pointer passed)",
                    fileName,
                    lineNumber);
    ASSERT((counterID < flouka_Ptr->totalCountersCount),
                    "FLOUKA:  CounterID is outside of the range initialized",
                    fileName,
                    lineNumber);
    ASSERT((TRUE == flouka_Ptr->information.counterInfoList_Ptr[counterID].isAssigned),
                    "FLOUKA:  CounterID is not assigned yet",
                    fileName,
                    lineNumber);
    /*
     * Steps done in this function:
     * ============================
     * 1. Return the counter value.
     */
    return (flouka_Ptr->counterValuesList_Ptr[counterID]);
}
