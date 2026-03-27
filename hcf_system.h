// ========== hcf_system.h ==========
#ifndef HCF_SYSTEM_H
#define HCF_SYSTEM_H

#include "hcf_device.h"

#define HCF_MAX_DEVICES 16

extern device_socket_t *hcf_device_registry[HCF_MAX_DEVICES];

bool hcf_register_device(device_socket_t *device);
void hcf_init(void);
void hcf_process(void);
bool hcf_send_command(uint8_t device_id, device_cmd_t *cmd);

#endif // HCF_SYSTEM_H
