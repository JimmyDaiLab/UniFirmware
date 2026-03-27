// ========== hcf_device.h ==========
#ifndef HCF_DEVICE_H
#define HCF_DEVICE_H

#include <stdint.h>
#include <stdbool.h>

#define DEVICE_CMD_START_DISPENSE 1
#define DEVICE_CMD_STOP 2
#define DEVICE_CMD_SET_PARAM 3

typedef struct {
    uint8_t type;
    uint32_t param1;
    uint32_t param2;
} device_cmd_t;

typedef enum {
    DEVICE_IDLE,
    DEVICE_RUNNING,
    DEVICE_ERROR,
    DEVICE_OFFLINE
} device_state_t;

typedef struct device_socket_s {
    uint8_t id;
    const char *name;
    device_state_t state;

    bool (*init)(struct device_socket_s *self, void *hal);
    void (*process)(struct device_socket_s *self);
    bool (*handle_cmd)(struct device_socket_s *self, device_cmd_t *cmd);
    void (*get_status)(struct device_socket_s *self, uint8_t *status_buf, uint32_t *status_len);

    void *private_data;
} device_socket_t;

#endif // HCF_DEVICE_H
