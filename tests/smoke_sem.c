#include <stdio.h>
#include <string.h>

#include "../prodcon_sem/buffer_sem.h"

static void fill(char data[DATA_LENGTH], char value)
{
    for (int i = 0; i < DATA_LENGTH; i++) {
        data[i] = value;
    }
}

static int expect_data(const char data[DATA_LENGTH], char value)
{
    for (int i = 0; i < DATA_LENGTH; i++) {
        if (data[i] != value) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    char in[DATA_LENGTH];
    char out[DATA_LENGTH];

    fill(in, 'A');
    memset(out, 0, sizeof(out));

    if (delete_buffer_421() != -1) {
        fprintf(stderr, "delete before init should fail\n");
        return 1;
    }
    if (enqueue_buffer_421(in) != -1) {
        fprintf(stderr, "enqueue before init should fail\n");
        return 1;
    }
    if (init_buffer_421() != 0) {
        fprintf(stderr, "init should pass\n");
        return 1;
    }
    if (enqueue_buffer_421(in) != 0) {
        fprintf(stderr, "enqueue should pass\n");
        return 1;
    }
    if (dequeue_buffer_421(out) != 0) {
        fprintf(stderr, "dequeue should pass\n");
        return 1;
    }
    if (!expect_data(out, 'A')) {
        fprintf(stderr, "dequeued data mismatch\n");
        return 1;
    }
    if (delete_buffer_421() != 0) {
        fprintf(stderr, "delete after init should pass\n");
        return 1;
    }
    puts("smoke_sem passed");
    return 0;
}

