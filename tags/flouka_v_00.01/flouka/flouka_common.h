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
#ifndef FLOUKA_COMMON_H_
#define FLOUKA_COMMON_H_


/***************************************************************************************************
 *
 *  M A C R O S
 *
 **************************************************************************************************/


#ifndef FALSE
#define FALSE                           0
#endif

#ifndef TRUE
#define TRUE                            1
#endif

#ifndef INLINE
#define INLINE __inline__
#endif

#ifndef STATIC
#define STATIC static
#endif

#ifdef DEBUG

#define FILE_NAME                       fileName
#define LINE_NUMBER                     lineNumber
#define FILE_AND_LINE_FOR_TYPE()        const char*  FILE_NAME, uint32 LINE_NUMBER
#define FILE_AND_LINE_FOR_CALL()        FILE_NAME, LINE_NUMBER
#define FILE_AND_LINE_FOR_REF()        __FILE__,  __LINE__
#define COMMA()                         ,
#define ASSERT(condition, message, file, line)                                                     \
{                                                                                                  \
    uint8* ptr = NULL;                                                                           \
    if(FALSE == (condition))                                                                       \
    {                                                                                              \
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");\
        printf("!! !SSERTION F!ILED\n");                                                           \
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");\
        printf("!! File: %s\n", __FILE__);                                                         \
        printf("!! Line: %d\n", __LINE__);                                                         \
        printf("!! Function: %s\n", __FUNCTION__);                                                 \
        printf("!! Message: %s\n", (message));                                                     \
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");\
        printf("!! Caller file: %s\n", (file));                                                    \
        printf("!! Caller line: %d\n", ((int)line));                                               \
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");\
        *ptr = 0;                                                                                  \
    } /*if(Assertion Failed)*/                                                                     \
}
#else

#define FILE_NAME
#define LINE_NUMBER
#define FILE_AND_LINE_FOR_TYPE()
#define FILE_AND_LINE_FOR_CALL()
#define FILE_AND_LINE_FOR_REF()
#define COMMA()
#define ASSERT(condition, message, file, line)

#endif

#define LENGTH_HEADER_SIZE 4



/***************************************************************************************************
 *
 *  T Y P E S
 *
 **************************************************************************************************/
typedef unsigned long  uint32;
typedef signed   long  int32;

typedef unsigned short uint16;
typedef signed   short int16;

typedef unsigned char uint8;
typedef signed   char int8;

typedef uint8 bool;

/***************************************************************************************************
 *
 *  I N T E R F A C E   F U N C T I O N   D E C L A R A T I O N S
 *
 **************************************************************************************************/


#endif /* FLOUKA_COMMON_H_ */
