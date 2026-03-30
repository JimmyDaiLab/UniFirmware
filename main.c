// ============================
// main.c
// ============================
#include "hcf_system.h"
#include "modbus_comm.h"

int main(void)
{
    hcf_init();

        return -1;
    }

        return -1;
    }

    modbus_comm_init();

    while (1) {
        modbus_comm_poll();
        hcf_process();
    }

    return 0;
}
