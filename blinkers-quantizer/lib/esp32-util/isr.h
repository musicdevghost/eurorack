#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

// Global GPIO Event Queue
static xQueueHandle gpio_evt_queue = NULL;

void install_isr_service();