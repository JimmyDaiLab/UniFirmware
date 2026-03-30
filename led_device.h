// ========== led_device.h ==========
#ifndef LED_DEVICE_H
#define LED_DEVICE_H

#include "hcf_device.h"
#include "led_hal.h"

#define LED_CMD_TURN_ON          0x01
#define LED_CMD_TURN_OFF         0x02
#define LED_CMD_BRIGHTNESS       0x03

device_socket_t *led_device_create(uint8_t index, led_hal_t *hal);

#endif // LED_DEVICE_H
