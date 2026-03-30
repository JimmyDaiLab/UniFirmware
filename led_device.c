// ========== led_device.c ==========
#include "led_driver.h"
#include "led_device.h"

#define LED_MAX_INSTANCES 4

typedef struct {
    led_driver_t driver;
} led_private_t;

static bool device_init(device_socket_t *self, void *hal)
{
    led_private_t *priv = (led_private_t *)self->private_data;

    led_driver_init(&priv->driver, self->id, (led_hal_t *)self->hal_ctx);
    self->state = DEVICE_IDLE;
    return true;
}

static void device_process(device_socket_t *self)
{
    led_private_t *priv = (led_private_t *)self->private_data;

    led_driver_process(&priv->driver);

    if (priv->driver.state == LED_STATE_OFF) {
        self->state = DEVICE_IDLE;
    } else {
        self->state = DEVICE_RUNNING;
    }
}

static bool device_handle_cmd(device_socket_t *self, device_cmd_t *cmd)
{
    led_private_t *priv = (led_private_t *)self->private_data;

    switch (cmd->type) {
    case LED_CMD_TURN_ON:
        led_driver_enqueue_cmd(&priv->driver, LED_CMD_ON, cmd->param1);
        return true;

    case LED_CMD_TURN_OFF:
        led_driver_enqueue_cmd(&priv->driver, LED_CMD_OFF, 0);
        return true;

    case LED_CMD_BRIGHTNESS:
        led_driver_enqueue_cmd(&priv->driver, LED_CMD_SET_BRIGHTNESS, cmd->param1);
        return true;

    case DEVICE_CMD_STOP:
        led_driver_enqueue_cmd(&priv->driver, LED_CMD_OFF, 0);
        return true;

    default:
        return false;
    }
}

static void device_get_status(device_socket_t *self, uint8_t *status_buf, uint32_t *status_len)
{
    led_private_t *priv = (led_private_t *)self->private_data;

    if (status_buf == NULL || status_len == NULL) return;
    status_buf[0] = (uint8_t)self->state;
    status_buf[1] = priv->driver.brightness;
    *status_len = 2;
}

device_socket_t *led_device_create(uint8_t index, led_hal_t *hal)
{
    static led_private_t priv_pool[LED_MAX_INSTANCES];
    static device_socket_t dev_pool[LED_MAX_INSTANCES];

    if (index >= LED_MAX_INSTANCES || hal == NULL)
        return NULL;

    dev_pool[index].id = 0;
    dev_pool[index].name = "led";
    dev_pool[index].state = DEVICE_OFFLINE;
    dev_pool[index].init = device_init;
    dev_pool[index].process = device_process;
    dev_pool[index].handle_cmd = device_handle_cmd;
    dev_pool[index].get_status = device_get_status;
    dev_pool[index].private_data = &priv_pool[index];
    dev_pool[index].hal_ctx = hal;

    return &dev_pool[index];
}
