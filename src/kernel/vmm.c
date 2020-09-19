/* virtual memory manager */

#include <types.h>
#include <stddef.h>
#include <printk.h>
#include <string.h>
#include <sys/kernel.h>
#include <sys/mm.h>


extern physical_memory_info_t physical_memory_info;

/* kernel's default page directory, used during context switches, etc... */ 
struct {
    pde_t *ptr;
} kpdir;


void
init_paging ()
{
    u32_t kstart_virtaddr, kend_virtaddr, pdir_indx, ptable_indx,
          physaddr, virtaddr;
    /* pointer to a page table that holds the kernel's virtual addreses mapping */
    pte_t *ptable;

    kpdir.ptr = (pde_t *) allocate_physical_memory_frame();
    if (__equal__(kpdir.ptr, PHYSICAL_FRAME_ALLOCATE_ERROR))
        MM_PAGING_INIT_FATAL_ERROR(
            "[ FATAL ERROR ] failed to initialize paging... now halting");

    kmemset(kpdir.ptr, 0x0, MM_FRAME_SIZE);

    /* 
     * recursivley map the page directory on itself so we can
     * dynamically update the page tables without the need for identity mapping
    */
    kpdir.ptr[0x3ff] = (u32_t) kpdir.ptr | MM_PDIR_ENTRY_FLAG_PRESENT |
        MM_PDIR_ENTRY_FLAG_WRITABLE;

    kstart_virtaddr = MM_KERNEL_VIRTUAL_LOAD_ADDRESS;
    kend_virtaddr = MM_KERNEL_VIRTUAL_LOAD_ADDRESS +
        physical_memory_info.kernel_end_address;
    kstart_virtaddr = MM_VIRTADDR_ROUND_PAGE_UP(kstart_virtaddr);
    kend_virtaddr = MM_VIRTADDR_ROUND_PAGE_UP(kend_virtaddr);

    /* map the kernel's virtual address spaces */
    for (virtaddr = kstart_virtaddr, physaddr = 0x0; virtaddr <= kend_virtaddr;
            virtaddr += MM_PAGE_SIZE, physaddr += MM_FRAME_SIZE) {
        pdir_indx = MM_GET_PDIR_INDEX(virtaddr);
        ptable_indx = MM_GET_PTABLE_INDEX(virtaddr);

        if (!(kpdir.ptr[pdir_indx] & MM_PDIR_ENTRY_FLAG_PRESENT)) {
            ptable = (pte_t *) allocate_physical_memory_frame();
            kmemset(ptable, 0x0, MM_FRAME_SIZE);
            kpdir.ptr[pdir_indx] = (u32_t) ptable |
                MM_PDIR_ENTRY_FLAG_PRESENT | MM_PDIR_ENTRY_FLAG_WRITABLE;
        }

        ptable[ptable_indx] = physaddr | MM_PTABLE_ENTRY_FLAG_PRESENT |
            MM_PTABLE_ENTRY_FLAG_WRITABLE | MM_PTABLE_ENTRY_FLAG_WRITE_THROUGH;
    }

    pdir_switch(kpdir.ptr);
}

u32_t
map_virtual_address (u32_t flags, u32_t virtual_address)
{
    u32_t pdir_indx, ptable_indx, physaddr, ptable_physaddr;
    pte_t *ptable;
    pde_t *pdir;

    pdir_indx = MM_GET_PDIR_INDEX(virtual_address);
    ptable_indx = MM_GET_PTABLE_INDEX(virtual_address);

    pdir = (pde_t *) 0xfffff000;
    ptable = (pte_t *) (0xffc00000 + (pdir_indx * MM_PAGE_SIZE));

    /* page table do exist ? if not create it */
    if (!(pdir[pdir_indx] & MM_PDIR_ENTRY_FLAG_PRESENT)) {
        ptable_physaddr = allocate_physical_memory_frame();
        if (__equal__(ptable_physaddr, PHYSICAL_FRAME_ALLOCATE_ERROR))
            return MM_VIRTUAL_ADDRESS_MAPPING_ERROR;
        pdir[pdir_indx] = ptable_physaddr | MM_PDIR_ENTRY_FLAG_PRESENT |
            MM_PDIR_ENTRY_FLAG_WRITABLE;
    }

    /* fail if a mapping do exist for this address */
    if (ptable[ptable_indx] & MM_PTABLE_ENTRY_FLAG_PRESENT)
        return VIRTUAL_MEM_PAGE_ALLOCATION_ERROR;
    
    physaddr = allocate_physical_memory_frame();
    if (__equal__(physaddr, PHYSICAL_FRAME_ALLOCATE_ERROR))
        return MM_VIRTUAL_ADDRESS_MAPPING_ERROR;
    ptable[ptable_indx] = physaddr | flags;

    return MM_VIRTUAL_ADDRESS_MAPPING_SUCCESS;
}

u32_t
unmap_virtual_address (u32_t virtual_address)
{
    u32_t pdir_indx, ptable_indx;
    pte_t *ptable;
    pde_t *pdir;

    pdir_indx = MM_GET_PDIR_INDEX(virtual_address);
    ptable_indx = MM_GET_PTABLE_INDEX(virtual_address);

    pdir = (pde_t *) 0xfffff000;
    ptable = (pte_t *) (0xffc00000 + (pdir_indx * MM_PAGE_SIZE));

    if (pdir[pdir_indx] & MM_PDIR_ENTRY_FLAG_PRESENT) {
        if (ptable[ptable_indx] & MM_PTABLE_ENTRY_FLAG_PRESENT) {
            deallocate_physical_memory_frame(ptable[ptable_indx] & 0xfffff000);
            ptable[ptable_indx] = INVALID_PTABLE_ENTRY;
        }

        /* 
         * reclaim the page table's physical memory if there is no
         * other virtual addresses mappings in it
        */
        for (u32_t i = 0; i < (MM_PAGE_SIZE / sizeof(pte_t)); ++i) {
            if (ptable[i] & MM_PTABLE_ENTRY_FLAG_PRESENT)
                goto end_unmap_virtual_address;
        }
        deallocate_physical_memory_frame(pdir[pdir_indx] & 0xfffff000);
        pdir[pdir_indx] = INVALID_PDIR_ENTRY;
    }

end_unmap_virtual_address:
    return MM_VIRTUAL_ADDRESS_UNMAPPING_SUCCESS;
}
