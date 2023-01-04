#include "driver/i2c.h"
#include "esp_log.h"

#define MCP4922_ADDR 0x60 // I2C address of the MCP4922 DAC

static const char *TAG = "MCP4922";

// Initialize the I2C interface and the MCP4922 DAC
void mcp4922_init() {
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    i2c_param_config(I2C_NUM_0, &config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    ESP_LOGI(TAG, "Initialized MCP4922 DAC");
}

// Write a value to the MCP4922 DAC
void mcp4922_write_dac(uint16_t value) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // Start a transmission to the MCP4922 DAC
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, MCP4922_ADDR << 1 | I2C_MASTER_WRITE, true);

    // Write the DAC value to the MCP4922
    i2c_master_write_byte(cmd, value >> 4, true); // Upper 8 bits
    i2c_master_write_byte(cmd, value << 4, true); // Lower 4 bits

    // Stop the transmission
    i2c_master_stop(cmd);

    // Execute the command
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error writing to MCP4922 DAC: %d", ret);
    }
}