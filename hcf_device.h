// ========== hcf_device.h ==========
#ifndef HCF_DEVICE_H
#define HCF_DEVICE_H

#include <stdint.h>
#include <stdbool.h>

#define DEVICE_CMD_STOP 0x00

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

typedef enum {
    DEVICE_EVT_IDLE,
    DEVICE_EVT_STARTED,
    DEVICE_EVT_ERROR,
} device_event_type_t;

typedef void (*device_event_cb_t)(uint8_t device_id, device_event_type_t event);

typedef struct device_socket_s {
    uint8_t id;
    const char *name;
    device_state_t state;

    bool (*init)(struct device_socket_s *self, void *hal);
    void (*process)(struct device_socket_s *self);
    bool (*handle_cmd)(struct device_socket_s *self, device_cmd_t *cmd);
    void (*get_status)(struct device_socket_s *self, uint8_t *status_buf, uint32_t *status_len);

    void *hal_ctx;
    void *private_data;
} device_socket_t;

#endif // HCF_DEVICE_H
