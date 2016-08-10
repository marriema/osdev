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
#include <vfs.h>

#define SCHED_TOLERANCE 5

// All possible process state, copied from sched.h
#define TASK_RUNNING            0
#define TASK_INTERRUPTIBLE      1
#define TASK_UNINTERRUPTIBLE    2
#define TASK_ZOMBIE             4
#define TASK_STOPPED            8
#define TASK_SWAPPING           16
#define TASK_EXCLUSIVE          32

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
    void * stack;
    uint32_t state;
    uint32_t time_slice;
    page_directory_t * page_dir;
}pcb_t;

extern list_t * process_list;
extern pcb_t * current_process;
extern register_t * saved_context;



pid_t allocate_pid();
void process_init();
void regs_switch(context_t * regs2);
void schedule();
void create_process(char * filename);
#endif
