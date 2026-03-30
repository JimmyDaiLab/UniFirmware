// ========== led_driver.c ==========
#include "led_driver.h"

void led_driver_enqueue_cmd(led_driver_t *d, led_cmd_type_t type, uint32_t param)
{
    led_cmd_t cmd = { .type = type, .param = param };
    cmd_queue_enqueue(&d->cmd_q, &cmd);
}

void led_driver_init(led_driver_t *d, uint8_t id, led_hal_t *hal)
{
    d->id = id;
    d->state = LED_STATE_OFF;
    d->brightness = 0;
    d->hal = hal;
    cmd_queue_init(&d->cmd_q, d->cmd_buf, sizeof(led_cmd_t), LED_CMD_QUEUE_SIZE);
}

void led_driver_process(led_driver_t *d)
{
    led_cmd_t cmd;

    while (cmd_queue_dequeue(&d->cmd_q, &cmd)) {
        switch (cmd.type) {
        case LED_CMD_ON:
            d->brightness = (cmd.param > 0) ? (uint8_t)cmd.param : 255;
            d->hal->set_brightness(d->id, d->brightness);
            d->hal->set_state(d->id, 1);
            d->state = LED_STATE_ON;
            break;

        case LED_CMD_OFF:
            d->hal->set_state(d->id, 0);
            d->hal->set_brightness(d->id, 0);
            d->brightness = 0;
            d->state = LED_STATE_OFF;
            break;

        case LED_CMD_SET_BRIGHTNESS:
            d->brightness = (uint8_t)cmd.param;
            d->hal->set_brightness(d->id, d->brightness);
            if (d->brightness > 0 && d->state == LED_STATE_OFF) {
                d->hal->set_state(d->id, 1);
                d->state = LED_STATE_ON;
            } else if (d->brightness == 0 && d->state == LED_STATE_ON) {
                d->hal->set_state(d->id, 0);
                d->state = LED_STATE_OFF;
            }
            break;

        default:
            break;
        }
    }
}
