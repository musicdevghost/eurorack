#include "isr.h"
#include "esp_log.h"

// Default priority for INTR
#define ESP_INTR_FLAG_DEFAULT 0

// LOG tag name
const char *ISR_TAG = "ISR";

/**
 * @brief Enables ISR service
 * 
 */
void install_isr_service() {
    esp_err_t res = gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

   if (res != ESP_OK)  {
       ESP_LOGE(ISR_TAG, "Could not install the ISR Service"); 
   }
}
