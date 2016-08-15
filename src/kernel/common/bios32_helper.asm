; A helper function for calling bios from protected mode
[bits 32]

global bios32_helper
global bios32_helper_end

extern new_gdt_entries;
extern new_gdt_ptr;
extern new_idt_ptr;
extern new_reg_ptr;
extern new_intnum_ptr;

%define REBASE(x)                              (((x) - bios32_helper) + 0x7c00)
%define GDTENTRY(x)                            ((x) << 3)
%define CODE32                                 GDTENTRY(1)  ; 0x08
%define DATA32                                 GDTENTRY(2)  ; 0x10
%define CODE16                                 GDTENTRY(5)  ; 0x28
%define DATA16                                 GDTENTRY(6)  ; 0x30


PG_BIT_OFF equ 0x7fffffff

section .text
bios32_helper: use32
    ; Now in 32bit protected mode
    ; Disable interrupts
    cli

    ; Turn off paging
    mov ecx, cr0
    and ecx, PG_BIT_OFF
    mov cr0, ecx
    ; Zero cr3
    xor ecx, ecx
    mov cr3, ecx
    
    ; Load new gdt and idt
    lgdt [new_gdt_ptr]
    jmp CODE16:REBASE(protected_mode_16)
protected_mode_16:use16
    ; Now in 16bit protected mode
    ; Update data segment selector
    mov ax, DATA16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Turn off protected mode
    mov eax, cr0
    and  al,  ~0x01
    mov cr0, eax
    jmp 0x0:REBASE(real_mode_16)
real_mode_16:use16
    
bios32_helper_end:
