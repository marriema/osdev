#include <process.h>


list_t * process_list;
pcb_t * current_process;
pcb_t * last_process;

/*
 * Yeah, context switch, what else to put here ?...
 * */
void context_switch(register_t * p_regs, context_t * n_regs) {
    // Save registers of current running process to memory(which is current_process->regs)
    last_process->regs.eax = p_regs->eax;
    last_process->regs.ebx = p_regs->ebx;
    last_process->regs.ecx = p_regs->ecx;
    last_process->regs.edx = p_regs->edx;
    last_process->regs.esi = p_regs->esi;
    last_process->regs.edi = p_regs->edi;
    last_process->regs.ebp = p_regs->ebp;
    last_process->regs.esp = p_regs->useresp;
    last_process->regs.eflags = p_regs->eflags;
    last_process->regs.eip = p_regs->eip;
    asm volatile("mov %%cr3, %0" : "=r"(last_process->regs.cr3));

    // Switch page directory
    switch_page_directory((page_directory_t*)n_regs->cr3, 1);
    // Load regs(memory) to the real registers
    regs_switch(n_regs);
}


/*
 * This function is registered to the timer wakeup list, so it will be wakeup every 2/18 seconds
 * */
void process_scheduler(void * data) {
    printf("Process Scheduler running\n");
    pcb_t * next;
    if(!current_process) return;
    // Now, find a process qualified to be ran next, for now, we're just doing a round robin scheduling
    listnode_t * nextnode = (current_process->self)->next;
    if(!nextnode)
        next = list_peek_front(process_list);
    else
        next = nextnode->val;
    last_process = current_process;
    current_process = next;
    context_switch(data, &next->regs);
}

void yield(register_t * data) {
    process_scheduler(data);
}

/*
 * Init process scheduler
 * */
void process_init() {
    process_list = list_create();
    // Tell the timer to call our process_scheduler every 2/18 seconds
    register_wakeup_call(process_scheduler, 2.0/18);
}
