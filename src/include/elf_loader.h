#ifndef ELF_LOADER_H
#define ELF_LOADER_H
#include <vfs.h>
#include <printf.h>
#include <process.h>
#include <kheap.h>
#include <string.h>

void load_program(char * name);
#endif
