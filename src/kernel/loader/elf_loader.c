#include <elf_loader.h>

void executable_loader(char * filename) {
    vfs_node_t * program = file_open(filename, 0);
    if(!program) {
        printf("Fail to open %s, does it even exist?\n", filename);
        return;
    }
    uint32_t size = vfs_get_file_size(program);
    // Make sure the executable size is a multiple of 4096
    if(size & 0x00000fff != 0)
        size = (size & 0xfffff000) + 0x1000;
    // The start of the image needs to be page-aligned
    char * image = kmalloc_a(size);
    vfs_read(image, 0, size, program_code);


}
