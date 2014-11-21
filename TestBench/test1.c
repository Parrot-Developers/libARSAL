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
#include <libARSAL/ARSAL_Mutex.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARSAL/ARSAL_Print.h>

#define __TAG__ "test1"

ARSAL_Mutex_t mutex;
ARSAL_Cond_t cond;
int variable = 0;

void *routine(void *arg)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "Routine started\n");
    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    variable = 1;
    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex signal\n");
    ARSAL_Cond_Signal(&cond);
    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv)
{
    ARSAL_Thread_t thread;

    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex init\n");
    ARSAL_Mutex_Init(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "condition init\n");
    ARSAL_Cond_Init(&cond);

    ARSAL_PRINT(ARSAL_PRINT_WARNING, __TAG__, "thread create\n");
    ARSAL_Thread_Create(&thread, routine, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "Variable : %d\n", variable);

    ARSAL_Thread_Join(thread, NULL);

    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex lock\n");
    ARSAL_Mutex_Lock(&mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex wait\n");
    ARSAL_Cond_Timedwait(&cond, &mutex, 1000);
    //ARSAL_Cond_Wait(&cond, &mutex);
    ARSAL_PRINT(ARSAL_PRINT_ERROR, __TAG__, "mutex unlock\n");
    ARSAL_Mutex_Unlock(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "Variable : %d\n", variable);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "condition destroy\n");
    ARSAL_Cond_Destroy(&cond);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "mutex destroy\n");
    ARSAL_Mutex_Destroy(&mutex);

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, __TAG__, "thread destroy\n");
    ARSAL_Thread_Destroy(&thread);
    return 0;
}
