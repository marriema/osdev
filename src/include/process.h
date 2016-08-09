#ifndef PROCESS_H
#define PROCESS_H
#include <system.h>
#include <pic.h>
#include <list.h>
#include <timer.h>
#include <printf.h>
#include <paging.h>
#include <isr.h>
#include <math.h>

#define SCHED_TOLERANCE 5
typedef uint32_t pid_t;
typedef struct context {
    uint32_t eax; // 0
    uint32_t ecx; // 4
    uint32_t edx; // 8
    uint32_t ebx; // 12
    uint32_t esp; // 16
    uint32_t ebp; // 20
    uint32_t esi; // 24
    uint32_t edi; // 28
    uint32_t eflags; //32
    uint32_t cr3; // 36
    uint32_t eip; //40
}context_t;

typedef struct pcb {
    context_t regs;
    pid_t pid;
    listnode_t * self;
    uint32_t initial;
    uint32_t state;
    uint32_t time_slice;
}pcb_t;

extern list_t * process_list;
extern pcb_t * current_process;



void process_init();
void regs_switch(context_t * regs2);
void yield(register_t * data);
#endif
