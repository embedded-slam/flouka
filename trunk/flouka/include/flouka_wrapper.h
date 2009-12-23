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
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * flouka is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with flouka. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************************************
 *
 * For more information, questions, or inquiries please contact:
 *
 * Mohamed Galal El-Din:    mohamed.g.ebrahim@gmail.com
 * Karim Emad Morsy:        karim.e.morsy@gmail.com
 *
 **************************************************************************************************/
#ifndef FLOUKA_WRAPPER_H_
#define FLOUKA_WRAPPER_H_

extern flouka_s* g_flouka_Ptr;

/**************************************************************************************************/
#define FLOUKA_INIT(maxGroupsCount,                                                                \
                    maxSubGroupsCount,                                                             \
                    maxCountersCount,                                                              \
                    allocationFunction_Ptr,                                                        \
                    deallocationFunction_Ptr,                                                      \
                    lockFunction_Ptr,                                                              \
                    unlockFunction_Ptr)                                                            \
{                                                                                                  \
    flouka_status_e status;                                                                        \
    status = flouka_init(&(g_flouka_Ptr),                                                          \
                         (maxGroupsCount),                                                         \
                         (maxSubGroupsCount),                                                      \
                         (maxCountersCount),                                                       \
                         (allocationFunction_Ptr),                                                 \
                         (deallocationFunction_Ptr),                                               \
                         (lockFunction_Ptr),                                                       \
                         (unlockFunction_Ptr) COMMA()                                              \
                         FILE_AND_LINE_FOR_REF());                                                 \
                                                                                                   \
    ASSERT((FLOUKA_STATUS_SUCCESS == status),                                                      \
           "STATISTICS COLLECTOR: Failed to create the object object",                             \
           __FILE__,                                                                               \
           __LINE__);                                                                              \
}
/**************************************************************************************************/
#define FLOUKA_DESTROY()                                                                           \
{                                                                                                  \
    flouka_destroy((g_flouka_Ptr) COMMA()                                                          \
                    FILE_AND_LINE_FOR_REF());                                                      \
}
/**************************************************************************************************/
#define FLOUKA_ASSIGN_GROUP(groupID,                                                               \
                            groupName_Ptr,                                                         \
                            groupDescription_Ptr)                                                  \
{                                                                                                  \
    flouka_assignGroup((g_flouka_Ptr),                                                             \
                       (groupID),                                                                  \
                       (groupName_Ptr),                                                            \
                       (groupDescription_Ptr) COMMA()                                              \
                       FILE_AND_LINE_FOR_REF());                                                   \
}
/**************************************************************************************************/
#define FLOUKA_ASSIGN_SUB_GROUP(subgroupID,                                                        \
                                groupID,                                                           \
                                subgroupName_Ptr,                                                  \
                                subgroupDescription_Ptr)                                           \
{                                                                                                  \
    flouka_assignSubGroup((g_flouka_Ptr),                                                          \
                          (subgroupID),                                                            \
                          (groupID),                                                               \
                          (subgroupName_Ptr),                                                      \
                          (subgroupDescription_Ptr) COMMA()                                        \
                          FILE_AND_LINE_FOR_REF());                                                \
}
/**************************************************************************************************/
#define FLOUKA_ASSIGN_COUNTER(counterID,                                                           \
                              groupID,                                                             \
                              unit_Ptr,                                                            \
                              counterName_Ptr,                                                     \
                              counterDescription_Ptr)                                              \
{                                                                                                  \
    flouka_assignCounter((g_flouka_Ptr),                                                           \
                         (counterID),                                                              \
                         (groupID),                                                                \
                         (unit_Ptr),                                                               \
                         (counterName_Ptr),                                                        \
                         (counterDescription_Ptr) COMMA()                                          \
                         FILE_AND_LINE_FOR_REF());                                                 \
}
/**************************************************************************************************/
#define FLOUKA_GET_INFORMATIOM_SIZE()                                                              \
        (LENGTH_HEADER_SIZE +                                                                      \
         flouka_getInformationSize((g_flouka_Ptr) COMMA()                                          \
                                                FILE_AND_LINE_FOR_REF()))
/**************************************************************************************************/
#define FLOUKA_GET_INFORMATION(informationBuffer_Ptr,                                              \
                               allocatedInfoBufferSize)                                            \
{                                                                                                  \
    flouka_getInformation((g_flouka_Ptr),                                                          \
                          (informationBuffer_Ptr),                                                 \
                          (allocatedInfoBufferSize) COMMA()                                        \
                          FILE_AND_LINE_FOR_REF());                                                \
}
/**************************************************************************************************/
#define FLOUKA_GET_STATISTICS(statisticsBufferPointer_Ptr,                                         \
                              statisticsBufferSize_Ptr)                                            \
{                                                                                                  \
    flouka_getStatistics((g_flouka_Ptr),                                                           \
                         (statisticsBufferPointer_Ptr),                                            \
                         (statisticsBufferSize_Ptr) COMMA()                                        \
                         FILE_AND_LINE_FOR_REF());                                                 \
}
/**************************************************************************************************/
#define FLOUKA_INCREMENT_COUNTER(counterID)                                                        \
{                                                                                                  \
    flouka_incrementCounter((g_flouka_Ptr),                                                        \
                            (counterID) COMMA()                                                    \
                            FILE_AND_LINE_FOR_REF());                                              \
}
/**************************************************************************************************/
#define FLOUKA_UPDATE_COUNTER(counterID,                                                           \
                              delta)                                                               \
{                                                                                                  \
    flouka_updateCounter((g_flouka_Ptr),                                                           \
                         (counterID),                                                              \
                         (delta) COMMA()                                                           \
                         FILE_AND_LINE_FOR_REF());                                                 \
}
/**************************************************************************************************/
#define FLOUKA_SET_COUNTER(counterID,                                                              \
                           value)                                                                  \
{                                                                                                  \
    flouka_setCounter((g_flouka_Ptr),                                                              \
                      (counterID),                                                                 \
                      (value) COMMA()                                                              \
                      FILE_AND_LINE_FOR_REF());                                                    \
}
/**************************************************************************************************/
#define FLOUKA_GET_COUNTER(counterID)                                                              \
    flouka_getCounter((g_flouka_Ptr),                                                              \
                      (counterID) COMMA()                                                          \
                      FILE_AND_LINE_FOR_REF());
/**************************************************************************************************/

#endif /* FLOUKA_WRAPPER_H_ */
