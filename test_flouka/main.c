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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "flouka.h"
#include "flouka_wrapper.h"


typedef enum GroupID
{
    GROUP_ID_TRANSMISSION = 0,
    GROUP_ID_RECEPTION    = 1,
    GROUP_ID_COUNT        = 2
}GroupID_e;

typedef enum SubGroupID
{
    SUB_GROUP_ID_TX_CONNECTION1    = 0,
    SUB_GROUP_ID_TX_CONNECTION2    = 1,
    SUB_GROUP_ID_RX_CONNECTION1    = 2,
    SUB_GROUP_ID_COUNT             = 3
}SubGroupID_e;

typedef enum CounterID
{
    COUNTER_ID_TRANSMISSION_BYTES_COUNT1 = 0,
    COUNTER_ID_TRANSMISSION_FAILURE1     = 1,
    COUNTER_ID_TRANSMISSION_BYTES_COUNT2 = 2,
    COUNTER_ID_TRANSMISSION_FAILURE2     = 3,
    COUNTER_ID_RECEPTION_BYTES_COUNT1    = 4,
    COUNTER_ID_COUNT                     = 5

}CounterID_e;

void unlock();
void lock();
void* alloc(size_t size);

void test_flouka();

flouka_s* g_flouka_Ptr = NULL;

int main()
{
    /*
     * Create the statistics counter.
     */

    FLOUKA_INIT((uint32_t)GROUP_ID_COUNT,       /*Number of groups*/
                (uint32_t)SUB_GROUP_ID_COUNT,   /*Number of sub groups*/
                (uint32_t)COUNTER_ID_COUNT,     /*Number of counters*/
                alloc,                          /*Allocation function*/
                free,                           /*Deallocation function*/
                lock,                           /*Locking function*/
                unlock);                        /*Unlocking function*/

    /*
     * Assign the group(s).
     */
    FLOUKA_ASSIGN_GROUP((uint32_t) GROUP_ID_TRANSMISSION,
                        "Transmission",
                        "This group collects all the counters associated with the transmission path");

    FLOUKA_ASSIGN_GROUP((uint32_t) GROUP_ID_RECEPTION,
                        "Reception",
                        "This group collects all the counters associated with the reception path");

    /*
     * Assign the sub group(s).
     */
    FLOUKA_ASSIGN_SUB_GROUP((uint32_t) SUB_GROUP_ID_TX_CONNECTION1,
                            (uint32_t) GROUP_ID_TRANSMISSION,
                            "Transmission Connection 1",
                            "This sub group collects all the counters associated with the transmission path for connection 1");

    FLOUKA_ASSIGN_SUB_GROUP((uint32_t) SUB_GROUP_ID_TX_CONNECTION2,
                            (uint32_t) GROUP_ID_TRANSMISSION,
                            "Transmission Connection 2",
                            "This sub group collects all the counters associated with the transmission path for connection 2");

    FLOUKA_ASSIGN_SUB_GROUP((uint32_t) SUB_GROUP_ID_RX_CONNECTION1,
                            (uint32_t) GROUP_ID_RECEPTION,
                            "Reception Connection 2",
                            "This sub group collects all the counters associated with the reception path for connection 1");

    /*
     * Assign the counter(s)
     */
    FLOUKA_ASSIGN_COUNTER((uint32_t) COUNTER_ID_TRANSMISSION_FAILURE1,
                          (uint32_t) SUB_GROUP_ID_TX_CONNECTION1,
                          "TX Failure(s)",
                          "# Transmission failure",
                          "This counter represents the number of transmission failure");

    FLOUKA_ASSIGN_COUNTER((uint32_t) COUNTER_ID_TRANSMISSION_BYTES_COUNT1,
                          (uint32_t) SUB_GROUP_ID_TX_CONNECTION1,
                          "Byte(s)",
                          "# Bytes transmitted",
                          "This counter represents the number of bytes transmitted");

    FLOUKA_ASSIGN_COUNTER((uint32_t) COUNTER_ID_TRANSMISSION_FAILURE2,
                          (uint32_t) SUB_GROUP_ID_TX_CONNECTION2,
                          "TX Failure(s)",
                          "# Transmission failure",
                          "This counter represents the number of transmission failure");

    FLOUKA_ASSIGN_COUNTER((uint32_t) COUNTER_ID_TRANSMISSION_BYTES_COUNT2,
                          (uint32_t) SUB_GROUP_ID_TX_CONNECTION2,
                          "Byte(s)",
                          "# Bytes transmitted",
                          "This counter represents the number of bytes transmitted");

    FLOUKA_ASSIGN_COUNTER((uint32_t) COUNTER_ID_RECEPTION_BYTES_COUNT1,
                          (uint32_t) SUB_GROUP_ID_RX_CONNECTION1,
                          "Byte(s)",
                          "# Bytes received",
                          "This counter represents the number of bytes received");



    test_flouka();

    return 0;
}

