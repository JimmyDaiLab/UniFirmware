// ========== hcf_system.h ==========
#ifndef HCF_SYSTEM_H
#define HCF_SYSTEM_H

#include "hcf_device.h"

#define HCF_MAX_DEVICES 16

void hcf_init(void);
bool hcf_register_device(device_socket_t *device);
device_socket_t *hcf_get_device(uint8_t device_id);
void hcf_process(void);
bool hcf_send_command(uint8_t device_id, device_cmd_t *cmd);
void hcf_set_event_callback(device_event_cb_t cb);
void hcf_notify_event(uint8_t device_id, device_event_type_t event);

#endif // HCF_SYSTEM_H
