#include "driver/gpio.h"
#include "esp_log.h"

// Initialize the LED
void led_init(int channel) {
    // Configure the LED GPIO as an output
    gpio_config_t config = {
        .pin_bit_mask = 1ULL << channel,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&config);
}

// Turn the LED on
void led_on(int channel) {
    gpio_set_level(channel, 1);
}

// Turn the LED off
void led_off(int channel) {
    gpio_set_level(channel, 0);
}