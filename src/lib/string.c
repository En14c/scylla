#include <compiler.h>
#include <types.h>
#include <stddef.h>

/*
 * [+] compares the strings pointed to by @str1, @str2 lexicographically.
 * [+] returns an integer that is
 *       - less than zero if str1 is less than str2.
 *       - equal to zero if str1 equals str2
 *       - greater than zero if str1 is greater than str2
 * [+] if the length is not valid then the result is an arbitrary value
 *     from the stack
*/
s32_t
kstrncmp (char *str1, char *str2, u32_t len)
{
#define KSTRNCMP_MAX_LEN 0x2000

    s32_t _diff;
    const u8_t *_ptr1, *_ptr2;

    _ptr1 = (const u8_t *)str1;
    _ptr2 = (const u8_t *)str2;

    if (len == 0 || len > KSTRNCMP_MAX_LEN)
        goto _end_kstrncmp;

    for (_diff = 0;
            *_ptr1 && *_ptr2 && !_diff && len;
            len--, _ptr1++, _ptr2++)
        _diff = *_ptr1 - *_ptr2;

_end_kstrncmp:
    return _diff;
}


/*
 * [+] compares the memory pointed to by @mem1, @mem2.
 * [+] returns
 *     - (-1) if the contents of the memory pointed to by @mem1, mem2
 *       does not match
 *     - (0) if the contents of the memory pointed to by @mem1, mem2
 *       match
 * [+] if the length is not valid then the result is an arbitrary value
 *     from the stack
*/
s32_t
kmemncmp (void *mem1, void *mem2, u32_t len)
{
#define KMEMNCMP_MAX_LEN 0x2000

    s32_t _ret;
    const u8_t *_ptr1, *_ptr2;

    _ptr1 = (const u8_t *)mem1;
    _ptr2 = (const u8_t *)mem2;

    if (len == 0 || len > KMEMNCMP_MAX_LEN)
        goto _end_kmemncmp;

    for (_ret = 0;len; len--, _ptr1++, _ptr2++) {
        if (*_ptr1 == *_ptr2)
            continue;
        _ret = -1;
        break;
    }

_end_kmemncmp:
    return _ret;
}


/*
 * [+] copy chunk of data /w length @len from memory pointed to
 *     by @src to memory pointed to by @dest.
 * [+] return values
 *     - (NULL) on errors if @len equals zero or is greater than some
 *       specific value
 *     - (pointer to dest) on success
*/
void*
kmemcpy (void *dest, void *src, u32_t len)
{
#define KMEMCPY_MAX_LEN 0x2000

    char *_dest;
    const char *_src;
    s32_t _err;
 
    _dest = dest;
    _src  = src;
    _err  = 0;

    if (len == 0 || len > KMEMCPY_MAX_LEN) {
        _err = 1;
        goto _end_kmemcpy;
    }

    for (;len; len--, _dest++, _src++)
        *_dest = *_src;

_end_kmemcpy:
    return _err ? NULL : dest; 
}


/*
 * get the length of the string pointed to by @str
*/
u32_t
kstrlen (char *str)
{
#define KSTRLEN_MAX_LEN 0x2000

    u32_t _len;
    const char *_str = (const char *)str;

    for (_len = 0;*_str != '\x00' && _len <= KSTRLEN_MAX_LEN;
            _len++, _str++);

   return _len;
}
