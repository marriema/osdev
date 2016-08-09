#include <process.h>


list_t * process_list;
pcb_t * current_process;
pcb_t * last_process;

uint32_t prev_jiffies;

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
    if(((page_directory_t*)n_regs->cr3) != NULL)
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
    // Only a fake process in the process list, not meaningful to schedule
    if(current_process->initial && list_size(process_list) == 1) {
        printf("Useless sched\n");
        return;
    }
    if(current_process->initial) {
        list_remove_node(process_list, current_process->self);
        prev_jiffies = jiffies;
    }
    else{
        // Take a look at current process's time slice, compare it with jiffies - prev_jiffies, if the abs(time_slice, jiffies - prev_jiffies) <= some number, switch to next process
        int cmp = jiffies - prev_jiffies;
        int slice = current_process->time_slice;
        // otherwise, we'll keep current process running
        if(abs(cmp - slice) > SCHED_TOLERANCE)
            return;
    }

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
    register_wakeup_call(process_scheduler, 30.0/hz);
}
