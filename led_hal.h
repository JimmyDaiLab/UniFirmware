// ========== led_hal.h (Abstract HAL Interface) ==========
#ifndef LED_HAL_H
#define LED_HAL_H

#include <stdint.h>

typedef struct {
    void (*set_state)(uint8_t id, uint8_t on);
    void (*set_brightness)(uint8_t id, uint8_t level);
} led_hal_t;

#endif // LED_HAL_H
