#include <gdt.h>
#include <paging.h>
#include <kheap.h>
#include <system.h>

idt_ptr_t real_idt_ptr;
idt_ptr_t real_gdt_ptr;

extern void bios32_helper();
extern void * bios32_helper;
extern void * bios32_helper_end;

void * new_gdt_entries;
void * new_gdt_ptr;
void * new_idt_ptr;
void * new_reg_ptr;
void * new_intnum_ptr;
/*
 * Prepare some descriptors for calling bios service from protected mode
 * We're using the easiest method : switch to real mode to call bios temporarily
 * So, prepare some 16bit global descriptors, and interrupt descriptors
 * */
void bios32_init() {
    // 16bit code segment
    gdt_set_entry(5, 0, 0xffffffff, 0x9A, 0x0f);
    // 16bit data segment
    gdt_set_entry(6, 0, 0xffffffff, 0x92, 0x0f);
    // gdt ptr
    real_gdt_ptr.base = (uint32_t)gdt_entries;
    real_gdt_ptr.limit = sizeof(gdt_entries) - 1;
    // idt ptr
    real_idt_ptr.base = 0;
    real_idt_ptr.limit = 0x3ff;
}

/*
 * Switch to real mode, do bios, then switch back to protected mode
 * need to move code to 0x7c00 for execution
 * the function that actually do all the mode switch and init work is in bios32_helper.asm (bios32_helper)
 * */
void bios32_service(uint32_t int_num, registers16_t * reg) {
    void * new_code_base = 0x7c00;
    void * new_data_base = 0x8c00;
    void * data_ptr = new_data_base;

    // Identity map the first 64 kb of physical memory
    allocate_region(kpage_dir, 0x0, 0x10000, 1, 1, 1);

    // Copy bios32_helper's code to [0x7c00, 0x8c00] (I hope this is enough space)
    uint32_t size = (uint32_t)bios32_helper - (uint32_t)bios32_helper_end;
    memcpy(new_code_base, bios32_helper, size);
    if(size > 4096){
         PANIC("That's not enough memory for bios32_helper, fix thix!\n");
    }

    // Copy relevant data to [0x8c00, ...] (gdt_entries, gdt_ptr, idt_ptr and so on)
    // And calculate the new address of these data so bios32_helper can reference them
    memcpy(data_ptr, gdt_entries, sizeof(gdt_entries));
    new_gdt_entries = data_ptr;
    data_ptr = data_ptr + sizeof(gdt_entries);
    data_ptr = ALIGN(data_ptr, 4);

    memcpy(data_ptr, &real_gdt_ptr, sizeof(real_gdt_ptr));
    new_gdt_ptr = data_ptr;
    data_ptr = data_ptr + sizeof(gdt_ptr_t);
    data_ptr = ALIGN(data_ptr, 4);

    memcpy(data_ptr, &real_idt_ptr, sizeof(real_idt_ptr));
    new_idt_ptr = data_ptr;
    data_ptr = data_ptr + sizeof(idt_ptr_t);
    data_ptr = ALIGN(data_ptr, 4);

    memcpy(data_ptr, reg, sizeof(registers16_t));
    new_reg_ptr = data_ptr;
    data_ptr = data_ptr + sizeof(registers16_t);
    data_ptr = ALIGN(data_ptr, 4);

    memcpy(data_ptr, &int_num, sizeof(uint32_t));
    new_intnum_ptr= data_ptr;
    data_ptr = data_ptr + sizeof(uint32_t);

    // Now transfer control to bios32_helper, which basically does everything
    bios32_helper();

    // Undo the identity mapping
    free_region(kpage_dir, 0x0, 0x10000, 1);
}
