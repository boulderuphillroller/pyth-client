#ifndef _pyth_oracle_util_log2_h_
#define _pyth_oracle_util_log2_h_

/* Portable robust integer log base 2 (i.e. find most significant bit).
   Define PYTH_ORACLE_UTIL_LOG2_STYLE to indicate the implementation
   style:
     0 - portable
     1 - use gcc's compiler builtins assuming uint32_t/uint64_t is an
         "unsigned int"/"unsigned long".
   Default is 0. */

#include "compat_stdint.h"

#ifndef PYTH_ORACLE_UTIL_LOG2_STYLE
#define PYTH_ORACLE_UTIL_LOG2_STYLE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Compute z = floor( log_2( x ) ) for positive x (i.e. return the
   index of the most significant bit set in x.  Exact.  Undefined
   behavior if x is not positive (can depend on the implementation
   style).  For the portable style, the cost is a O(log_2 N) fast
   integer ops where N is bit width of x. */

#if PYTH_ORACLE_UTIL_LOG2_STYLE==0

static inline int          /* In [0,7] */
log2_uint8( uint8_t _x ) { /* Positive */
  uint32_t x = (uint32_t)_x;
  int z = 0;
  if( x >= (UINT32_C(1)<< 4) ) z +=  4, x >>=  4;
  if( x >= (UINT32_C(1)<< 2) ) z +=  2, x >>=  2;
  if( x >= (UINT32_C(1)<< 1) ) z +=  1;
  return z;
}

static inline int            /* In [0,15] */
log2_uint16( uint16_t _x ) { /* Positive */
  uint32_t x = (uint32_t)_x;
  int z = 0;
  if( x >= (UINT32_C(1)<< 8) ) z +=  8, x >>=  8;
  if( x >= (UINT32_C(1)<< 4) ) z +=  4, x >>=  4;
  if( x >= (UINT32_C(1)<< 2) ) z +=  2, x >>=  2;
  if( x >= (UINT32_C(1)<< 1) ) z +=  1;
  return z;
}

static inline int           /* In [0,31] */
log2_uint32( uint32_t x ) { /* Positive */
  int z = 0;
  if( x >= (UINT32_C(1)<<16) ) z += 16, x >>= 16;
  if( x >= (UINT32_C(1)<< 8) ) z +=  8, x >>=  8;
  if( x >= (UINT32_C(1)<< 4) ) z +=  4, x >>=  4;
  if( x >= (UINT32_C(1)<< 2) ) z +=  2, x >>=  2;
  if( x >= (UINT32_C(1)<< 1) ) z +=  1;
  return z;
}

static inline int           /* In [0,63] */
log2_uint64( uint64_t x ) { /* Positive */
  int z = 0;
  if( x >= (UINT64_C(1)<<32) ) z += 32, x >>= 32;
  if( x >= (UINT64_C(1)<<16) ) z += 16, x >>= 16;
  if( x >= (UINT64_C(1)<< 8) ) z +=  8, x >>=  8;
  if( x >= (UINT64_C(1)<< 4) ) z +=  4, x >>=  4;
  if( x >= (UINT64_C(1)<< 2) ) z +=  2, x >>=  2;
  if( x >= (UINT64_C(1)<< 1) ) z +=  1;
  return z;
}

#elif PYTH_ORACLE_UTIL_LOG2_STYLE==1

static inline int log2_uint8 ( uint8_t  x ) { return 31-__builtin_clz ( (unsigned int )x ); }
static inline int log2_uint16( uint16_t x ) { return 31-__builtin_clz ( (unsigned int )x ); }
static inline int log2_uint32( uint32_t x ) { return 31-__builtin_clz ( (unsigned int )x ); }
static inline int log2_uint64( uint64_t x ) { return 63-__builtin_clzl( (unsigned long)x ); }

#else
#error "Unsupported PYTH_ORACLE_UTIL_LOG2_STYLE"
#endif

static inline int /* In [0, 6] */ log2_int8 ( int8_t  x /* positive */ ) { return log2_uint8 ( (uint8_t )x ); }
static inline int /* In [0,14] */ log2_int16( int16_t x /* positive */ ) { return log2_uint16( (uint16_t)x ); }
static inline int /* In [0,30] */ log2_int32( int32_t x /* positive */ ) { return log2_uint32( (uint32_t)x ); }
static inline int /* In [0,62] */ log2_int64( int64_t x /* positive */ ) { return log2_uint64( (uint64_t)x ); }

/* Same as the above but these variants return default when x is zero
   such that they have no undefined behavior */

static inline int log2_uint8_def ( uint8_t  x, int def ) { return x ? log2_uint8 ( x ) : def; }
static inline int log2_uint16_def( uint16_t x, int def ) { return x ? log2_uint16( x ) : def; }
static inline int log2_uint32_def( uint32_t x, int def ) { return x ? log2_uint32( x ) : def; }
static inline int log2_uint64_def( uint64_t x, int def ) { return x ? log2_uint64( x ) : def; }

static inline int log2_int8_def ( int8_t  x, int def ) { return (x>(int8_t )0) ? log2_int8 ( x ) : def; }
static inline int log2_int16_def( int16_t x, int def ) { return (x>(int16_t)0) ? log2_int16( x ) : def; }
static inline int log2_int32_def( int32_t x, int def ) { return (x>(int32_t)0) ? log2_int32( x ) : def; }
static inline int log2_int64_def( int64_t x, int def ) { return (x>(int64_t)0) ? log2_int64( x ) : def; }

#ifdef __cplusplus
}
#endif

#endif /* _pyth_oracle_util_log2_h_ */
