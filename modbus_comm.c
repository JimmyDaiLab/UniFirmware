// ========== modbus_comm.c ==========
#include "modbus_comm.h"
#include "hcf_system.h"
#include "pump_device.h"
#include <stdbool.h>
#include <stdio.h>

static bool modbus_started = false;

void modbus_comm_init(void)
{
    modbus_started = false;
}

void modbus_comm_poll(void)
{
    if (!modbus_started) {
        device_cmd_t cmd = { .type = PUMP_CMD_START_DISPENSE, .param1 = 100, .param2 = 0 };
        if (hcf_send_command(0, &cmd)) {
            // printf("modbus: pump 0 start dispense 100\n");
        }

        modbus_started = true;
        return;
    }

    static bool stop_sent = false;
    if (!stop_sent) {
        device_cmd_t cmd = { .type = DEVICE_CMD_STOP, .param1 = 0, .param2 = 0 };
        if (hcf_send_command(0, &cmd)) {
            // printf("modbus: pump 0 stop\n");
        }
        stop_sent = true;
    }
}
