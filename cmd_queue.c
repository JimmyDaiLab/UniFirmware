// ========== cmd_queue.c ==========
#include "cmd_queue.h"
#include <string.h>

void cmd_queue_init(cmd_queue_t *q, void *buf, uint8_t item_size, uint8_t capacity)
{
    q->buf = (uint8_t *)buf;
    q->item_size = item_size;
    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
}

bool cmd_queue_enqueue(cmd_queue_t *q, const void *item)
{
    uint8_t next = (q->head + 1) % q->capacity;
    if (next == q->tail)
        return false;

    memcpy(&q->buf[q->head * q->item_size], item, q->item_size);
    q->head = next;
    return true;
}

bool cmd_queue_dequeue(cmd_queue_t *q, void *item)
{
    if (q->head == q->tail)
        return false;

    memcpy(item, &q->buf[q->tail * q->item_size], q->item_size);
    q->tail = (q->tail + 1) % q->capacity;
    return true;
}
