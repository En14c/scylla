#ifndef __STRING_H
#define __STRING_H

#include <types.h>


extern s32_t kstrncmp(char *, char *, u32_t);
extern s32_t kmemncmp(void *, void *, u32_t);
extern u32_t kstrlen(char *);
extern void* kmemcpy(void *, void *, u32_t);
extern void* kmemset(void *, u8_t, u32_t);

#endif /* __STRING_H */
