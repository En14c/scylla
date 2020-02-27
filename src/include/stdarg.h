/* see linux stdarg(3) for more info */
#ifndef __STDARG_H
#define __STDARG_H

typedef char* va_list;

/*
 * amount of space required in the stack for an
 * argument of type (t).
*/
#define __va_sz(t) \
    (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

/*
 * - initialize the variable (ap) for subsequent use by var_arg and var_end
 *    macros.
 * - set the variable (ap) to point to the first argument after (larg).
 * - (larg) is the name of the last argument before the variable argument list,
 *   that is the last argument of which the calling function knows the type.
 *   [ex] foo_fun(char *arg0, ...) arg0 here is larg (cdecl calling convention)
*/
#define va_start(ap, larg) \
    ((ap) = ((va_list) (&larg) + __va_sz(larg)))


/*
 * increment (ap) to point to the next argument in the list, then return
 * a pointer to what (ap) pointed to at first.
*/
#define va_arg(ap, t) \
    (((ap) = (ap) +  __va_sz(t)), *((t*)((ap) - __va_sz(t))))

/*
 * end the processing of the variable argument list.
*/
#define va_end(ap) ((void)0)

#endif /* __STDARG_H */
