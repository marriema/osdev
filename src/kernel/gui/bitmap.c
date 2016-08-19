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

    printf("bitmap is %u x %u\n", ret->width, ret->height);
    printf("file is here: %p\n", buf);
    printf("image is here %p\n", ret->image_bytes);
    return ret;
}


void weird_memcpy(void * dest, void * src, uint32_t count) {
    char * p1 = dest, * p2 = src;;
    while(count-- > 0) {
        memcpy(p1, p2-3, 3);
        p1 = p1 + 3;
        p2 = p2 - 3;
    }
}
void bitmap_display(bitmap_t * bmp) {
    if(!bmp) return;
    void * data = bmp->image_bytes;
    char * screen = (char*)0xfc000000;
    weird_memcpy(screen, data + bmp->width * bmp->height * 3, bmp->width * bmp->height);
    //memcpy(screen, data+ bmp->width * bmp->height * 3, bmp->width * bmp-> height * 3);
}
