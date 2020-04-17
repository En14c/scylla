#ifndef __STRING_H
#define __STRING_H

#include <types.h>


s32_t kstrncmp(char *, char *, u32_t);
s32_t kmemncmp(void *, void *, u32_t);
u32_t kstrlen(char *);
void* kmemcpy(void *dest, void *src, u32_t);

#endif /* __STRING_H */
