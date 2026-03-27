// ========== pump_device.c ==========
#include <stdlib.h>
#include <stdio.h>
#include "hcf_device.h"
#include "pump_driver.h"
#include "pump_device.h"
#include "stm32_pump_hal.h"

typedef struct {
    pump_driver_t driver;
} pump_private_t;

static bool device_init(void *self, void *hal)
{
    device_socket_t *device = (device_socket_t *)self;
    pump_private_t *priv = (pump_private_t *)device->private_data;

    pump_driver_init(&priv->driver, device->id, (pump_hal_t *)hal);
    device->state = DEVICE_IDLE;
    return true;
}

static void device_process(void *self)
{
    device_socket_t *device = (device_socket_t *)self;
    pump_private_t *priv = (pump_private_t *)device->private_data;

    pump_driver_process(&priv->driver);

    if (priv->driver.state == PUMP_IDLE) {
        device->state = DEVICE_IDLE;
    } else {
        device->state = DEVICE_RUNNING;
    }
}

static bool device_handle_cmd(void *self, device_cmd_t *cmd)
{
    device_socket_t *device = (device_socket_t *)self;
    pump_private_t *priv = (pump_private_t *)device->private_data;

    switch (cmd->type) {
    case DEVICE_CMD_START_DISPENSE:
        pump_driver_enqueue_cmd(&priv->driver, CMD_START_DISPENSE, cmd->param1);
        device->state = DEVICE_RUNNING;
        return true;

    case DEVICE_CMD_STOP:
        pump_driver_enqueue_cmd(&priv->driver, CMD_STOP, 0);
        return true;

    default:
        return false;
    }
}

static void device_get_status(void *self, uint8_t *status_buf, uint32_t *status_len)
{
    device_socket_t *device = (device_socket_t *)self;

    if (status_buf == NULL || status_len == NULL) return;
    status_buf[0] = (uint8_t)device->state;
    *status_len = 1;
}

device_socket_t *pump_device_create(void)
{
    static pump_private_t priv;
    static device_socket_t device = {
        .id = 0,
        .name = "pump",
        .state = DEVICE_OFFLINE,
        .init = device_init,
        .process = device_process,
        .handle_cmd = device_handle_cmd,
        .get_status = device_get_status,
        .private_data = &priv
    };

    device.init(&device, &stm32_pump_hal);
    return &device;
}
