#include "esp_log.h"
#include "isr.h"
#include "internal-adc.h"

#define DEBUG_LED 4
#define GATE_IN 39

const char *MAIN_TAG = "MAIN";

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void process_gate_input() {
    gpio_set_level(DEBUG_LED, 1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(DEBUG_LED, 0);
}

static void gpio_isr_handler_task(void* arg) {
    uint32_t io_num;
    
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

            switch (io_num) {
                case GATE_IN: process_gate_input(); break;
                default: break;
            }
        }
    }
}

void app_main(void) {
    // Configure DEBUG LED as output
    gpio_pad_select_gpio(DEBUG_LED);
    gpio_set_direction(DEBUG_LED, GPIO_MODE_OUTPUT);
    // / DEBUG

    install_isr_service();
    config_gate_trigger_input(GATE_IN);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_isr_handler_task, "gpio_isr_handler_task", 2048, NULL, 10, NULL);

    esp_err_t res_handler = gpio_isr_handler_add(GATE_IN, gpio_isr_handler, (void*) GATE_IN);

    if (res_handler != ESP_OK)  {
       ESP_LOGE(MAIN_TAG, "Could not add an handler for the gate input"); 
    }
}