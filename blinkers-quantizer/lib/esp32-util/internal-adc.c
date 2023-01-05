#include "internal-adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

// LOG tag name
const char *DAC_TAG = "DAC";

// ADC Attenuation
#define ADC_ATTEN ADC_ATTEN_DB_11

// ADC Calibration
#if CONFIG_IDF_TARGET_ESP32
#define ADC_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_VREF
#elif CONFIG_IDF_TARGET_ESP32S2
#define ADC_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_TP
#elif CONFIG_IDF_TARGET_ESP32C3
#define ADC_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_TP
#elif CONFIG_IDF_TARGET_ESP32S3
#define ADC_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_TP_FIT
#endif

// holds calibration data
static esp_adc_cal_characteristics_t adc1_chars;

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
/**
 * @brief Initializes and checks the ADC calibration
 * 
 * @return true 
 * @return false 
 */
bool adc_calibration_init(void) {
    esp_err_t ret;
    bool cali_enable = 0;

    ret = esp_adc_cal_check_efuse(ADC_CALI_SCHEME);

    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGI(DAC_TAG, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGI(DAC_TAG, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        ESP_LOGI(DAC_TAG, "Calibration scheme enabled");
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc1_chars);
    } else {
        ESP_LOGE(DAC_TAG, "Invalid arg");
    }

    return cali_enable;
}
/**
 * @brief Set ADC Channel resolution and attenuation with the default max values
 * 
 * @param channel 
 */
void config_adc1_channel_with_defaults(adc1_channel_t channel) {
    esp_err_t res_width, res_atten;

    res_width = adc1_config_width(ADC_WIDTH_BIT_12);
    res_atten = adc1_config_channel_atten(channel, ADC_ATTEN_DB_11);

    if (res_width != ESP_OK)  {
       ESP_LOGE(DAC_TAG, "Could not configure the width for channel: %d - Error: %x", channel, res_width); 
    }

    if (res_atten != ESP_OK)  {
       ESP_LOGE(DAC_TAG, "Could not configure the attenuation for channel: %d - Error: %x", channel, res_atten); 
    }
}
/**
 * @brief Get the adc1 channel raw object
 * 
 * @param channel 
 */
int get_adc1_channel_raw(adc1_channel_t channel) {
    return adc1_get_raw(channel);
}