void lock()
{
    /*
     * The test assumes counters/groups assignment occur from the same thread, and that's why this
     * function has no implementation. But if this is not the case, then this function shall call
     * the appropriate locking function for example:
     *
     * pthread_mutex_lock()
     */
}

void unlock()
{
    /*
     * The test assumes counters/groups assignment occur from the same thread, and that's why this
     * function has no implementation. But if this is not the case, then this function shall call
     * the appropriate locking function for example:
     *
     * pthread_mutex_unlock()
     */
}

void* alloc(size_t size)
{
    /*
     * The function shall initialize the allocated memory to zero.
     */
    return memset(malloc(size), 0, size);
}

void test_flouka()
{
    int32_t             listenSocket;
    int32_t             connectSocket;
    socklen_t           clientAddressLength;
    struct  sockaddr_in clientAddress;
    struct  sockaddr_in serverAddress;
    uint8_t             request;
    uint8_t*            informationBuffer_Ptr;
    uint32_t            informationBufferSize;
    uint8_t*            statisticsBuffer_Ptr;
    uint32_t            statisticsBufferSize;
    int32_t             status;
    uint16_t            listenPort;

    listenPort = 4444;

    listenSocket = socket(AF_INET,
                          SOCK_STREAM,
                          0);

    serverAddress.sin_family        = AF_INET;
    serverAddress.sin_addr.s_addr   = htonl(INADDR_ANY);
    serverAddress.sin_port          = htons(listenPort);

    bind(listenSocket,
         (struct sockaddr *) &serverAddress,
         sizeof(serverAddress));

    listen(listenSocket,
           1);

    clientAddressLength = sizeof(clientAddress);

    printf("Waiting for client to connect on port (%d)...\n",
           listenPort);

    connectSocket = accept(listenSocket,
                           (struct sockaddr *) &clientAddress,
                           &clientAddressLength);

    printf("Client connected.\n");


    status = recv(connectSocket,
                  &request,
                  sizeof(request),
                  0);

    if(status < 0)
    {
        printf("Client disconnected, server is going to shutdown\n");
        return;
    }
    switch(request)
    {
        case 0:
            printf("Termination requested\n");
            return;
        case 1:
            printf("Information requested\n");
            informationBufferSize = FLOUKA_GET_INFORMATIOM_SIZE();
            informationBuffer_Ptr = (uint8_t*) malloc(informationBufferSize);


            FLOUKA_GET_INFORMATION(informationBuffer_Ptr,
                                   informationBufferSize);

            send(connectSocket,
                 informationBuffer_Ptr,
                 informationBufferSize,
                 0);

            printf("(%lu) bytes of statistics setup information sent.\n",
                   (unsigned long)informationBufferSize);

            free(informationBuffer_Ptr);
            break;
        case 2:
            ASSERT(FALSE,
                   "TEST FLOUKA: Client is expected to request the information first",
                   __FILE__,
                   __LINE__);
            break;
        default:
            ASSERT(FALSE,
                   "TEST FLOUKA: Invalid request received!!!",
                   __FILE__,
                   __LINE__);

            break;
    }


    FLOUKA_GET_STATISTICS(&statisticsBuffer_Ptr,
                          &statisticsBufferSize);

    while(0 != request)
    {
        status = recv(connectSocket,
                      &request,
                      sizeof(request),
                      0);

        if(status < 0)
        {
            printf("Client disconnected, server is going to shutdown\n");
            return;
        }

        switch(request)
        {
            case 0:
                printf("Termination requested\n");
                return;
            case 1:
                printf("Information requested\n");
                ASSERT(FALSE,
                       "TEST FLOUKA: Client is expected to request the statistics",
                       __FILE__,
                       __LINE__);
                break;
            case 2:
                printf("Statistics requested\n");
                send(connectSocket,
                     statisticsBuffer_Ptr,
                     statisticsBufferSize,
                     0);

                /*
                 * Increment/update the counters.
                 */
                FLOUKA_INCREMENT_COUNTER(COUNTER_ID_TRANSMISSION_FAILURE1);
                FLOUKA_INCREASE_COUNTER(COUNTER_ID_TRANSMISSION_BYTES_COUNT1,
                                      1000);

                break;
            default:
                ASSERT(FALSE,
                       "TEST FLOUKA: Invalid request received!!!",
                       __FILE__,
                       __LINE__);
                break;
        }
    }
}
