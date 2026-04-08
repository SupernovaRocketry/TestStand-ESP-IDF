#include "header.h"

static const char *TAG_ADS1 = "LOADCELL";
static const char *TAG_ADS2 = "TRANS";

static void loadcell_init(ads1256_handle_t *loadcell_handle)
{
    /* Load Cell struct setup */
    ads1256_config_t loadcell_cfg = {
        .spi_host = SPI_HOST,
        .cs = LOADCELL_CS,
        .drdy = LOADCELL_DRDY,
        .reset = GPIO_NUM_NC,
        .gain = ADS1256_GAIN_1,
        .drate = ADS1256_DRATE_1000SPS,
        .drdy_timeout_ms = ADS_DRDY_TIMEOUT_MS,
        .ads_transfer_size = ADS_TRANSFER_SIZE,
        .bufen = false,
    };

    /* Load Cell initialization */
    ESP_ERROR_CHECK(ads1256_init(&loadcell_cfg, loadcell_handle));

    ESP_LOGI(TAG_ADS1, "ADS1 initialized");
    vTaskDelay(pdMS_TO_TICKS(100)); // ?


}

static void transducer_init(ads1256_gain_t *trans_handle)
{
    /* Pressure Transducer struct setup */
    ads1256_config_t trans_cfg = {
        .spi_host = SPI_HOST,
        .cs = TRANS_CS,
        .drdy = TRANS_DRDY,
        .reset = GPIO_NUM_NC,
        .gain = ADS1256_GAIN_1,
        .drate = ADS1256_DRATE_1000SPS,
        .drdy_timeout_ms = ADS_DRDY_TIMEOUT_MS,
        .ads_transfer_size = ADS_TRANSFER_SIZE,
        .bufen = false,
    };

    ESP_ERROR_CHECK(ads1256_init(&trans_cfg, trans_handle));

    ESP_LOGI(TAG_ADS2, "ADS2 initialized");
    vTaskDelay(pdMS_TO_TICKS(100)); // ?


}

void loadcell_task(void *pvParameters)
{
    ads1256_handle_t loadcell_handle;
    ads1256_handle_t transducer_handle;

    loadcell_init(loadcell_handle);
    transducer_init(transducer_handle);

}