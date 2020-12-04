#ifndef __KERNEL_ALLOC_H
#define __KERNEL_ALLOC_H

#include <types.h>
#include <printk.h>
#include <stddef.h>
#include <multiboot.h>
#include <sys/kernel.h>
#include <sys/mm.h>


#define KERNEL_HEAP_NUM_PAGES 0x8
#define KERNEL_HEAP_SIZE KERNEL_HEAP_NUM_PAGES * MM_PAGE_SIZE

#define KERNEL_HEAP_BLOCK_SIZE_ALIGNMENT 0x4

// round (size) to the nearest value for which (f) is a factor
#define kheap_round_size(size, f) \
    ((size) % f != 0 ? size + (f - ((size) % f)) : (size))

typedef u32_t kheap_block_header_t;
typedef u32_t kheap_block_boundary_tag_t;

#define KERNEL_HEAP_BLOCK_USED  0x01
#define KERNEL_HEAP_FIRST_BLOCK 0x02
#define KERNEL_HEAP_BLOCK_SIZE  0xfffffffc 

#define KERNEL_HEAP_MAX_REQUEST_SIZE 0x400

#define KERNEL_HEAP_INVALID_BLOCK_HEADER 0x0
#define KERNEL_HEAP_INVALID_BLOCK_BTAG   KERNEL_HEAP_INVALID_BLOCK_HEADER

#define KERNEL_HEAP_STALE_BLOCK_SIZE \
    KERNEL_HEAP_BLOCK_SIZE_ALIGNMENT + 2 * sizeof(kheap_block_header_t)

#define is_heap_block_used(hblock_header_ptr) \
    (*((kheap_block_header_t *)(hblock_header_ptr)) & KERNEL_HEAP_BLOCK_USED)

#define is_heap_block_free(hblock_header_ptr) \
    (!is_heap_block_used((hblock_header_ptr)))

#define is_heap_next_block_free(hblock_header_ptr)                              \
    is_heap_block_free(                                                         \
        (char *)(hblock_header_ptr) + get_heap_block_size((hblock_header_ptr)))

#define is_heap_prev_block_free(hblock_header_ptr)                        \
    is_heap_block_free(                                                   \
        (char *)(hblock_header_ptr) - sizeof(kheap_block_boundary_tag_t))

#define is_first_heap_block(hblock_header_ptr) \
    (*((kheap_block_header_t *)(hblock_header_ptr)) & KERNEL_HEAP_FIRST_BLOCK)

#define get_heap_block_size(hblock_header_ptr) \
    (*((kheap_block_header_t *)(hblock_header_ptr)) & KERNEL_HEAP_BLOCK_SIZE)

#define init_heap_blocks_implicit_list(hblock_header_ptr, heapsize)            \
    do {                                                                       \
        kheap_block_header_t *_stale_hblock_ptr, *_hptr;                       \
        kheap_block_boundary_tag_t *_stale_hblock_btag_ptr, *_bptr;            \
        size_t _stale_hblock_size;                                             \
        _stale_hblock_size = KERNEL_HEAP_STALE_BLOCK_SIZE;                     \
        _bptr = (kheap_block_boundary_tag_t *)((char *)hblock_header_ptr +     \
            heapsize - sizeof(kheap_block_boundary_tag_t));                    \
        _stale_hblock_ptr = (kheap_block_header_t *)hblock_header_ptr;         \
        _stale_hblock_btag_ptr =                                               \
            (kheap_block_boundary_tag_t *)((char *)_stale_hblock_ptr +         \
                _stale_hblock_size - sizeof(kheap_block_boundary_tag_t));      \
        _hptr = (kheap_block_header_t *)((char *)_stale_hblock_ptr +           \
            _stale_hblock_size);                                               \
        *_hptr = (heapsize - _stale_hblock_size) & ~KERNEL_HEAP_BLOCK_USED;    \
        *_bptr = (heapsize - _stale_hblock_size) & ~KERNEL_HEAP_BLOCK_USED;    \
        *_stale_hblock_ptr = _stale_hblock_size | KERNEL_HEAP_BLOCK_USED;      \
        *_stale_hblock_btag_ptr = _stale_hblock_size | KERNEL_HEAP_BLOCK_USED; \
    } while (0)

