// ============================
// main.c
// ============================
#include "hcf_system.h"
#include "led_device.h"
#include "stm32_led_hal.h"
#include "modbus_comm.h"

int main(void)
{
    hcf_init();

    device_socket_t *led = led_device_create(0, &stm32_led_hal);
    if (led == NULL) {
        return -1;
    }

    if (!hcf_register_device(led)) {
        return -1;
    }

    modbus_comm_init();

    while (1) {
        modbus_comm_poll();
        hcf_process();
    }

    return 0;
}
