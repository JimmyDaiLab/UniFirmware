// ========== hcf_system.c ==========
#include "hcf_system.h"
#include <stddef.h>

static device_socket_t *device_registry[HCF_MAX_DEVICES] = {0};
static device_event_cb_t event_cb = NULL;

void hcf_init(void)
{
    for (int i = 0; i < HCF_MAX_DEVICES; i++) {
        device_registry[i] = NULL;
    }
    event_cb = NULL;
}

bool hcf_register_device(device_socket_t *device)
{
    if (!device) return false;
    for (int i = 0; i < HCF_MAX_DEVICES; i++) {
        if (device_registry[i] == NULL) {
            device_registry[i] = device;
            device->id = (uint8_t)i;

            if (device->init) {
                device->init(device, device->hal_ctx);
            }
            return true;
        }
    }
    return false;
}

device_socket_t *hcf_get_device(uint8_t device_id)
{
    if (device_id >= HCF_MAX_DEVICES)
        return NULL;
    return device_registry[device_id];
}

void hcf_process(void)
{
    for (int i = 0; i < HCF_MAX_DEVICES; i++) {
        if (device_registry[i] && device_registry[i]->state != DEVICE_OFFLINE) {
            device_registry[i]->process(device_registry[i]);
        }
    }
}

bool hcf_send_command(uint8_t device_id, device_cmd_t *cmd)
{
    if (device_id >= HCF_MAX_DEVICES || !device_registry[device_id] || !cmd)
        return false;

    device_socket_t *dev = device_registry[device_id];
    if (dev->handle_cmd) {
        return dev->handle_cmd(dev, cmd);
    }
    return false;
}

void hcf_set_event_callback(device_event_cb_t cb)
{
    event_cb = cb;
}

void hcf_notify_event(uint8_t device_id, device_event_type_t event)
{
    if (event_cb) {
        event_cb(device_id, event);
    }
}
