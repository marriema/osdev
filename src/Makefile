# Setup compiler name and flags
CC=i686-elf-gcc
CFLAGS=-Wall -Wno-int-conversion -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdlib -ffreestanding -I./include -g -ggdb -c
NASM=nasm
NASMFLAGS=-f elf32
LDFLAGS=-T link.ld -ffreestanding -O2 -nostdlib -g -ggdb

# Setup C/ASM SOURCES
SOURCES=$(ROOT_DIR)/kmain.c $(COMMON_DIR)/system.c $(COMMON_DIR)/string.c $(DT_DIR)/gdt.c\
	$(DT_DIR)/idt.c $(DRIVERS_DIR)/vga.c $(DEBUG_UTILS_DIR)/printf.c $(DRIVERS_DIR)/pic.c\
	$(COMMON_DIR)/port_io.c $(INTERRUPT_DIR)/exception.c $(INTERRUPT_DIR)/interrupt.c $(DRIVERS_DIR)/timer.c $(MEM_DIR)/pmm.c $(MEM_DIR)/paging.c\
	$(MEM_DIR)/kheap.c $(DRIVERS_DIR)/pci.c $(DRIVERS_DIR)/ata.c $(DS_DIR)/list.c $(DS_DIR)/generic_tree.c\
	$(FILESYSTEM_DIR)/vfs.c $(FILESYSTEM_DIR)/ext2.c $(SCHEDULER_DIR)/usermode.c $(DT_DIR)/tss.c $(SYSCALL_DIR)/syscall.c $(SCHEDULER_DIR)/process.c

ASM_SOURCES=$(ROOT_DIR)/entry.asm $(DT_DIR)/idt_helper.asm $(DT_DIR)/gdt_helper.asm $(INTERRUPT_DIR)/exception_helper.asm \
	$(INTERRUPT_DIR)/interrupt_helper.asm $(SCHEDULER_DIR)/usermode_helper.asm $(DT_DIR)/tss_helper.asm $(SCHEDULER_DIR)/context_switch.asm

# Setup object files
OBJECTS=$(SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)
EXECUTABLE=../kernel

# Setup all source directories
KERNEL_DIR=kernel
OBJS_DIR=objs
ALL_OBJECTS=$(ALL_COBJECTS) $(ALL_ASMOBJECTS)
ALL_COBJECTS=$(ROOT_COBJS) $(DT_COBJS) $(COMMON_COBJS) $(INTERRUPT_COBJS) $(MEM_COBJS) $(FILESYSTEM_COBJS) $(DRIVERS_COBJS) $(SCHEDULER_COBJS) $(DS_COBJS) $(SYSCALL_COBJS) $(LOADER_COBJS) $(DEBUG_UTILS_COBJS)
ALL_ASMOBJECTS=$(COMMON_ASMOBJS) $(DS_ASMOBJS) $(DEBUG_UTILS_ASMOBJS) $(DT_ASMOBJS) $(DRIVERS_ASMOBJS) $(FILESYSTEM_ASMOBJS) $(INTERRUPT_ASMOBJS) $(MEM_ASMOBJS) $(SCHEDULER_ASMOBJS) $(SYSCALL_ASMOBJS) $(LOADER_ASMOBJS) $(ROOT_ASMOBJS)


