// ============================
// main.c
// ============================
#include "hcf_system.h"
#include "pump_device.h"
#include "stm32_pump_hal.h"
#include "modbus_comm.h"

int main(void)
{
    hcf_init();

    device_socket_t *pump = pump_device_create(0, &stm32_pump_hal);
    if (pump == NULL) {
        return -1;
    }

    if (!hcf_register_device(pump)) {
        return -1;
    }

    modbus_comm_init();

    while (1) {
        modbus_comm_poll();
        hcf_process();
    }

    return 0;
}
