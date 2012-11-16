/**
 * @file libSAL/endianness.h
 * @brief This file contains headers about endianness abstraction layer
 * @date 11/14/2012
 * @author nicolas.brulez@parrot.com
*/
#ifndef _LIBSAL_ENDIANNESS_H_
#define _LIBSAL_ENDIANNESS_H_

#include <inttypes.h>

#ifndef __APPLE__ // Apple use a different method for endianness handling

#include <endian.h>


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

#else // ifdef __APPLE__

#include <architecture/byte_order.h>
#include <libkern/OSByteOrder.h>

/**
 * @brief Define little endian macro to the same values as linux <endian.h>
 */
#ifndef __LITTLE_ENDIAN
# define __LITTLE_ENDIAN 1234
#endif

/**
 * @brief Define big endian macro to the same values as linux <endian.h>
 */
#ifndef __BIG_ENDIAN
# define __BIG_ENDIAN 4321
#endif

/**
 * @brief Define pdp endian macro to the same values as linux <endian.h>
 */
#ifndef __PDP_ENDIAN
# define __PDP_ENDIAN 3412
#endif

/**
 * @brief AR.Drone endianness
 */
#ifndef __DRONE_ENDIAN
#define __DRONE_ENDIAN __LITTLE_ENDIAN
#endif
#ifndef __INVER_ENDIAN
#define __INVER_ENDIAN __BIG_ENDIAN
#endif

#if __DRONE_ENDIAN == __LITTLE_ENDIAN
/*
 * HOST --> DRONE Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) to drone endianness
 */
#define htods(v) OSSwapHostToLittleInt16(v)
/**
 * @brief Convert a long int (4 bytes) to drone endianness
 */
#define htodl(v) OSSwapHostToLittleInt32(v)
/**
 * @brief Convert a long long int (8 bytes) to drone endianness
 */
#define htodll(v) OSSwapHostToLittleInt64(v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) to drone endianness
 */
#define htodf(v) ARSwapHostToLittleFloat(v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) to drone endianness
 */
#define htodd(v) ARSwapHostToLittleDouble(v)

/*
 * DRONE --> HOST Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) from drone endianness
 */
#define dtohs(v) OSSwapLittleToHostInt16(v)
/**
 * @brief Convert a long int (4 bytes) from drone endianness
 */
#define dtohl(v) OSSwapLittleToHostInt32(v)
/**
 * @brief Convert a long long int (8 bytes) from drone endianness
 */
#define dtohll(v) OSSwapLittleToHostInt64(v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) from drone endianness
 */
#define dtohf(v) ARSwapLittleToHostFloat(v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) from drone endianness
 */
#define dtohd(v) ARSwapLittleToHostDouble(v)

#elif __DRONE_ENDIAN == __BIG_ENDIAN
/*
 * HOST --> DRONE Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) to drone endianness
 */
#define htods(v) OSSwapHostToBigInt16(v)
/**
 * @brief Convert a long int (4 bytes) to drone endianness
 */
#define htodl(v) OSSwapHostToBigInt32(v)
/**
 * @brief Convert a long long int (8 bytes) to drone endianness
 */
#define htodll(v) OSSwapHostToBigInt64(v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) to drone endianness
 */
#define htodf(v) ARSwapHostToBigFloat(v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) to drone endianness
 */
#define htodd(v) ARSwapHostToBigDouble(v)

/*
 * DRONE --> HOST Conversion macros
 */

/**
 * @brief Convert a short int (2 bytes) from drone endianness
 */
#define dtohs(v) OSSwapBigToHostInt16(v)
/**
 * @brief Convert a long int (4 bytes) from drone endianness
 */
#define dtohl(v) OSSwapBigToHostInt32(v)
/**
 * @brief Convert a long long int (8 bytes) from drone endianness
 */
#define dtohll(v) OSSwapBigToHostInt64(v)
/**
 * @brief Convert a IEEE-754 float (4 bytes) from drone endianness
 */
#define dtohf(v) ARSwapBigToHostFloat(v)
/**
 * @brief Convert a IEEE-754 double (8 bytes) from drone endianness
 */
#define dtohd(v) ARSwapBigToHostDouble(v)


#if !defined(OS_INLINE)
# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#        define OS_INLINE static inline
# else
#        define OS_INLINE static __inline__
# endif
#endif

/**
 * @brief INTERNAL_FUNCTION - Swap a float value from host to little endian
 * @param orig Initial value to swap
 * @return little endian value of orig
 */
OS_INLINE
float
ARSwapHostToLittleFloat (float orig)
{
  union fconv {
    float f;
    uint32_t i;
  } u;
  u.f = orig;
  u.i = OSSwapHostToLittleInt32 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a float value from little to host endian
 * @param orig Initial little endian value to swap
 * @return host endian value of orig
 */
OS_INLINE
float
ARSwapLittleToHostFloat (float orig)
{
  union fconv {
    float f;
    uint32_t i;
  } u;
  u.f = orig;
  u.i = OSSwapLittleToHostInt32 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a float value from host to big endian
 * @param orig Initial value to swap
 * @return big endian value of orig
 */
OS_INLINE
float
ARSwapHostToBigFloat (float orig)
{
  union fconv {
    float f;
    uint32_t i;
  } u;
  u.f = orig;
  u.i = OSSwapHostToBigInt32 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a float value from big to host endian
 * @param orig Initial value to swap
 * @return big endian value of orig
 */
OS_INLINE
float
ARSwapBigToHostFloat (float orig)
{
  union fconv {
    float f;
    uint32_t i;
  } u;
  u.f = orig;
  u.i = OSSwapBigToHostInt32 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a double value from host to little endian
 * @param orig Initial value to swap
 * @return little endian value of orig
 */
OS_INLINE
double
ARSwapHostToLittleDouble (double orig)
{
  union fconv {
    double f;
    uint64_t i;
  } u;
  u.f = orig;
  u.i = OSSwapHostToLittleInt64 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a double value from little to host endian
 * @param orig Initial little endian value to swap
 * @return host endian value of orig
 */
OS_INLINE
double
ARSwapLittleToHostDouble (double orig)
{
  union fconv {
    double f;
    uint64_t i;
  } u;
  u.f = orig;
  u.i = OSSwapLittleToHostInt64 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a double value from host to big endian
 * @param orig Initial value to swap
 * @return big endian value of orig
 */
OS_INLINE
double
ARSwapHostToBigDouble (double orig)
{
  union fconv {
    double f;
    uint64_t i;
  } u;
  u.f = orig;
  u.i = OSSwapHostToBigInt64 (u.i);
  return u.f;
}

/**
 * @brief INTERNAL_FUNCTION - Swap a double value from big to host endian
 * @param orig Initial value to swap
 * @return big endian value of orig
 */
OS_INLINE
double
ARSwapBigToHostDouble (double orig)
{
  union fconv {
    double f;
    uint64_t i;
  } u;
  u.f = orig;
  u.i = OSSwapBigToHostInt64 (u.i);
  return u.f;
}

#else
#error Drone PDP endianness is not supported
#endif

#endif // __APPLE__

#endif /* _LIBSAL_ENDIANNESS_H_ */
