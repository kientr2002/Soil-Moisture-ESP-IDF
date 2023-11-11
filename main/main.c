#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"


/*SET UP PORT:
VCC - 3V port
GND - GND port
DO - NOTHING
AO - GPIO34 port
*/
static const char *TAG = "ADC EXAMPLE";

static esp_adc_cal_characteristics_t adc1_chars;

void app_main(void)
{
    uint32_t voltage;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11)); //GPIO 34

    while (1) 
    {
        // ADC and soil moisture parameter
        float dry_adc_value = 4095.0;
        float wet_adc_value = 1400.0; // The estimate is based on measured facts.
        float wet_soil_moisture = 100.0; 
        voltage = adc1_get_raw(ADC1_CHANNEL_6);     //GPIO 34  
        float soil_moisture_percentage = (dry_adc_value - voltage)/(dry_adc_value - wet_adc_value) * wet_soil_moisture;

        if(soil_moisture_percentage > 100){
          soil_moisture_percentage = 100;
        }
        
        ESP_LOGI(TAG, "ADC1_CHANNEL_6: %ld and Soil Moisture Percentage is: %f (%%)\n", voltage, soil_moisture_percentage);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }