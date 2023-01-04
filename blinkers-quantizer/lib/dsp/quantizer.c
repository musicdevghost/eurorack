#include "quantizer.h"
#include "esp_log.h"
#include <math.h>

int current_map_size = 0;
uint16_t *current_input_map;
uint32_t *current_output_map;

uint16_t SEMI_TONES_SCALE_INPUT_MAP[] = {341, 682, 1023, 1364, 1705, 2046, 2387, 2728, 3069, 3410, 3751, 4092};

static const char *TAG = "QUANTIZER";

void set_map(scale_t scale_ref) {
    current_input_map = SEMI_TONES_SCALE_INPUT_MAP;

    switch (scale_ref) {
        case SEMI_TONES:
            current_input_map = SEMI_TONES_SCALE_INPUT_MAP;
            // current_output_map = SEMI_TONES_SCALE_OUTPUT_MAP;
            current_map_size = 12;
        default:
            break;
    }

    return;
}

uint32_t get_closest_scale_value(uint32_t value, uint32_t scale_ref) {
    set_map(scale_ref);

    if (value > 4095) value = 4095;

    int min_diff = abs(current_input_map[0] - value);
    int index = 0;

    for (int i = 1; i < current_map_size; i++) {
        int diff = abs(current_input_map[i] - value);

        if (diff < min_diff) {
            min_diff = diff;
            index = i;
        }
    }

    return current_input_map[index];
}

// quantizes and returns the value for use in an 8 BIT DAC
uint8_t quantize_to_scale_8b(uint32_t value, scale_t scale_ref) {
    ESP_LOGI(TAG, "=================================================================================");

    uint32_t quantized = get_closest_scale_value(value, scale_ref);

    // 8 bit version
    uint8_t dac_8bit_value = (uint8_t)(quantized * 255.0f / 4095.0f * 8.0f);

    ESP_LOGI(TAG, "SRC: %d", value);
    ESP_LOGI(TAG, "DST: %d", quantized);
    ESP_LOGI(TAG, "8 BIT: %d", dac_8bit_value);

    ESP_LOGI(TAG, "=================================================================================");

    return quantized;
}