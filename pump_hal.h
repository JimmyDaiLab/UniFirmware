// ============================
// pump_hal.h (Abstract HAL Interface)
// ============================
#ifndef PUMP_HAL_H
#define PUMP_HAL_H

#include <stdint.h>

// Abstract HAL interface (platform-independent)
typedef struct {
    void (*set_pwm)(uint8_t id, uint16_t duty);
    void (*set_dir)(uint8_t id, uint8_t dir);
    uint32_t (*get_tick)(void);
} pump_hal_t;

#endif // PUMP_HAL_H
