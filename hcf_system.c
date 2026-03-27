// ========== hcf_system.c ==========
#include "hcf_system.h"

device_socket_t *hcf_device_registry[HCF_MAX_DEVICES] = {0};

void hcf_init(void)
{
    for (int i = 0; i < HCF_MAX_DEVICES; i++) {
        hcf_device_registry[i] = NULL;
    }
}

bool hcf_register_device(device_socket_t *device)
{
    if (!device) return false;
    for (int i = 0; i < HCF_MAX_DEVICES; i++) {
        if (hcf_device_registry[i] == NULL) {
            hcf_device_registry[i] = device;
            device->id = (uint8_t)i;
            device->state = DEVICE_OFFLINE;
            return true;
        }
    }
    return false;
}

void hcf_process(void)
{
    for (int i = 0; i < HCF_MAX_DEVICES; i++) {
        if (hcf_device_registry[i] && hcf_device_registry[i]->state != DEVICE_OFFLINE) {
            hcf_device_registry[i]->process(hcf_device_registry[i]);
        }
    }
}

bool hcf_send_command(uint8_t device_id, device_cmd_t *cmd)
{
    if (device_id >= HCF_MAX_DEVICES || !hcf_device_registry[device_id] || !cmd)
        return false;

    device_socket_t *dev = hcf_device_registry[device_id];
    if (dev->handle_cmd) {
        bool ret = dev->handle_cmd(dev, cmd);
        if (ret) {
            if (dev->state == DEVICE_OFFLINE)
                dev->state = DEVICE_IDLE;
            return true;
        }
    }
    return false;
}
