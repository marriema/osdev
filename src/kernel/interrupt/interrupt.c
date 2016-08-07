#include <isr.h>
#include <system.h>
#include <pic.h>
#include <printf.h>


// For both exceptions and irq interrupt
isr_t interrupt_handlers[256];


/*
 * Register a function as the handler for a certian interrupt number, both exception and irq interrupt can change their handler using this function
 * */
void register_interrupt_handler(int num, isr_t handler) {
    if(num < 256)
        interrupt_handlers[num] = handler;
}

void final_irq_handler(register_t reg) {
    if(interrupt_handlers[reg.int_no] != NULL) {
        isr_t handler = interrupt_handlers[reg.int_no];
        handler(&reg);
    }
    irq_ack(reg.int_no);

}

void print_reg(register_t * reg) {
    printf("Registers dump:\n");
    printf("eax 0x%x ebx 0x%x 0x%ecx 0x%x %edx 0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    printf("edi 0x%x esi 0x%x %ebp 0x%x %esp 0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    printf("eip 0x%x cs 0x%x ss 0x%x eflags 0x%x useresp 0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
}
