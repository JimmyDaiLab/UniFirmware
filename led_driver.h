// ========== led_driver.h ==========
#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "led_hal.h"
#include "cmd_queue.h"

typedef enum {
    LED_CMD_NONE,
    LED_CMD_ON,
    LED_CMD_OFF,
    LED_CMD_SET_BRIGHTNESS,
} led_cmd_type_t;

typedef struct {
    led_cmd_type_t type;
    uint32_t param;
} led_cmd_t;

typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON,
} led_state_t;

#define LED_CMD_QUEUE_SIZE 4

typedef struct {
    uint8_t id;
    led_state_t state;
    uint8_t brightness;

    led_hal_t *hal;
    led_cmd_t cmd_buf[LED_CMD_QUEUE_SIZE];
    cmd_queue_t cmd_q;
} led_driver_t;

void led_driver_init(led_driver_t *d, uint8_t id, led_hal_t *hal);
void led_driver_process(led_driver_t *d);
void led_driver_enqueue_cmd(led_driver_t *d, led_cmd_type_t type, uint32_t param);

#endif // LED_DRIVER_H
