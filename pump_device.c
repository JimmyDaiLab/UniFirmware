// ========== pump_device.c ==========
#include <stdlib.h>
#include <stdio.h>
#include "pump_driver.h"
#include "pump_device.h"

#define PUMP_MAX_INSTANCES 4

typedef struct {
    pump_driver_t driver;
} pump_private_t;

static bool device_init(device_socket_t *self, void *hal)
{
    pump_private_t *priv = (pump_private_t *)self->private_data;

    pump_driver_init(&priv->driver, self->id, (pump_hal_t *)self->hal_ctx);
    self->state = DEVICE_IDLE;
    return true;
}

static void device_process(device_socket_t *self)
{
    pump_private_t *priv = (pump_private_t *)self->private_data;

    pump_driver_process(&priv->driver);

    if (priv->driver.state == PUMP_IDLE) {
        self->state = DEVICE_IDLE;
    } else {
        self->state = DEVICE_RUNNING;
    }
}

static bool device_handle_cmd(device_socket_t *self, device_cmd_t *cmd)
{
    pump_private_t *priv = (pump_private_t *)self->private_data;

    switch (cmd->type) {
    case PUMP_CMD_START_DISPENSE:
        pump_driver_enqueue_cmd(&priv->driver, CMD_START_DISPENSE, cmd->param1);
        self->state = DEVICE_RUNNING;
        return true;

    case DEVICE_CMD_STOP:
        pump_driver_enqueue_cmd(&priv->driver, CMD_STOP, 0);
        return true;

    default:
        return false;
    }
}

static void device_get_status(device_socket_t *self, uint8_t *status_buf, uint32_t *status_len)
{
    if (status_buf == NULL || status_len == NULL) return;
    status_buf[0] = (uint8_t)self->state;
    *status_len = 1;
}

device_socket_t *pump_device_create(uint8_t index, pump_hal_t *hal)
{
    static pump_private_t priv_pool[PUMP_MAX_INSTANCES];
    static device_socket_t dev_pool[PUMP_MAX_INSTANCES];

    if (index >= PUMP_MAX_INSTANCES || hal == NULL)
        return NULL;

    dev_pool[index].id = 0;
    dev_pool[index].name = "pump";
    dev_pool[index].state = DEVICE_OFFLINE;
    dev_pool[index].init = device_init;
    dev_pool[index].process = device_process;
    dev_pool[index].handle_cmd = device_handle_cmd;
    dev_pool[index].get_status = device_get_status;
    dev_pool[index].private_data = &priv_pool[index];
    dev_pool[index].hal_ctx = hal;

    return &dev_pool[index];
}
