#include "vox-adc.h"
#include "driver/i2c.h"
#include "esp_log.h"

static esp_adc_cal_characteristics_t adc1_chars;

static const char *TAG = "ADC";

void calibrate_adc1_channel(adc1_channel_t channel, adc_atten_t atten) {
    esp_adc_cal_characterize(ADC_UNIT_1, atten, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(channel, atten));
}

int readRawDataFromADC1(adc1_channel_t channel) {
    return adc1_get_raw(channel);
}

uint32_t readAnalogVoltageFromADC1(adc1_channel_t channel) {
    return esp_adc_cal_raw_to_voltage(readRawDataFromADC1(channel), &adc1_chars);
}

void calibrate_gate_input(int channel) {
    gpio_config_t config = {
        .pin_bit_mask = 1ULL << channel,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };

    esp_err_t res = gpio_config(&config);

    if (res != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing channel as gate: %d", channel);
    }
}