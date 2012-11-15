/**
 * @file libSAL/endianness.h
 * @brief This file contains headers about endianness abstraction layer
 * @date 11/14/2012
 * @author nicolas.brulez@parrot.com
*/
#ifndef _LIBSAL_ENDIANNESS_H_
#define _LIBSAL_ENDIANNESS_H_

#include <endian.h>
#include <inttypes.h>

/**
 * @brief AR.Drone endianness
 */
#ifndef __DRONE_ENDIAN
#define __DRONE_ENDIAN __LITTLE_ENDIAN
#endif
#ifndef __INVER_ENDIAN
#define __INVER_ENDIAN __BIG_ENDIAN
#endif


#if __BYTE_ORDER == __DRONE_ENDIAN
/*
 * HOST --> DRONE Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) to drone endianness
 */
#define htods(v) (v)
/**
 * @brief Convert a long int (4 bytes) to drone endianness
 */
#define htodl(v) (v)
/**
 * @brief Convert a long long int (8 bytes) to drone endianness
 */
#define htodll(v) (v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) to drone endianness
 */
#define htodf(v) (v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) to drone endianness
 */
#define htodd(v) (v)

/*
 * DRONE --> HOST Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) from drone endianness
 */
#define dtohs(v) (v)
/**
 * @brief Convert a long int (4 bytes) from drone endianness
 */
#define dtohl(v) (v)
/**
 * @brief Convert a long long int (8 bytes) from drone endianness
 */
#define dtohll(v) (v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) from drone endianness
 */
#define dtohf(v) (v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) from drone endianness
 */
#define dtohd(v) (v)

#elif __BYTE_ORDER == __INVER_ENDIAN
/*
 * HOST --> DRONE Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) to drone endianness
 */
#define htods(v) (__typeof__ (v))_libsal_bswaps((uint16_t)v)
/**
 * @brief Convert a long int (4 bytes) to drone endianness
 */
#define htodl(v) (__typeof__ (v))_libsal_bswapl((uint32_t)v)
/**
 * @brief Convert a long long int (8 bytes) to drone endianness
 */
#define htodll(v) (__typeof__ (v))_libsal_bswapll((uint64_t)v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) to drone endianness
 */
#define htodf(v) (__typeof__ (v))_libsal_bswapf((float)v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) to drone endianness
 */
#define htodd(v) (__typeof__ (v))_libsal_bswapd((double)v)

/*
 * DRONE --> HOST Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) from drone endianness
 */
#define dtohs(v) (__typeof__ (v))_libsal_bswaps((uint16_t)v)
/**
 * @brief Convert a long int (4 bytes) from drone endianness
 */
#define dtohl(v) (__typeof__ (v))_libsal_bswapl((uint32_t)v)
/**
 * @brief Convert a long long int (8 bytes) from drone endianness
 */
#define dtohll(v) (__typeof__ (v))_libsal_bswapll((uint64_t)v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) from drone endianness
 */
#define dtohf(v) (__typeof__ (v))_libsal_bswapf((float)v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) from drone endianness
 */
#define dtohd(v) (__typeof__ (v))_libsal_bswapd((double)v)

/*
 * INTERNAL FUNCTIONS --- DO NOT USE THEM DIRECTLY
 */

/**
 * @brief INTERNAL FUNCTION : Swap byte order of a short int
 * @param orig Initial value
 * @return Swapped value
 */
static inline uint16_t _libsal_bswaps (uint16_t orig)
{
    return ((orig & 0xFF00) >> 8) | ((orig & 0x00FF) << 8);
}

/**
 * @brief INTERNAL FUNCTION : Swap byte order of a long int
 * @param orig Initial value
 * @return Swapped value
 */
static inline uint32_t _libsal_bswapl (uint32_t orig)
{
    return ((orig & 0xFF000000) >> 24) | ((orig & 0x00FF0000) >> 8) | ((orig & 0x0000FF00) << 8) | ((orig & 0x000000FF) << 24);
}

/**
 * @brief INTERNAL FUNCTION : Swap byte order of a long long int
 * @param orig Initial value
 * @return Swapped value
 */
static inline uint64_t _libsal_bswapll (uint64_t orig)
{
    uint32_t _bswap_hi = _libsal_bswapl(orig >> 32);
    uint32_t _bswap_lo = _libsal_bswapl(orig & 0x0FFFFFFFFll);
    return ((uint64_t)_bswap_lo << 32) | (uint64_t)_bswap_hi;
}

/**
 * @brief INTERNAL FUNCTION : Swap byte order of a IEEE-754 float
 * @param orig Initial value
 * @return Swapped value
 */
static inline float _libsal_bswapf (float orig)
{
    uint32_t res = _libsal_bswapl (*(uint32_t*)&orig);
    return *(float *)&res;
}

/**
 * @brief INTERNAL FUNCTION : Swap byte order of a IEEE-754 double
 * @param orig Initial value
 * @return Swapped value
 */
static inline double _libsal_bswapd (double orig)
{
    uint64_t res = _libsal_bswapll (*(uint64_t*)&orig);
    return *(double *)&res;
}

#else // __BYTE_ORDER in neither LITTLE nor BIG endian
#error PDP Byte endianness not supported
#endif // __BYTE_ORDER


#endif /* _LIBSAL_ENDIANNESS_H_ */
