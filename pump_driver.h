// ============================
// pump_driver.h (Event + Table Driven, Abstract HAL)
// ============================
#ifndef PUMP_DRIVER_H
#define PUMP_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// Abstract HAL interface (platform-independent)
typedef struct {
    void (*set_pwm)(uint8_t id, uint16_t duty);
    void (*set_dir)(uint8_t id, uint8_t dir);
    uint32_t (*get_tick)(void);
} pump_hal_t;

typedef enum {
    CMD_NONE,
    CMD_STOP,
    CMD_START_DISPENSE,
    CMD_CLEAR_PULSE,
} pump_cmd_type_t;

typedef struct {
    pump_cmd_type_t type;
    uint32_t param;
} pump_cmd_t;

typedef enum {
    EVT_NONE,
    EVT_STOP,
    EVT_START,
    EVT_FLOW_REACHED,
    EVT_TIMEOUT,
    EVT_PUMP_REACHED,
} pump_event_t;

typedef enum {
    PUMP_IDLE,
    PUMP_DISPENSE_1,
    PUMP_DISPENSE_2,
    PUMP_DISPENSE_3,
} pump_state_t;

#define CMD_QUEUE_SIZE 8

typedef struct {
    pump_cmd_t queue[CMD_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} pump_cmd_queue_t;

typedef struct {
    uint8_t id;

    pump_state_t state;

    volatile uint32_t pump_pulse;
    volatile uint32_t flow_pulse;

    uint32_t target_pump_pulse;
    uint32_t target_flow_pulse;

    uint32_t timestamp;

    pump_hal_t *hal;

    pump_cmd_queue_t cmd_q;

    uint8_t event_stop;
    uint8_t event_start;

} pump_driver_t;

void pump_driver_init(pump_driver_t *p, uint8_t id, pump_hal_t *hal);
void pump_driver_process(pump_driver_t *p);
void pump_driver_enqueue_cmd(pump_driver_t *p, pump_cmd_type_t type, uint32_t param);

void pump_driver_isr_pulse(pump_driver_t *p);
void pump_driver_flow_isr_pulse(pump_driver_t *p);

#endif