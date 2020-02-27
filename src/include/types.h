#ifndef __TYPES_H
#define __TYPES_H


typedef int bool;


#if defined(__X86_ARCH_TARGET)

/*
 * typedefs for intel x86
*/
typedef signed char  s8_t;
typedef signed short s16_t;
typedef signed int   s32_t;
typedef signed long long s64_t;

typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;
typedef unsigned long long u64_t;

#endif /* __X86_ARCH_TARGET */

#endif /* __TYPES_H */
