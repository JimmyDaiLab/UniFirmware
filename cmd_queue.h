// ========== cmd_queue.h ==========
#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t *buf;
    uint8_t item_size;
    uint8_t capacity;
    uint8_t head;
    uint8_t tail;
} cmd_queue_t;

void cmd_queue_init(cmd_queue_t *q, void *buf, uint8_t item_size, uint8_t capacity);
bool cmd_queue_enqueue(cmd_queue_t *q, const void *item);
bool cmd_queue_dequeue(cmd_queue_t *q, void *item);

#endif // CMD_QUEUE_H
