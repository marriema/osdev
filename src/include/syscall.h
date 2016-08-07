#ifndef SYSCALL_H
#define SYSCALL_H
#include <system.h>
#include <isr.h>
#include <vfs.h>
#include <process.h>

#define NUM_SYSCALLS 2

void syscall_dispatcher(register_t * regs);

void syscall_init();

#endif
