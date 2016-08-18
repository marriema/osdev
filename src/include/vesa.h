#ifndef VESA_H
#define VESA_H
#include <system.h>
#include <string.h>
#include <bios32.h>

typedef struct mode_info_block {
  uint16_t attributes;
  uint8_t windowA, windowB;
  uint16_t granularity;
  uint16_t windowSize;
  uint16_t segmentA, segmentB;
  uint32_t winFuncPtr;
  uint16_t pitch;

  uint16_t resolutionX, resolutionY;
  uint8_t wChar, yChar, planes, bpp, banks;
  uint8_t memoryModel, bankSize, imagePages;
  uint8_t reserved0;

  uint8_t readMask, redPosition;
  uint8_t greenMask, greenPosition;
  uint8_t blueMask, bluePosition;
  uint8_t reservedMask, reservedPosition;
  uint8_t directColorAttributes;

  // linear frame buffer
  uint32_t physbase;
  uint32_t offScreenMemOff;
  uint16_t offScreenMemSize;
  uint8_t  reserved1 [206];
}mode_info_t;

void vesa_memset_rgb(uint8_t * dest, uint32_t rgb, uint32_t count);

void * vesa_get_lfb();

void vesa_get_mode(uint16_t mode, mode_info_t * mode_info);

void vesa_set_mode(uint32_t mode);

void vesa_init();

#endif
