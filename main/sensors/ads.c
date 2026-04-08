#include "header.h"

static const char *TAG_ADS1 = "LOADCELL";
static const char *TAG_ADS2 = "TRANS";

static void loadcell_init(ads1256_handle_t *loadcell_handle) {
    /* Load Cell struct setup */
    ads1256_config_t loadcell_cfg = {
        .spi_host        = SPI_HOST,
        .cs              = LOADCELL_CS,
        .drdy            = LOADCELL_DRDY,
        .gain            = ADS1256_GAIN_1,
        .drate           = ADS1256_DRATE_1000SPS,
        .drdy_timeout_ms = ADS_DRDY_TIMEOUT_MS,
        .bufen           = false,
    };

    /* Load Cell initialization */
    ESP_ERROR_CHECK(ads1256_init(&loadcell_cfg, loadcell_handle));

    ESP_LOGI(TAG_ADS1, "ADS1 initialized");
}

static void transducer_init(ads1256_handle_t *trans_handle) {
    /* Pressure Transducer struct setup */
    ads1256_config_t trans_cfg = {
        .spi_host        = SPI_HOST,
        .cs              = TRANS_CS,
        .drdy            = TRANS_DRDY,
        .gain            = ADS1256_GAIN_1,
        .drate           = ADS1256_DRATE_1000SPS,
        .drdy_timeout_ms = ADS_DRDY_TIMEOUT_MS,
        .bufen           = false,
    };

    /* Pressure Transducer initialization */
    ESP_ERROR_CHECK(ads1256_init(&trans_cfg, trans_handle));

    ESP_LOGI(TAG_ADS2, "ADS2 initialized");
}

void ads_task(void *pvParameters) {
    ads1256_handle_t loadcell_handle;
    ads1256_handle_t transducer_handle;

    loadcell_init(&loadcell_handle);
    transducer_init(&transducer_handle);

    ads1256_sample_t ads_sample[2];

    while (true) {

        ads1256_start_conversion(loadcell_handle);
        ads1256_start_conversion(transducer_handle);

        // Load Cell + Pressure Transducer reading
        ads1256_read_result(loadcell_handle, &ads_sample[0].raw);
        ads1256_read_result(transducer_handle, &ads_sample[1].raw);

        // Update global sample
        xSemaphoreTake(xADSMutex, portMAX_DELAY);
        ads1256_sample_g[0] = ads_sample[0];
        ads1256_sample_g[1] = ads_sample[1];
        xSemaphoreGive(xADSMutex);
    }

    ESP_ERROR_CHECK(ads1256_delete(loadcell_handle));
    ESP_ERROR_CHECK(ads1256_delete(transducer_handle));
    vTaskDelete(NULL);
}