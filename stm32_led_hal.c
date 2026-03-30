// ========== stm32_led_hal.c ==========
#include "stm32_led_hal.h"
#include <stdio.h>

static void stm32_led_set_state(uint8_t id, uint8_t on)
{
    printf("LED%d state=%d\n", id, on);
}

static void stm32_led_set_brightness(uint8_t id, uint8_t level)
{
    printf("LED%d brightness=%d\n", id, level);
}

led_hal_t stm32_led_hal = {
    .set_state = stm32_led_set_state,
    .set_brightness = stm32_led_set_brightness,
};