#define setup_heap_block(hblock_header_ptr, size, status)              \
    do {                                                               \
        kheap_block_header_t *_hptr, *_nhptr;                          \
        kheap_block_boundary_tag_t *_bptr, *_nbptr;                    \
        size_t curr_block_size, nblock_size;                           \
        curr_block_size = get_heap_block_size(hblock_header_ptr);      \
        _hptr = (kheap_block_header_t *)hblock_header_ptr;             \
        _bptr = (kheap_block_boundary_tag_t *)((char *)_hptr +         \
            size - sizeof(kheap_block_boundary_tag_t));                \
        *_hptr = KERNEL_HEAP_INVALID_BLOCK_HEADER;                     \
        *_bptr = KERNEL_HEAP_INVALID_BLOCK_BTAG;                       \
        *_hptr = size | status;                                        \
        *_bptr = size | status;                                        \
        if (curr_block_size > size) {                                  \
            nblock_size = curr_block_size - size;                      \
            _nhptr = (kheap_block_header_t *)((char *)_hptr + size);   \
            _nbptr = (kheap_block_boundary_tag_t *)((char *)_hptr +    \
                curr_block_size - sizeof(kheap_block_boundary_tag_t)); \
            *_nhptr = KERNEL_HEAP_INVALID_BLOCK_HEADER;                \
            *_nbptr = KERNEL_HEAP_INVALID_BLOCK_BTAG;                  \
            *_nhptr = nblock_size & ~KERNEL_HEAP_BLOCK_USED;           \
            *_nbptr = nblock_size & ~KERNEL_HEAP_BLOCK_USED;           \
        }                                                              \
    } while (0) 

#define heap_block_usable_mem_ptr(hblock_header_ptr)                  \
    ({                                                                \
        u8_t *heap_block_usable_mem_ptr;                              \
        heap_block_usable_mem_ptr =                                   \
            (u8_t *)hblock_header_ptr + sizeof(kheap_block_header_t); \
        (kheap_block_header_t *)heap_block_usable_mem_ptr;            \
    })

#define heap_free_block(hblock_header_ptr)                                    \
    do {                                                                      \
        kheap_block_header_t *_hptr;                                          \
        kheap_block_boundary_tag_t *_bptr;                                    \
        _hptr = (kheap_block_header_t *)hblock_header_ptr;                    \
        _bptr = (kheap_block_boundary_tag_t *)((char *)_hptr +                \
            get_heap_block_size(_hptr) - sizeof(kheap_block_boundary_tag_t)); \
        *_hptr &= ~KERNEL_HEAP_BLOCK_USED;                                    \
        *_bptr &= ~KERNEL_HEAP_BLOCK_USED;                                    \
    } while (0)

#define heap_try_coalesce_next_block(hblock_header_ptr)                       \
    do {                                                                      \
        kheap_block_header_t *_hptr;                                          \
        kheap_block_boundary_tag_t *_bptr;                                    \
        size_t curr_block_size, next_block_size, coalesce_size;               \
        _hptr = (kheap_block_header_t *)hblock_header_ptr;                    \
        _bptr = (kheap_block_boundary_tag_t *)((char *)_hptr +                \
            get_heap_block_size(_hptr) - sizeof(kheap_block_boundary_tag_t)); \
        if (is_heap_next_block_free(hblock_header_ptr)) {                     \
            curr_block_size = get_heap_block_size(_hptr);                     \
            next_block_size = get_heap_block_size(                            \
                (char *)_hptr + curr_block_size);                             \
            coalesce_size = curr_block_size + next_block_size;                \
            *_hptr = coalesce_size & ~KERNEL_HEAP_BLOCK_USED;                 \
            *_bptr = coalesce_size & ~KERNEL_HEAP_BLOCK_USED;                 \
        }                                                                     \
    } while (0)

#define heap_try_coalesce_prev_block(hblock_header_ptr)                       \
    do {                                                                      \
        kheap_block_header_t *_hptr, *_prevblock_hptr;                        \
        kheap_block_boundary_tag_t *_bptr, *_prevblock_btag_ptr;              \
        size_t curr_block_size, prev_block_size, coalesce_size;               \
        _hptr = (kheap_block_header_t *)hblock_header_ptr;                    \
        _bptr = (kheap_block_boundary_tag_t *)((char *)_hptr +                \
            get_heap_block_size(_hptr) - sizeof(kheap_block_boundary_tag_t)); \
        _prevblock_btag_ptr = (kheap_block_boundary_tag_t *)((char *)_hptr -  \
            sizeof(kheap_block_boundary_tag_t));                              \
        if (is_heap_prev_block_free(hblock_header_ptr)) {                     \
            curr_block_size = get_heap_block_size(hblock_header_ptr);         \
            prev_block_size = get_heap_block_size(_prevblock_btag_ptr);       \
            coalesce_size = curr_block_size + prev_block_size;                \
            _prevblock_hptr = (kheap_block_header_t *)(                       \
                (char *)_hptr - prev_block_size);                             \
            *_prevblock_hptr = coalesce_size & ~KERNEL_HEAP_BLOCK_USED;       \
            *_bptr = coalesce_size & ~KERNEL_HEAP_BLOCK_USED;                 \
        }                                                                     \
    } while (0)

#define kernel_heap_allocater_failure_handler() \
    kernel_panic("[FATAL ERROR] kernel heap allocater failure...")

void init_kheap(void);
void* kalloc(size_t);
void kfree(void *);

#endif /* __KERNEL_ALLOC_H */