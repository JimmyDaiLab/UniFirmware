// ============================
// pump_driver.c (Event + Table Driven, Abstract HAL)
// ============================
#include "pump_driver.h"

#define ANY_STATE 0xFF

// ----------------------------
// Command Queue
// ----------------------------
static bool dequeue(pump_driver_t *p, pump_cmd_t *cmd) {
    if (p->cmd_q.head == p->cmd_q.tail)
        return false;

    *cmd = p->cmd_q.queue[p->cmd_q.tail];
    p->cmd_q.tail = (p->cmd_q.tail + 1) % CMD_QUEUE_SIZE;
    return true;
}

void pump_driver_enqueue_cmd(pump_driver_t *p, pump_cmd_type_t type, uint32_t param) {
    uint8_t next = (p->cmd_q.head + 1) % CMD_QUEUE_SIZE;
    if (next == p->cmd_q.tail)
        return; // full

    p->cmd_q.queue[p->cmd_q.head].type = type;
    p->cmd_q.queue[p->cmd_q.head].param = param;
    p->cmd_q.head = next;
}

// ----------------------------
// Init
// ----------------------------
void pump_driver_init(pump_driver_t *p, uint8_t id, pump_hal_t *hal) {
    p->id = id;
    p->state = PUMP_IDLE;
    p->pump_pulse = 0;
    p->flow_pulse = 0;
    p->hal = hal;
    p->cmd_q.head = p->cmd_q.tail = 0;
    p->event_stop = 0;
    p->event_start = 0;
}

// ----------------------------
// Command ¡ú Event
// ----------------------------
static void handle_cmd(pump_driver_t *p, pump_cmd_t *cmd) {
    switch (cmd->type) {
    case CMD_STOP:
        p->event_stop = 1;
        break;

    case CMD_START_DISPENSE:
        p->target_flow_pulse = cmd->param;
        p->event_start = 1;
        break;

    case CMD_CLEAR_PULSE:
        p->pump_pulse = 0;
        p->flow_pulse = 0;
        break;

    default:
        break;
    }
}

// ----------------------------
// Event Generator
// ----------------------------
static pump_event_t get_event(pump_driver_t *p) {
    if (p->event_stop) {
        p->event_stop = 0;
        return EVT_STOP;
    }

    if (p->event_start) {
        p->event_start = 0;
        return EVT_START;
    }

    if (p->flow_pulse >= p->target_flow_pulse && p->state == PUMP_DISPENSE_1)
        return EVT_FLOW_REACHED;

    if ((p->hal->get_tick() - p->timestamp) >= 230 && p->state == PUMP_DISPENSE_2)
        return EVT_TIMEOUT;

    if (p->pump_pulse >= p->target_pump_pulse && p->state == PUMP_DISPENSE_3)
        return EVT_PUMP_REACHED;

    return EVT_NONE;
}

// ----------------------------
// Actions
// ----------------------------
static void action_start(pump_driver_t *p) {
    p->hal->set_dir(p->id, 1);
    p->hal->set_pwm(p->id, 500);
}

static void action_stop_pwm(pump_driver_t *p) {
    p->hal->set_pwm(p->id, 0);
    p->timestamp = p->hal->get_tick();
}

static void action_reverse(pump_driver_t *p) {
    p->pump_pulse = 0;
    p->hal->set_dir(p->id, 0);
    p->hal->set_pwm(p->id, 500);
}

static void action_finish(pump_driver_t *p) {
    p->hal->set_pwm(p->id, 0);
}

static void action_force_stop(pump_driver_t *p) {
    p->hal->set_pwm(p->id, 0);
}

// ----------------------------
// Transition Table
// ----------------------------
typedef struct {
    uint8_t state;
    pump_event_t event;
    void (*action)(pump_driver_t *);
    uint8_t next_state;
} state_trans_t;

static const state_trans_t table[] = {
    {PUMP_IDLE, EVT_START, action_start, PUMP_DISPENSE_1},

    {PUMP_DISPENSE_1, EVT_FLOW_REACHED, action_stop_pwm, PUMP_DISPENSE_2},

    {PUMP_DISPENSE_2, EVT_TIMEOUT, action_reverse, PUMP_DISPENSE_3},

    {PUMP_DISPENSE_3, EVT_PUMP_REACHED, action_finish, PUMP_IDLE},

    // Global STOP
    {ANY_STATE, EVT_STOP, action_force_stop, PUMP_IDLE},
};

#define TABLE_SIZE (sizeof(table)/sizeof(table[0]))

// ----------------------------
// Process
// ----------------------------
void pump_driver_process(pump_driver_t *p) {
    pump_cmd_t cmd;

    // 1. command ¡ú event flag
    if (dequeue(p, &cmd)) {
        handle_cmd(p, &cmd);
    }

    // 2. event detect
    pump_event_t evt = get_event(p);
    if (evt == EVT_NONE)
        return;

    // 3. lookup table
    for (int i = 0; i < TABLE_SIZE; i++) {
        if ((table[i].state == p->state || table[i].state == ANY_STATE) &&
            table[i].event == evt) {

            if (table[i].action)
                table[i].action(p);

            p->state = table[i].next_state;
            break;
        }
    }
}

// ----------------------------
// ISR
// ----------------------------
void pump_driver_isr_pulse(pump_driver_t *p) {
    p->pump_pulse++;
}

void pump_driver_flow_isr_pulse(pump_driver_t *p) {
    p->flow_pulse++;
}