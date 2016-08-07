#include <system.h>
#include <printf.h>


void panic(const char *message, const char *file, uint32_t line)
{
    asm volatile("cli");
    printf("PANIC(%s) at %s : %u\n", message, file, line);
    for(;;);
}
