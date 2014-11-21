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
// Define to force the testbench to beleive that the drone is big endian
#define INVERT_DEVICE_ENDIANNESS (0)

#ifdef __APPLE__
# include <architecture/byte_order.h>
# include <libkern/OSByteOrder.h>
# define __LITTLE_ENDIAN 1234
# define __BIG_ENDIAN 4321
# define __PDP_ENDIAN 3412
# ifdef __BIG_ENDIAN__
#  define __BYTE_ORDER __BIG_ENDIAN
# elif defined (__LITTLE_ENDIAN__)
#  define __BYTE_ORDER __LITTLE_ENDIAN
# else
#  define __BYTE_ORDER __PDP_ENDIAN
# endif
#else
# include <endian.h>
#endif

#if INVERT_DEVICE_ENDIANNESS
#define __DEVICE_ENDIAN __BIG_ENDIAN
#endif

// Header we want to test
#include <libARSAL/ARSAL_Endianness.h>
// Use ARSAL_PRINT
#include <libARSAL/ARSAL_Print.h>

// Host endian values
#define HE_I16 0x1234
#define HE_I32 0x12345678l
#define HE_I64 0x1234567890abcdefll

// Drone endian values
#if __BYTE_ORDER == __DEVICE_ENDIAN
#define DE_I16 HE_I16
#define DE_I32 HE_I32
#define DE_I64 HE_I64
#elif __BYTE_ORDER == __INVER_ENDIAN
#define DE_I16 0x3412
#define DE_I32 0x78563412l
#define DE_I64 0xefcdab9078563412ll
#else
#error PDP ENDIANNESS NOT SUPPORTED
#endif

#define TEST_FAILED (0)
#define TEST_OK (1)

// Dump functions
void
dumpi16 (uint16_t val)
{
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "0x%04x\n", val);
}

void
dumpi32 (uint32_t val)
{
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "0x%08x\n", val);
}

void
dumpi64 (uint64_t val)
{
    uint32_t HI, LO;
    HI = (uint32_t) (val >> 32);
    LO = (uint32_t) (val & 0x0FFFFFFFFll);
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "0x%08x%08x\n", HI, LO);
}

// Test functions
int testInt16 (int prevRes)
{
    uint16_t init, drone, host;
    if (TEST_FAILED == prevRes)
    {
        return TEST_FAILED;
    }

    init = HE_I16;
    drone = htods (init);
    if (drone != DE_I16)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Host -> Drone conversion failed for 2 byte int\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi16 (DE_I16);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi16 (drone);
        return TEST_FAILED;
    }
    host = dtohs (drone);
    if (host != init)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Drone -> Host conversion failed for 2 byte int\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi16 (init);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi16 (host);
        return TEST_FAILED;
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Endianness conversion succeded for 2 byte int\n");
    return TEST_OK;
}

int testInt32 (int prevRes)
{
    uint32_t init, drone, host;
    if (TEST_FAILED == prevRes)
    {
        return TEST_FAILED;
    }

    init = HE_I32;
    drone = htodl (init);
    if (drone != DE_I32)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Host -> Drone conversion failed for 4 byte int\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi32 (DE_I32);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi32 (drone);
        return TEST_FAILED;
    }
    host = dtohl (drone);
    if (host != init)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Drone -> Host conversion failed for 4 byte int\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi32 (init);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi32 (host);
        return TEST_FAILED;
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Endianness conversion succeded for 4 byte int\n");
    return TEST_OK;
}

int testInt64 (int prevRes)
{
    uint64_t init, drone, host;
    if (TEST_FAILED == prevRes)
    {
        return TEST_FAILED;
    }

    init = HE_I64;
    drone = htodll (init);
    if (drone != DE_I64)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Host -> Drone conversion failed for 8 byte int\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi64 (DE_I64);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi64 (drone);
        return TEST_FAILED;
    }
    host = dtohll (drone);
    if (host != init)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Drone -> Host conversion failed for 8 byte int\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi64 (init);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi64 (host);
        return TEST_FAILED;
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Endianness conversion succeded for 8 byte int\n");
    return TEST_OK;
}

typedef union {
    uint32_t i;
    float f;
} floati;

int testFloat (int prevRes)
{
    floati init, drone, host;
    if (TEST_FAILED == prevRes)
    {
        return TEST_FAILED;
    }

    init.i = HE_I32;
    drone.f = htodf (init.f);
    if (drone.i != DE_I32)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Host -> Drone conversion failed for IEEE-754 Float\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi32 (HE_I32);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi32 (drone.i);
        return TEST_FAILED;
    }
    host.f = dtohf (drone.f);
    if (host.i != init.i)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Drone -> Host conversion failed for IEEE-754 Float\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi32 (init.i);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi32 (host.i);
        return TEST_FAILED;
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Endianness conversion succeded for IEEE-754 Float\n");
    return TEST_OK;
}

typedef union {
    uint64_t i;
    double d;
} doublei;

int testDouble (int prevRes)
{
    doublei init, drone, host;
    if (TEST_FAILED == prevRes)
    {
        return TEST_FAILED;
    }

    init.i = HE_I64;
    drone.d = htodd (init.d);
    if (drone.i != DE_I64)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Host -> Drone conversion failed for IEEE-754 Double\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi64 (DE_I64);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi64 (drone.i);
        return TEST_FAILED;
    }
    host.d = dtohd (drone.d);
    if (host.i != init.i)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Drone -> Host conversion failed for IEEE-754 Double\n");
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Expected : \n");
        dumpi64 (init.i);
        ARSAL_PRINT (ARSAL_PRINT_ERROR, "testEndianness", "Got      : \n");
        dumpi64 (host.i);
        return TEST_FAILED;
    }
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Endianness conversion succeded for IEEE-754 Double\n");
    return TEST_OK;
}

#define lestr "Little"
#define bestr "Big   "
#define oestr "Other "

int
main (int argc, char *argv[])
{
// Set host endian string
#if __BYTE_ORDER == __LITTLE_ENDIAN
    const char *hostE = lestr;
#elif __BYTE_ORDER == __BIG_ENDIAN
    const char *hostE = bestr;
#else
    const char *hostE = oestr;
#endif
// Set drone endian string
#if __DEVICE_ENDIAN == __LITTLE_ENDIAN
    const char *droneE = lestr;
#elif __DEVICE_ENDIAN == __BIG_ENDIAN
    const char *droneE = bestr;
#else
    const char *droneE = oestr;
#endif
// Show both endianness
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Host  Endianness : %s\n", hostE);
    ARSAL_PRINT (ARSAL_PRINT_WARNING, "testEndianness", "Drone Endianness : %s\n", droneE);

    int res = TEST_OK;
    // Test for 2 bytes int
    res = testInt16 (res);
    // Test for 4 bytes int
    res = testInt32 (res);
    // Test for 4 bytes int
    res = testInt64 (res);
    // Test for 4 bytes int
    res = testFloat (res);
    // Test for 4 bytes int
    res = testDouble (res);

    if (TEST_OK == res)
    {
        return 0;
    }
    return 1;
}
