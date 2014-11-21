/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <libARSAL/ARSAL_Socket.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Print.h>

#define PORT        0x1234
#define HOST        "localhost"
#define MSG         "HELLO WORLD !!!"
#define DIRSIZE     8192

#define __TAG__ "test2"

void *thread_client(void *arg)
{
    char hostname[100];
    char dir[DIRSIZE];
    int sd;
    struct sockaddr_in pin;
    struct hostent *hp;

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "%s started\n", __FUNCTION__);
    strcpy(hostname,HOST);

    /* go find out about the desired host machine */
    if ((hp = gethostbyname(hostname)) == 0)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "gethostbyname : %s\n", strerror(errno));
        return NULL;
    }

    /* fill in the socket structure with host information */
    memset(&pin, 0, sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
    pin.sin_port = htons(PORT);

    /* grab an Internet domain socket */
    if ((sd = ARSAL_Socket_Create(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Create : %s\n", strerror(errno));
        return NULL;
    }

    /* connect to PORT on HOST */
    if (ARSAL_Socket_Connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Connect : %s\n", strerror(errno));
        return NULL;
    }

    /* send a message to the server PORT on machine HOST */
    if (ARSAL_Socket_Send(sd, MSG, strlen(MSG), 0) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Send : %s\n", strerror(errno));
        return NULL;
    }

    /* wait for a message to come back from the server */
    if (ARSAL_Socket_Recv(sd, dir, DIRSIZE, 0) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Recv : %s\n", strerror(errno));
        return NULL;
    }

    /* spew-out the results and bail out of here! */
    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "Message received : %s\n", dir);

    ARSAL_Socket_Close(sd);

    return NULL;
}

void *thread_server(void *arg)
{
    char     dir[DIRSIZE];  /* used for incomming dir name, and
                               outgoing data */
    int      sd, sd_current;
    unsigned int     addrlen;
    struct   sockaddr_in sin;
    struct   sockaddr_in pin;

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "%s started\n", __FUNCTION__);
    /* grab an Internet domain socket */
    if ((sd = ARSAL_Socket_Create(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Create : %s\n", strerror(errno));
        return NULL;
    }

    /* fill in the socket structure with host information */
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    /* bind the socket to the port number */
    if (ARSAL_Socket_Bind(sd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Bind : %s\n", strerror(errno));
        return NULL;
    }

    /* show that we are willing to listen */
    if (ARSAL_Socket_Listen(sd, 5) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Listen : %s\n", strerror(errno));
        return NULL;
    }

    /* wait for a client to talk to us */
    addrlen = sizeof(pin);
    if ((sd_current = ARSAL_Socket_Accept(sd, (struct sockaddr *)&pin, &addrlen)) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Accept : %s\n", strerror(errno));
        return NULL;
    }

    /* get a message from the client */
    if (ARSAL_Socket_Recv(sd_current, dir, sizeof(dir), 0) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Recv : %s\n", strerror(errno));
        return NULL;
    }

    /* spew-out the results and bail out of here! */
    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "Message received : %s\n", dir);

    /* acknowledge the message, reply w/ the file names */
    if (ARSAL_Socket_Send(sd_current, dir, strlen(dir), 0) == -1)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "ARSAL_Socket_Send : %s\n", strerror(errno));
        return NULL;
    }

    /* close up both sockets */
    ARSAL_Socket_Close(sd_current);
    ARSAL_Socket_Close(sd);

    /* give client a chance to properly shutdown */
    sleep(1);

    return NULL;
}

int main(int argc, char **argv)
{
    ARSAL_Thread_t server, client;

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "create threads\n");
    ARSAL_Thread_Create(&server, thread_server, NULL);
    ARSAL_Thread_Create(&client, thread_client, NULL);

    ARSAL_Thread_Join(server, NULL);
    ARSAL_Thread_Join(client, NULL);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "destroy threads\n");
    ARSAL_Thread_Destroy(&server);
    ARSAL_Thread_Destroy(&client);
    return 0;
}
