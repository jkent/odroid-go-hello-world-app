#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef enum {
    EVENT_TYPE_KEYPAD,
} event_type_t;

typedef struct {
    event_type_t type;
} event_head_t;

typedef struct {
    event_head_t head;
    uint16_t state;
    uint16_t pressed;
    uint16_t released;
} event_keypad_t;

typedef union {
    event_type_t type;
    event_keypad_t keypad;
} event_t;

extern QueueHandle_t event_queue;

void event_init(void);
