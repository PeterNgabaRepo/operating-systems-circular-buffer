#include <stdio.h>

#include "../buffer/buffer.h"

int main(void)
{
    if (delete_buffer_421() != -1) {
        fprintf(stderr, "delete before init should fail\n");
        return 1;
    }
    if (insert_buffer_421(7) != -1) {
        fprintf(stderr, "insert before init should fail\n");
        return 1;
    }
    if (init_buffer_421() != 0) {
        fprintf(stderr, "init should pass\n");
        return 1;
    }
    for (int i = 0; i < SIZE_OF_BUFFER; i++) {
        if (insert_buffer_421(i) != 0) {
            fprintf(stderr, "insert should pass at %d\n", i);
            return 1;
        }
    }
    if (insert_buffer_421(99) != -1) {
        fprintf(stderr, "insert into full buffer should fail\n");
        return 1;
    }
    if (delete_buffer_421() != 0) {
        fprintf(stderr, "delete after init should pass\n");
        return 1;
    }
    puts("smoke_buffer passed");
    return 0;
}

