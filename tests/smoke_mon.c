#include <stdio.h>
#include <string.h>

#include "../prodcon_mon/buffer_mon.h"
#include "../prodcon_mon/condFxn.h"

#define WRAP_EXTRA 7

static void fill_binary_payload(unsigned char data[DATA_LENGTH], unsigned int seed)
{
    for (int i = 0; i < DATA_LENGTH; i++) {
        data[i] = (unsigned char)(((i * 41U) + seed) & 0xffU);
    }
}

static int expect_payload(const unsigned char data[DATA_LENGTH], unsigned int seed)
{
    unsigned char expected[DATA_LENGTH];

    fill_binary_payload(expected, seed);
    return memcmp(data, expected, DATA_LENGTH) == 0;
}

int main(void)
{
    unsigned char in[DATA_LENGTH];
    unsigned char out[DATA_LENGTH];

    fill_binary_payload(in, 0);
    memset(out, 0, sizeof(out));

    if (delete_buffer_421() != -1) {
        fprintf(stderr, "delete before init should fail\n");
        return 1;
    }
    if (dequeue_buffer_421(out) != -1) {
        fprintf(stderr, "dequeue before init should fail\n");
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
    if (!isEmpty() || isFull()) {
        fprintf(stderr, "new buffer should be empty and not full\n");
        return 1;
    }

    for (int i = 0; i < SIZE_OF_BUFFER; i++) {
        fill_binary_payload(in, (unsigned int)i);
        if (enqueue_buffer_421(in) != 0) {
            fprintf(stderr, "enqueue should pass at full-fill index %d\n", i);
            return 1;
        }
    }
    if (!isFull() || isEmpty()) {
        fprintf(stderr, "full buffer should report full and not empty\n");
        return 1;
    }
    fill_binary_payload(in, 99);
    if (enqueue_buffer_421(in) != -1) {
        fprintf(stderr, "21st enqueue into full monitor buffer should fail\n");
        return 1;
    }

    for (int i = 0; i < WRAP_EXTRA; i++) {
        memset(out, 0, sizeof(out));
        if (dequeue_buffer_421(out) != 0) {
            fprintf(stderr, "dequeue should pass before wrap index %d\n", i);
            return 1;
        }
        if (!expect_payload(out, (unsigned int)i)) {
            fprintf(stderr, "pre-wrap dequeued data mismatch at index %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < WRAP_EXTRA; i++) {
        fill_binary_payload(in, (unsigned int)(SIZE_OF_BUFFER + i));
        if (enqueue_buffer_421(in) != 0) {
            fprintf(stderr, "enqueue should pass after write wrap index %d\n", i);
            return 1;
        }
    }
    if (!isFull()) {
        fprintf(stderr, "buffer should be full again after wrap enqueues\n");
        return 1;
    }

    for (int i = WRAP_EXTRA; i < SIZE_OF_BUFFER + WRAP_EXTRA; i++) {
        memset(out, 0, sizeof(out));
        if (dequeue_buffer_421(out) != 0) {
            fprintf(stderr, "dequeue should pass after wrap index %d\n", i);
            return 1;
        }
        if (!expect_payload(out, (unsigned int)i)) {
            fprintf(stderr, "post-wrap dequeued data mismatch at index %d\n", i);
            return 1;
        }
    }
    if (!isEmpty() || isFull()) {
        fprintf(stderr, "drained buffer should be empty and not full\n");
        return 1;
    }
    if (dequeue_buffer_421(out) != -1) {
        fprintf(stderr, "dequeue from empty monitor buffer should fail\n");
        return 1;
    }
    if (delete_buffer_421() != 0) {
        fprintf(stderr, "delete after init should pass\n");
        return 1;
    }
    puts("smoke_mon passed");
    return 0;
}
