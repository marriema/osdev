#include <multiboot.h>
#include <system.h>
#include <printf.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <vga.h>
#include <timer.h>
#include <asciiart.h>
#include <pmm.h>
#include <paging.h>
#include <kheap.h>
#include <pci.h>
#include <ata.h>
#include <vfs.h>
#include <string.h>
#include <ext2.h>
#include <process.h>
#include <usermode.h>
#include <syscall.h>


extern uint8_t * bitmap;
extern ata_dev_t primary_master;

#define MSIZE 48 * M

void test_switch() {
    printf("In test_switch\n");
    // Yield
    asm volatile("xchg %bx,%bx");
    asm volatile("xchg %bx,%bx");
    asm volatile("xchg %bx,%bx");
    asm volatile("mov $1, %eax");
    asm volatile("int $0x80");
    for(;;);
}

int kmain(multiboot_info_t * mb_info) {

    video_init();
    printf("%s\n", simpleos_logo);

    // Initialize everything (green)
    set_curr_color(LIGHT_GREEN);
    printf("Initializing video(text mode 80 * 25)...\n");

    printf("Initializing gdt, idt and tss...\n");
    gdt_init();
    idt_init();
    tss_init(5, 0x10, 0);

    printf("Initializing physical memory manager...\n");
    pmm_init(1096 * M);

    printf("Initializing paging...\n");
    paging_init();

    printf("Initializing kernel heap...\n");
    kheap_init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX_ADDRESS);

    printf("Initializing timer...\n");
    timer_init();

    printf("Initializing pci...\n");
    pci_init();

    printf("Initializing vfs, ext2 and ata/dma...\n");
    vfs_init();
    ata_init();
    ext2_init("/dev/hda", "/");

    printf("Initializing system calls and enter usermode...\n");
    syscall_init();
    enter_usermode();
    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    tss_set_stack(0x10, esp);

    set_curr_color(WHITE);

    // Test goes here
    printf("Testing systemcall from userland: create a file called DonaldTrumpFail under root dir\n");
    char *fname = "/DonaldTrumpFail";
    uint32_t permission = 0;
    asm volatile("xor %eax,%eax");
    asm volatile("mov %0, %%ebx" :: "r"(fname));
    asm volatile("mov %0, %%ecx" :: "r"(permission));
    asm volatile("xchg %bx,%bx");
    asm volatile("int $0x80");

    printf("Did syscall created a file named %s under root dir ?\n", fname);
    vfs_db_listdir("/");

    process_init();
    // Create a process for testing
    pcb_t * p1 = kcalloc(sizeof(pcb_t), 1);
    p1->self = list_insert_front(process_list, p1);
    current_process = p1;

    // Do some no-ops and yield
    // The following in-line assembly calls yield(), which enters the kernel, find next process and switch to it
    // but we only have one process p1, so it will still switch back to p1
    asm volatile("xchg %bx,%bx");
    asm volatile("xchg %bx,%bx");
    asm volatile("xchg %bx,%bx");
    asm volatile("mov $1, %eax");
    asm volatile("int $0x80");

    // Let's try manually create another process and yield again
    pcb_t * p2 = kcalloc(sizeof(pcb_t), 1);
    memcpy(p2, p1, sizeof(pcb_t));
    p2->regs.eip = (uint32_t)test_switch;
    p2->self = list_insert_front(process_list, p2);

    asm volatile("mov $1, %eax");
    asm volatile("int $0x80");

    printf("In main\n");
    set_curr_color(LIGHT_RED);

    printf("\nDone!\n");
    return 0;
}