ROOT_DIR=$(KERNEL_DIR)
ROOT_CSRC=$(wildcard $(ROOT_DIR)/*.c)
ROOT_ASMSRC=$(wildcard $(ROOT_DIR)/*.asm)
ROOT_COBJS=$(ROOT_CSRC:.c=.o)
ROOT_ASMOBJS=$(ROOT_ASMSRC:.asm=.o)

COMMON_DIR=$(KERNEL_DIR)/common
COMMON_CSRC=$(wildcard $(COMMON_DIR)/*.c)
COMMON_ASMSRC=$(wildcard $(COMMON_DIR)/*.asm)
COMMON_COBJS=$(COMMON_CSRC:.c=.o)
COMMON_ASMOBJS=$(COMMON_ASMSRC:.asm=.o)

DS_DIR=$(KERNEL_DIR)/data_structure
DS_CSRC=$(wildcard $(DS_DIR)/*.c)
DS_ASMSRC=$(wildcard $(DS_DIR)/*.asm)
DS_COBJS=$(DS_CSRC:.c=.o)
DS_ASMOBJS=$(DS_ASMSRC:.asm=.o)

DEBUG_UTILS_DIR=$(KERNEL_DIR)/debug_utils
DEBUG_UTILS_CSRC=$(wildcard $(DEBUG_UTILS_DIR)/*.c)
DEBUG_UTILS_ASMSRC=$(wildcard $(DEBUG_UTILS_DIR)/*.asm)
DEBUG_UTILS_COBJS=$(DEBUG_UTILS_CSRC:.c=.o)
DEBUG_UTILS_ASMOBJS=$(DEBUG_UTILS_ASMSRC:.asm=.o)


DT_DIR=$(KERNEL_DIR)/descriptor_tables
DT_CSRC=$(wildcard $(DT_DIR)/*.c)
DT_ASMSRC=$(wildcard $(DT_DIR)/*.asm)
DT_COBJS=$(DT_CSRC:.c=.o)
DT_ASMOBJS=$(DT_ASMSRC:.asm=.o)
	
DRIVERS_DIR=$(KERNEL_DIR)/drivers
DRIVERS_CSRC=$(wildcard $(DRIVERS_DIR)/*.c)
DRIVERS_ASMSRC=$(wildcard $(DRIVERS_DIR)/*.asm)
DRIVERS_COBJS=$(DRIVERS_CSRC:.c=.o)
DRIVERS_ASMOBJS=$(DRIVERS_ASMSRC:.asm=.o)
	
FILESYSTEM_DIR=$(KERNEL_DIR)/filesystem
FILESYSTEM_CSRC=$(wildcard $(FILESYSTEM_DIR)/*.c)
FILESYSTEM_ASMSRC=$(wildcard $(FILESYSTEM_DIR)/*.asm)
FILESYSTEM_COBJS=$(FILESYSTEM_CSRC:.c=.o)
FILESYSTEM_ASMOBJS=$(FILESYSTEM_ASMSRC:.asm=.o)

INTERRUPT_DIR=$(KERNEL_DIR)/interrupt
INTERRUPT_CSRC=$(wildcard $(INTERRUPT_DIR)/*.c)
INTERRUPT_ASMSRC=$(wildcard $(INTERRUPT_DIR)/*.asm)
INTERRUPT_COBJS=$(INTERRUPT_CSRC:.c=.o)
INTERRUPT_ASMOBJS=$(INTERRUPT_ASMSRC:.asm=.o)

MEM_DIR=$(KERNEL_DIR)/mem
MEM_CSRC=$(wildcard $(MEM_DIR)/*.c)
MEM_ASMSRC=$(wildcard $(MEM_DIR)/*.asm)
MEM_COBJS=$(MEM_CSRC:.c=.o)
MEM_ASMOBJS=$(MEM_ASMSRC:.asm=.o)

SCHEDULER_DIR=$(KERNEL_DIR)/scheduler
SCHEDULER_CSRC=$(wildcard $(SCHEDULER_DIR)/*.c)
SCHEDULER_ASMSRC=$(wildcard $(SCHEDULER_DIR)/*.asm)
SCHEDULER_COBJS=$(SCHEDULER_CSRC:.c=.o)
SCHEDULER_ASMOBJS=$(SCHEDULER_ASMSRC:.asm=.o)

SYSCALL_DIR=$(KERNEL_DIR)/syscall
SYSCALL_CSRC=$(wildcard $(SYSCALL_DIR)/*.c)
SYSCALL_ASMSRC=$(wildcard $(SYSCALL_DIR)/*.asm)
SYSCALL_COBJS=$(SYSCALL_CSRC:.c=.o)
SYSCALL_ASMOBJS=$(SYSCALL_ASMSRC:.asm=.o)

LOADER_DIR=$(KERNEL_DIR)/loader
LOADER_CSRC=$(wildcard $(LOADER_DIR)/*.c)
LOADER_ASMSRC=$(wildcard $(LOADER_DIR)/*.asm)
LOADER_COBJS=$(LOADER_CSRC:.c=.o)
LOADER_ASMOBJS=$(LOADER_ASMSRC:.asm=.o)

all:$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):$(ALL_OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) -lgcc
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.asm
	@echo "anything here??"
	$(NASM) $(NASMFLAGS) $< -o $@

.PHONY: clean
clean:
	find . -type f -name '*.o' -delete
	rm -rf *.o $(EXECUTABLE) *~
	rm -rf $(TRASHBIN)/*.o
