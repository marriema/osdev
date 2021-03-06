# Setup compiler name and flags
CC=i686-elf-gcc
CFLAGS=-Wall -Wno-int-conversion -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdlib -ffreestanding -I./include -std=gnu99 -g -ggdb -c
NASM=nasm
NASMFLAGS=-f elf32 -O0
LDFLAGS=-T link.ld -ffreestanding -O2 -nostdlib -g -ggdb

# Setup C/ASM SOURCES(Don't change the order of the following source file names! bad things can happen!)
SOURCES=$(ROOT_DIR)/kmain.c $(COMMON_DIR)/system.c $(COMMON_DIR)/string.c $(DT_DIR)/gdt.c\
	$(DT_DIR)/idt.c $(DRIVERS_DIR)/vga.c $(DEBUG_UTILS_DIR)/printf.c $(DRIVERS_DIR)/pic.c\
	$(COMMON_DIR)/port_io.c $(INTERRUPT_DIR)/exception.c $(INTERRUPT_DIR)/interrupt.c $(DRIVERS_DIR)/timer.c $(MEM_DIR)/pmm.c $(MEM_DIR)/paging.c\
	$(MEM_DIR)/kheap.c $(DRIVERS_DIR)/pci.c $(DRIVERS_DIR)/ata.c $(DS_DIR)/list.c $(DS_DIR)/generic_tree.c\
	$(FILESYSTEM_DIR)/vfs.c $(FILESYSTEM_DIR)/ext2.c $(SCHEDULER_DIR)/usermode.c $(DT_DIR)/tss.c $(SYSCALL_DIR)/syscall.c $(SCHEDULER_DIR)/process.c\
	$(LOADER_DIR)/elf_loader.c $(SYSCALL_DIR)/_exit.c $(COMMON_DIR)/bios32.c $(DRIVERS_DIR)/vesa.c $(GUI_DIR)/bitmap.c


ASM_SOURCES=$(ROOT_DIR)/entry.asm $(DT_DIR)/idt_helper.asm $(DT_DIR)/gdt_helper.asm $(INTERRUPT_DIR)/exception_helper.asm \
	$(INTERRUPT_DIR)/interrupt_helper.asm $(SCHEDULER_DIR)/usermode_helper.asm $(DT_DIR)/tss_helper.asm $(SCHEDULER_DIR)/context_switch.asm $(COMMON_DIR)/bios32_helper.asm

# Setup object files
OBJECTS=$(SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)
EXECUTABLE=../os_kernel

# Setup all source directories
KERNEL_DIR=kernel
OBJS_DIR=objs

ROOT_DIR=$(KERNEL_DIR)
COMMON_DIR=$(KERNEL_DIR)/common
DS_DIR=$(KERNEL_DIR)/data_structure
DEBUG_UTILS_DIR=$(KERNEL_DIR)/debug_utils
DT_DIR=$(KERNEL_DIR)/descriptor_tables
DRIVERS_DIR=$(KERNEL_DIR)/drivers
FILESYSTEM_DIR=$(KERNEL_DIR)/filesystem
INTERRUPT_DIR=$(KERNEL_DIR)/interrupt
MEM_DIR=$(KERNEL_DIR)/mem
SCHEDULER_DIR=$(KERNEL_DIR)/scheduler
SYSCALL_DIR=$(KERNEL_DIR)/syscall
LOADER_DIR=$(KERNEL_DIR)/loader
GUI_DIR=$(KERNEL_DIR)/gui

all:$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):$(OBJECTS)
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
