#include "internal-adc.h"
#include "esp_log.h"

// LOG tag name
const char *DAC_TAG = "DAC";

/**
 * @brief Confis a GPIO pin as a Trigger Input processor
 * 
 * @param channel 
 */
void config_gate_trigger_input(gpio_num_t channel) {
    gpio_config_t gate_config;
    gate_config.pin_bit_mask = 1ULL << channel;
    gate_config.mode = GPIO_MODE_INPUT;
    gate_config.pull_up_en = GPIO_PULLUP_ENABLE;
    gate_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gate_config.intr_type = GPIO_INTR_NEGEDGE;

    esp_err_t res_config = gpio_config(&gate_config);

    if (res_config != ESP_OK)  {
       ESP_LOGE(DAC_TAG, "Could not configure the gate trigger input"); 
    }
}