#include "driver/gpio.h"
#include "driver/adc.h"

void config_gate_trigger_input(gpio_num_t channel);
bool adc_calibration_init(void);
void config_adc1_channel_with_defaults(adc1_channel_t channel);
int get_adc1_channel_raw(adc1_channel_t channel);