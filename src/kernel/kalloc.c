/*
** simple kernel heap allocater implementation **.

- first-fit is used for choosing a free block to allocate.
- implicit list is used to keep track of the used/free blocks.
- splitting is used whenever the allocated space is smaller than the first-fit hole found.
- bidirectional coalescing is used whenever an allocated block is being freed.
- to make bidirectional coalescing possible we use boundary tags at each block end.

[ BLOCK STRUCTURE ]

31                  2 1                 0
+++++++++++++++++++++++++++++++++++++++++
BLOCK SIZE           | BLOCK STATUS     | (BLOCK HEADER)
-----------------------------------------
                                        |
BLOCK PAYLOAD + PADDING (FOR ALIGNMENT) |
                                        |
-----------------------------------------
BLOCK SIZE           | BLOCK STATUS     | (BLOCK BOUNDARY TAG)
+++++++++++++++++++++++++++++++++++++++++

block status values
-------------------
- allocated block 0x01
- free block      0x0

block payload
-------------
- holds actual data + some padding bytes (if any)
*/

#include <stddef.h>
#include <string.h>
#include <printk.h>
#include <types.h>
#include <sys/kernel.h>
#include <sys/mm.h>
#include <sys/kalloc.h>


extern virtual_memory_info_t virtual_memory_info;


void 
init_kheap()
{
	u32_t kheap_start, kheap_end, kheap_vaddr;

	printk("initializing kernel heap... ");

	kheap_start = virtual_memory_info.kernel_end_address;
	virtual_memory_info.kheap.start_address = kheap_start;
    virtual_memory_info.kheap.end_address = kheap_start + KERNEL_HEAP_SIZE;
	virtual_memory_info.kheap.size = KERNEL_HEAP_SIZE;

	kheap_end = virtual_memory_info.kheap.end_address;
	for (kheap_vaddr = kheap_start;
		    kheap_vaddr < kheap_end; kheap_vaddr += MM_PAGE_SIZE) {
		if (map_virtual_address(
			    MM_PTABLE_ENTRY_FLAG_PRESENT | MM_PTABLE_ENTRY_FLAG_WRITABLE,
			    kheap_vaddr) != MM_VIRTUAL_ADDRESS_MAPPING_SUCCESS)
			kernel_heap_allocater_failure_handler();
	}

    init_heap_blocks_implicit_list(kheap_start,
        virtual_memory_info.kheap.size);

	printk("[ done ]\n");
}

void*
kalloc(size_t request_size)
{
    size_t _request_size, allocated_size;
    u8_t *heap_block_ptr;

    _request_size = kheap_round_size(request_size,
        KERNEL_HEAP_BLOCK_SIZE_ALIGNMENT);
    allocated_size = _request_size + 2 * sizeof(kheap_block_header_t);

    if (allocated_size > KERNEL_HEAP_MAX_REQUEST_SIZE)
        kernel_heap_allocater_failure_handler();

    // search for first fit hole
    heap_block_ptr = (u8_t *)virtual_memory_info.kheap.start_address;
    while (((u32_t)heap_block_ptr < virtual_memory_info.kheap.end_address) &&
            (is_heap_block_used(heap_block_ptr) ||
            (get_heap_block_size(heap_block_ptr) < allocated_size)))
        heap_block_ptr += get_heap_block_size(heap_block_ptr);

    // no good fit was found
    if ((u32_t)heap_block_ptr >= virtual_memory_info.kheap.end_address)
        kernel_heap_allocater_failure_handler();

    setup_heap_block(heap_block_ptr, allocated_size, KERNEL_HEAP_BLOCK_USED);

    return heap_block_usable_mem_ptr(heap_block_ptr);
}

void
kfree(void *ptr)
{
    kheap_block_header_t *heap_current_block_ptr;

    heap_current_block_ptr =
        (kheap_block_header_t *)((char *)ptr - sizeof(kheap_block_header_t));
    heap_free_block(heap_current_block_ptr);
    heap_try_coalesce_next_block(heap_current_block_ptr);
    heap_try_coalesce_prev_block(heap_current_block_ptr);
}
