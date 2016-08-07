#ifndef SYSTEM_H
#define SYSTEM_H

// Define some constants that (almost) all other modules need
#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic(#b, __FILE__, __LINE__))

// Our kernel now loads at 0xC0000000, so what low memory address such as 0xb800 you used to access, should be LOAD_MEMORY_ADDRESS + 0xb800
#define LOAD_MEMORY_ADDRESS 0xC0000000

#define NULL 0
#define TRUE 1
#define FALSE 0

#define K 1024
#define M (1024*K)
#define G (1024*M)

#define KDEBUG 1
typedef unsigned int uint32_t ;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

// Defined in port_io.c
void outportb(uint16_t port, uint8_t val);
uint8_t inportb(uint16_t port);

uint16_t inports(uint16_t _port);
void outports(uint16_t _port, uint16_t _data);

uint32_t inportl(uint16_t _port);
void outportl(uint16_t _port, uint32_t _data);


// Defined in system.c
void panic(const char *message, const char *file, uint32_t line);

#endif
