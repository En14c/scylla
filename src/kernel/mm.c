#include <types.h>
#include <printk.h>
#include <sys/kernel.h>
#include <sys/mm.h>
#include <multiboot.h>

/* ### physical memory manager ### */

typedef struct physical_memory_info
{
    /* kernel's physical memory start address (load address) */
    u32_t kernel_load_address;
    /* kernel's physical memory end address (adding the bitmap's size) */
    u32_t kernel_end_address;
    /* overall size of the available RAM areas */
    u32_t size;
    /* the number of physical RAM frames */
    u32_t frames_num;
    /* physical frame's size in bytes */
    u32_t frame_size;
    /* bitmap for tracking the status of the physical frames (USED || FREE) */
#define PHYSICAL_MEM_FRAMES_BITMAP_SLOT_SIZE 0x8
    u32_t frames_bitmap_size;
    char *frames_bitmap;
} __attribute__((packed)) physical_memory_info_t;

physical_memory_info_t physical_memory_info;

/*
 * get current status (USED || FREE) for a given physical frame
*/
static u32_t
get_physical_frame_status (u32_t physf_num)
{
    char *bitmap;

    bitmap = physical_memory_info.frames_bitmap;
    u32_t frame_status = (bitmap[physf_num / 8] >> (physf_num % 8)) & 0x1;
    return frame_status == 0 ? PHYSICAL_FRAME_FREE : PHYSICAL_FRAME_USED;
}

/*
 * search the bitmap for the first FREE physical frame
*/
static u32_t
get_free_physical_frame ()
{
    char *bitmap;

    bitmap = physical_memory_info.frames_bitmap;
    for (int i = 0; i < (PHYSICAL_FRAMES_BITMAP_SIZE); i++) {
        for (int j = 0; j < 8; j++) {
            if (!((bitmap[i] >> j) & 0x1))
                return i * 8 + j;
        }
    }
    // whoops, no physical frames are free
    return PHYSICAL_FRAME_USED;
}

/*
 * mark frame as used
*/
static s32_t
mark_physical_frame_used (u32_t frame_num)
{
    char *bitmap;

    bitmap = physical_memory_info.frames_bitmap;
    if (get_physical_frame_status(frame_num) == PHYSICAL_FRAME_USED)
        return PHYSICAL_FRAME_MARK_USED_ERROR;
    bitmap[frame_num / 8] |= 1 << (frame_num % 8);
    return PHYSICAL_FRAME_MARK_USED_SUCCESS;
}

/*
 * mark frame as free
*/
static s32_t
mark_physical_frame_free (u32_t frame_num)
{
    char *bitmap;

    bitmap = physical_memory_info.frames_bitmap;
    bitmap[frame_num / 8] &= ~(1 << (frame_num % 8));
    return PHYSICAL_FRAME_MARK_FREE_SUCCESS;
}

/*
 * allocate a physical memory frame (mark as USED)
 * and return it's start physical address
*/
u32_t
allocate_physical_memory_frame ()
{
    u32_t frame_num;
    u32_t physical_mem_address;

    frame_num = get_free_physical_frame();
    // no free physical memory frames available ?
    if (frame_num == PHYSICAL_FRAME_USED)
        return PHYSICAL_FRAME_ALLOCATE_ERROR;
    physical_mem_address = physical_memory_info.frame_size * frame_num;
    return physical_mem_address;
}

/*
 * reclaim a physical memory frame (mark as FREE)
 * @param s_address: the start address of the frame
*/
u32_t
deallocate_physical_memory_frame (u32_t s_address)
{
    u32_t frame_num;

    frame_num = s_address / physical_memory_info.frame_size;
    if (mark_physical_frame_free(frame_num) == PHYSICAL_FRAME_MARK_FREE_ERROR)
        return PHYSICAL_FRAME_DEALLOCATE_ERROR;
    return PHYSICAL_FRAME_DEALLOCATE_SUCCESS;
}

/*
 * mark a range of physical frames as USED
 *
 * @param start_phys_addr: the start physical address for an arbitrary
 *          physical memory address range (let it be x)
 * @param end_phys_addr: the end physical address for an arbitrary
 *          physical memory address range (x) 
*/
s32_t
mark_physical_memory_frames_range_used (
    u32_t start_phys_addr, u32_t end_phys_addr)
{
    u32_t start_frame, end_frame, frame_num;
    s32_t _ret;

    _ret = PHYSICAL_FRAME_MARK_USED_SUCCESS;

    start_frame = MM_GET_PHYSICAL_MEM_FRAME_NUM(start_phys_addr);
    end_frame = MM_GET_PHYSICAL_MEM_FRAME_NUM(end_phys_addr);

    for (frame_num = start_frame; frame_num <= end_frame; ++frame_num) {
        if (mark_physical_frame_used(frame_num)
                == PHYSICAL_FRAME_MARK_USED_ERROR)
            break;
    }

    /*
     * check that all physical frames in the range have been marked as
     * USED else (if at least one frame was not allocated) revert the
     * other frames' status
    */
    if (frame_num != (end_frame + 1)) {
        for (u32_t _frame_num = start_frame;
                _frame_num <= frame_num; ++_frame_num)
            mark_physical_frame_free(_frame_num);
        _ret = PHYSICAL_FRAME_MARK_USED_ERROR;
    }

    return _ret;
}

/*
 * mark a range of physical frames as FREE
 *
 * @param start_phys_addr: the start physical address for an arbitrary
 *          physical memory address range (let it be x)
 * @param end_phys_addr: the end physical address for an arbitrary
 *          physical memory address range (x) 
*/
s32_t
mark_physical_memory_frames_range_free (
        u32_t start_phys_addr, u32_t end_phys_addr)
{
    u32_t start_frame, end_frame, frame_num;
    s32_t _ret;

    _ret = PHYSICAL_FRAME_MARK_FREE_SUCCESS;

    start_frame = MM_GET_PHYSICAL_MEM_FRAME_NUM(start_phys_addr);
    end_frame = MM_GET_PHYSICAL_MEM_FRAME_NUM(end_phys_addr);

    for (frame_num = start_frame; frame_num <= end_frame; ++frame_num)
        mark_physical_frame_free(frame_num);

    return _ret;
}

/*
 * get informations about physical memory mapping, size of 
 * usable physical memory, etc.. from the multiboot information
 * structure that has been created by the bootloader
*/
extern u32_t k_start, k_end;
void
detect_physical_memory (multiboot_info_t *mboot_info)
{
    multiboot_memory_map_entry_t *mmap_entry;
    u32_t mmap_info_end;
    u32_t kernel_start_addr, kernel_end_addr;

    kernel_start_addr = (u32_t)&k_start & PHYSICAL_MEM_ADDR_FRAME_ALIGN;
    kernel_end_addr = (u32_t)&k_end;

    if (mboot_info->flags & MULTIBOOT_FLAG_MMAP) {
        mmap_entry = (multiboot_memory_map_entry_t *)mboot_info->mmap_addr;
        mmap_info_end = mboot_info->mmap_addr + mboot_info->mmap_length;
        while ((u32_t)mmap_entry < mmap_info_end) {
            printk("%s\n",
                    (mmap_entry->type == MULTIBOOT_MMAP_MEM_AVAILABLE) ?
                    "[ Usable RAM ] " : "[ Reserved ]");
            printk("Base Address: %x\n", mmap_entry->base_addr_low);
            printk("Length: %x Bytes\n", mmap_entry->length_low);

            if (mmap_entry->type == MULTIBOOT_MMAP_MEM_AVAILABLE &&
                    mmap_entry->base_addr_low >= kernel_start_addr) {
                physical_memory_info.size = mmap_entry->length_low;
                physical_memory_info.frame_size = PHYSICAL_MEM_FRAME_SIZE;
                physical_memory_info.frames_num =
                    physical_memory_info.size / PHYSICAL_MEM_FRAME_SIZE;
                physical_memory_info.frames_bitmap = (char *)kernel_end_addr;
                physical_memory_info.frames_bitmap_size =
                    (physical_memory_info.frames_num /
                     PHYSICAL_MEM_FRAMES_BITMAP_SLOT_SIZE);

                physical_memory_info.kernel_load_address = kernel_start_addr;
                physical_memory_info.kernel_end_address =
                    (u32_t)physical_memory_info.frames_bitmap +
                    physical_memory_info.frames_bitmap_size;

                /*
                 * mark memmory below 1MB and the memory occupied by
                 * the kernel memory as well as the memory occupied
                 * by the physical memory frames bit map as USED
                 * note that we do not make use of the RAM addresses
                 * below 1MB in this sample kernel
                */
                mark_physical_memory_frames_range_used(
                        0x0, physical_memory_info.kernel_end_address);
            }
            // GRUB treats the base_addr_low field as offset 0 in
            // the mmap entry structure
            mmap_entry = 
                (multiboot_memory_map_entry_t *)
                ((u32_t) mmap_entry + mmap_entry->size +
                 sizeof(mmap_entry->size));
        }
        goto detect_physical_memory_success;
    }
    // halt the machine if we can't get the physical memory map
    printk(
        "** [FATAL ERROR] failed to get physical memory mapping informations "
        "the machine will enter halt state **");
    hlt();
detect_physical_memory_success:
    return;
}
