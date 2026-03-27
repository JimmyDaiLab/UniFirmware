// ========== stm32_pump_hal.c ==========
#include "pump_driver.h"
#include <stdio.h>  // for printf in simulation

static void stm32_set_pwm(uint8_t id, uint16_t duty) {
    printf("PWM%d set to %d\n", id, duty);
}

static void stm32_set_dir(uint8_t id, uint8_t dir) {
    printf("DIR%d set to %d\n", id, dir);
}

static uint32_t stm32_get_tick(void) {
    return 0;
}

pump_hal_t stm32_pump_hal = {
    .set_pwm = stm32_set_pwm,
    .set_dir = stm32_set_dir,
    .get_tick = stm32_get_tick
};