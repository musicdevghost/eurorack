#include "esp_adc_cal.h"

#define ADC_WIDTH ADC_WIDTH_BIT_12

void calibrate_adc1_channel(adc1_channel_t channel, adc_atten_t atten);
float readScaledVoltageFromADC1(adc1_channel_t channel);
int readRawDataFromADC1(adc1_channel_t channel);
uint32_t readAnalogVoltageFromADC1(adc1_channel_t channel);
void calibrate_gate_input(int channel);