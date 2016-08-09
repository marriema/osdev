#include <elf_loader.h>


/*
 * Load a program from filesystem and run it
 * */
void load_program(char * filename) {
    vfs_node_t * program = file_open(filename, 0);
    if(!program) {
        printf("Fail to open %s, does it even exist?\n", filename);
        return;
    }
    uint32_t size = vfs_get_file_size(program);
    char * program_code = kcalloc(size, 1);
    vfs_read(program, 0, size, program_code);
    pcb_t * p1 = kcalloc(sizeof(pcb_t), 1);
    memcpy(p1, current_process, sizeof(pcb_t));
    p1->regs.eip = (uint32_t)program_code;
    //p1->regs.eflags = 0x200; // enable interrupt
    p1->self = list_insert_front(process_list, p1);

    // yield
    asm volatile("mov $1, %eax");
    asm volatile("int $0x80");
}
