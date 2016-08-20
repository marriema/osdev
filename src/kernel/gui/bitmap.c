#include <bitmap.h>
/*
 * Simmple bitmap library used in kernel
 *
 * */

bitmap_t * bitmap_create(char * filename) {
    bitmap_t * ret = kmalloc(sizeof(bitmap_t));
    vfs_node_t * file = file_open(filename, 0);
    if(!file) {
        return NULL;
    }
    uint32_t size = vfs_get_file_size(file);
    void * buf = kmalloc(size);
    vfs_read(file, 0, size, buf);


    // Parse the bitmap
    bmp_fileheader_t * h = buf;
    unsigned int offset = h->bfOffBits;
    printf("bitmap size: %u\n", h->bfSize);
    printf("bitmap offset: %u\n", offset);

    bmp_infoheader_t * info = buf + sizeof(bmp_fileheader_t);

    ret->width = info->biWidth;
    ret->height = info->biHeight;
    ret->image_bytes= (void*)((unsigned int)buf + offset);
    ret->buf = buf;
    ret->total_size= size;

    printf("bitmap is %u x %u\n", ret->width, ret->height);
    printf("file is here: %p\n", buf);
    printf("image is here %p\n", ret->image_bytes);
    return ret;
}



void bitmap_display(bitmap_t * bmp) {
    if(!bmp) return;
    char * image = bmp->image_bytes;
    char * screen = (char*)0xfc000000;
    // Do copy
    for(int i = 0; i < bmp->height; i++) {
        // Copy the ith row of image to height - 1 - i row of screen, each row is of length width * 3
        char * image_row = image + i * bmp->width * 3;
        char * screen_row = screen + (bmp->height - 1 - i) * bmp->width * 3;
        memcpy(screen_row, image_row, bmp->width * 3);
        for(int j = 0; j < bmp->width; j++) {
            if(image_row[j] != screen_row[j]) {
                printf("Different pixels in row %d column %d\n", i, j);
            }
        }
    }
    printf("Everything is fine?\n");
    for(int i = 0; i < bmp->height; i++) {
        char * image_row = image + i * bmp->width * 3;
        char * screen_row = screen + (bmp->height - 1 - i) * bmp->width * 3;
        for(int j = 0; j < bmp->width; j++) {
            if(image_row[j] != screen_row[j]) {
                printf("xx:Different pixels in row %d column %d\n", i, j);
            }
        }
    }

}
