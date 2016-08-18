#include <vesa.h>

uint32_t curr_mode;

void vesa_memset_rgb(uint8_t * dest, uint32_t rgb, uint32_t count) {
    if(count % 3 != 0)
        count = count + 3 - (count % 3);
    uint8_t r = rgb & 0x00ff0000;
    uint8_t g = rgb & 0x0000ff00;
    uint8_t b = rgb & 0x000000ff;
    for(int i = 0; i < count; i++) {
        *dest++ = r;
        *dest++ = g;
        *dest++ = b;
    }
}
void * vesa_get_lfb() {
    mode_info_t mode_info = {0};
    vesa_get_mode((uint16_t)curr_mode, &mode_info);
    return (void*)mode_info.physbase;
}

void vesa_get_mode(uint16_t mode, mode_info_t * mode_info) {
    register16_t reg_in = {0};
    register16_t reg_out = {0};
    reg_in.ax = 0x4F01;
    reg_in.cx = mode;
    reg_in.di = 0x9000;
    bios32_service(BIOS_GRAPHICS_SERVICE, &reg_in, &reg_out);
    memcpy(mode_info, (void*)0x9000, sizeof(mode_info_t));
}

void vesa_set_mode(uint32_t mode) {
    register16_t reg_in = {0};
    register16_t reg_out = {0};
    reg_in.ax = 0x4F02;
    reg_in.bx = mode;
    bios32_service(BIOS_GRAPHICS_SERVICE, &reg_in, &reg_out);
    curr_mode = mode;
}

void vesa_init() {
    bios32_init();
}
