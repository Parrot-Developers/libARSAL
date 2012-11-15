// Define to force the testbench to beleive that the drone is big endian
#include <endian.h>
#define FORCE_ENDIANNESS (0)

#if FORCE_ENDIANNESS
#define __DRONE_ENDIAN __BIG_ENDIAN
#define __INVER_ENDIAN __LITTLE_ENDIAN
#endif

// Header we want to test
#include <libSAL/endianness.h>
// Use SAL_PRINT
#include <libSAL/print.h>

// Host endian values
#define HE_I16 0x1234
#define HE_I32 0x12345678l
#define HE_I64 0x1234567890abcdefll

// Drone endian values
#if __BYTE_ORDER == __DRONE_ENDIAN
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
    SAL_PRINT (PRINT_WARNING, "0x%04x\n", val);
}

void
dumpi32 (uint32_t val)
{
    SAL_PRINT (PRINT_WARNING, "0x%08x\n", val);
}

void
dumpi64 (uint64_t val)
{
    uint32_t HI, LO;
    HI = (uint32_t) (val >> 32);
    LO = (uint32_t) (val & 0x0FFFFFFFFll);
    SAL_PRINT (PRINT_WARNING, "0x%08x%08x\n", HI, LO);
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
        SAL_PRINT (PRINT_ERROR, "Host -> Drone conversion failed for 2 byte int\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi16 (DE_I16);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi16 (drone);
        return TEST_FAILED;
    }
    host = dtohs (drone);
    if (host != init)
    {
        SAL_PRINT (PRINT_ERROR, "Drone -> Host conversion failed for 2 byte int\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi16 (init);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi16 (host);
        return TEST_FAILED;
    }
    SAL_PRINT (PRINT_WARNING, "Endianness conversion succeded for 2 byte int\n");
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
        SAL_PRINT (PRINT_ERROR, "Host -> Drone conversion failed for 4 byte int\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi32 (DE_I32);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi32 (drone);
        return TEST_FAILED;
    }
    host = dtohl (drone);
    if (host != init)
    {
        SAL_PRINT (PRINT_ERROR, "Drone -> Host conversion failed for 4 byte int\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi32 (init);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi32 (host);
        return TEST_FAILED;
    }
    SAL_PRINT (PRINT_WARNING, "Endianness conversion succeded for 4 byte int\n");
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
        SAL_PRINT (PRINT_ERROR, "Host -> Drone conversion failed for 8 byte int\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi64 (DE_I64);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi64 (drone);
        return TEST_FAILED;
    }
    host = dtohll (drone);
    if (host != init)
    {
        SAL_PRINT (PRINT_ERROR, "Drone -> Host conversion failed for 8 byte int\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi64 (init);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi64 (host);
        return TEST_FAILED;
    }
    SAL_PRINT (PRINT_WARNING, "Endianness conversion succeded for 8 byte int\n");
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
        SAL_PRINT (PRINT_ERROR, "Host -> Drone conversion failed for IEEE-754 Float\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi32 (HE_I32);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi32 (drone.i);
        return TEST_FAILED;
    }
    host.f = dtohf (drone.f);
    if (host.i != init.i)
    {
        SAL_PRINT (PRINT_ERROR, "Drone -> Host conversion failed for IEEE-754 Float\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi32 (init.i);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi32 (host.i);
        return TEST_FAILED;
    }
    SAL_PRINT (PRINT_WARNING, "Endianness conversion succeded for IEEE-754 Float\n");
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
        SAL_PRINT (PRINT_ERROR, "Host -> Drone conversion failed for IEEE-754 Double\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi64 (DE_I64);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi64 (drone.i);
        return TEST_FAILED;
    }
    host.d = dtohd (drone.d);
    if (host.i != init.i)
    {
        SAL_PRINT (PRINT_ERROR, "Drone -> Host conversion failed for IEEE-754 Double\n");
        SAL_PRINT (PRINT_ERROR, "Expected : \n");
        dumpi64 (init.i);
        SAL_PRINT (PRINT_ERROR, "Got      : \n");
        dumpi64 (host.i);
        return TEST_FAILED;
    }
    SAL_PRINT (PRINT_WARNING, "Endianness conversion succeded for IEEE-754 Double\n");
    return TEST_OK;
}

static const char *lestr = "Little";
static const char *bestr = "Big   ";
static const char *oestr = "Other ";

int
main (int argc, char *argv[])
{
// Set host endian string
#if __BYTE_ORDER == __LITTLE_ENDIAN
    char *hostE = lestr;
#elif __BYTE_ORDER == __BIG_ENDIAN
    char *hostE = bestr;
#else
    char *hostE = oestr;
#endif
// Set drone endian string
#if __DRONE_ENDIAN == __LITTLE_ENDIAN
    char *droneE = lestr;
#elif __DRONE_ENDIAN == __BIG_ENDIAN
    char *droneE = bestr;
#else
    char *droneE = oestr;
#endif
// Show both endianness
    SAL_PRINT (PRINT_WARNING, "Host  Endianness : %s\n", hostE);
    SAL_PRINT (PRINT_WARNING, "Drone Endianness : %s\n", droneE);

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
