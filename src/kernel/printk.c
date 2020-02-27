#include <stddef.h>
#include <stdarg.h>

#include <types.h>

#include <drivers/video.h>

#include <ascii.h>
#include <compiler.h>



/*
 * - convert an integer to string in the format of the specified base
 * - supported base:
 *   `-+ base 10
 *     + base 16
*/
#define inttostr(num, base, buf)                                  \
    ({                                                            \
         char _nums[] = "0123456789abcdef";                       \
         u32_t _tmp_len, _len = 1;                                \
         typeof((num)) _rem, _tmp_num = (num);                    \
                                                                  \
         /* get length of the string representation for (num) */  \
         for (;_tmp_num >= (base);_len++, _tmp_num /= (base));    \
         _tmp_len = _len;                                         \
                                                                  \
         for (;(num) >= (base);_len--, (num) /= (base)) {         \
             _rem = (num) % (base);                               \
             (buf)[_len - 1] = _nums[_rem];                       \
         }                                                        \
         (buf)[_len - 1] = _nums[(num)];                          \
         (buf)[_tmp_len] = '\x00';                                \
    })


/*
 * - simple printf implementation that will aid in debugging, logging, etc...
 * - needs more improvement (ex: hanlde floating point numbers, etc...)
*/
void printk(const char *fmt, ...)
{
    char num_strrepr[sizeof(u64_t) * 2];
    va_list argp;
    char _char, *strp;
    u32_t dval, xval;

    va_start(argp, fmt);
    while ((_char = *fmt++)) {
        if (_char == ASCII_PERCENT_CHAR_CODE) {
            _char = *fmt++;
            
            switch (_char) {
                case 'd':
                    dval = va_arg(argp, int);
                    if ((s32_t)dval < 0) {
                        dval = 0xffffffff - dval + 1;
                        prntchar('-');
                    }
                    inttostr(dval, 10, num_strrepr);
                    prntstr(num_strrepr);
                    ((u64_t *)num_strrepr)[0] = (u64_t)0x0;
                    ((u64_t *)num_strrepr)[1] = (u64_t)0x0;
                    break;
                case 'x':
                    xval = va_arg(argp, int);
                    inttostr(xval, 16, num_strrepr);
                    prntstr("0x");
                    prntstr(num_strrepr);
                    ((u64_t *)num_strrepr)[0] = (u64_t)0x0;
                    ((u64_t *)num_strrepr)[1] = (u64_t)0x0;
                    break;
                case 's':
                    strp = va_arg(argp, char *);
                    prntstr(strp);
                    break;
                case 'c':
                    // va_arg takes only fully promoted types so type cast
                    _char = (char) va_arg(argp, int);
                    prntchar(_char);
                    break;
            }
        } else {
            prntchar(_char);
        }
    }
    va_end(argp);
}