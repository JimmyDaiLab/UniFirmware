// ========== pump_device.h ==========
#ifndef PUMP_DEVICE_H
#define PUMP_DEVICE_H

#include "hcf_device.h"
#include "pump_hal.h"

#define PUMP_CMD_START_DISPENSE  0x01
#define PUMP_CMD_SET_PARAM       0x02

device_socket_t *pump_device_create(uint8_t index, pump_hal_t *hal);

#endif // PUMP_DEVICE_H
