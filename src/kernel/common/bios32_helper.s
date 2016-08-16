; A helper function for calling bios from protected mode
[bits 32]

global bios32_helper
global bios32_helper_end

global asm_gdt_ptr
global asm_gdt_entries
global asm_idt_ptr
global asm_reg_ptr
global asm_intnum_ptr


extern gdt_init
extern idt_init
extern new_gdt_entries;
extern new_gdt_ptr;
extern new_idt_ptr;
extern new_reg_ptr;
extern new_intnum_ptr;

%define REBASE(x)                              (((x) - bios32_helper) + 0x7c00)
%define GDTENTRY(x)                            ((x) << 3)
%define CODE32                                 GDTENTRY(1)  ; 0x08
%define DATA32                                 GDTENTRY(2)  ; 0x10
%define CODE16                                 GDTENTRY(6)  ; 0x30
%define DATA16                                 GDTENTRY(7)  ; 0x38


PG_BIT_OFF equ 0x7fffffff
PG_BIT_ON equ 0x80000000

section .text
bios32_helper: use32
    pusha
    ; Now in 32bit protected mode
    ; Disable interrupts
    cli
    ; Turn off paging
    mov ecx, cr0
    and ecx, PG_BIT_OFF
    mov cr0, ecx
    ; Zero cr3(save it in ebx before zeroing it)
    xor ecx, ecx
    mov ebx, cr3
    mov cr3, ecx

    ; Load new gdt
    lgdt [REBASE(asm_gdt_ptr)]
    
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

    ; Set interrupt number
    lea edi, [REBASE(int_num)]
    stosb

    ; Turn off protected mode
    mov eax, cr0
    and  al,  ~0x01
    mov cr0, eax
    jmp 0x0:REBASE(real_mode_16)
real_mode_16:use16
    ; opcode for int
    db 0xCD
int_num:
    ; put the actual interrupt number here
    db 0x00
    mov eax, cr0
    inc eax
    mov cr0, eax
    jmp CODE32:REBASE(protected_mode_32)
protected_mode_32:use32
    mov ax, DATA32
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; Turn off paging
    mov ecx, cr0
    or ecx, PG_BIT_ON
    mov cr0, ecx
    ; restore cr3
    mov cr3, ebx

    call gdt_init
    call idt_init
    sti
    popa
    ret
padding:
    db 0x0
    db 0x0
    db 0x0
asm_gdt_entries:
    ; 8 gdt entries
    resb 64
asm_gdt_ptr:
    dd 0x00000000
    dd 0x00000000
asm_idt_ptr:
    dd 0x00000000
    dd 0x00000000
asm_reg_ptr:
    resw 12
asm_intnum_ptr:
    dd 0x00000000
bios32_helper_end: