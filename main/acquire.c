#include "header.h"

static const char *TAG_ACQ = "ACQUIRE";

static void status_check(void) {}

static void loadcell_init(ads1256_handle_t *loadcell_handle) {
    /* Load Cell struct setup */
    ads1256_config_t loadcell_cfg = {
        .spi_host        = SPI_HOST,
        .cs              = LOADCELL_CS,
        .drdy            = LOADCELL_DRDY,
        .gain            = ADS1256_GAIN_1,
        .drate           = ADS1256_DRATE_1000SPS,
        .pos_channel     = ADS1256_MUX_AIN0,
        .neg_channel     = ADS1256_MUX_AIN1,
        .drdy_timeout_ms = ADS_DRDY_TIMEOUT_MS,
        .bufen           = false,
    };

    /* Load Cell initialization */
    ESP_ERROR_CHECK(ads1256_init(&loadcell_cfg, loadcell_handle));

    ESP_LOGI(TAG_ACQ, "ADS1 initialized");
}

static void transducer_init(ads1256_handle_t *trans_handle) {
    /* Pressure Transducer struct setup */
    ads1256_config_t trans_cfg = {
        .spi_host        = SPI_HOST,
        .cs              = TRANS_CS,
        .drdy            = TRANS_DRDY,
        .gain            = ADS1256_GAIN_1,
        .drate           = ADS1256_DRATE_1000SPS,
        .pos_channel     = ADS1256_MUX_AIN0,
        .neg_channel     = ADS1256_MUX_AIN1,
        .drdy_timeout_ms = ADS_DRDY_TIMEOUT_MS,
        .bufen           = false,
    };

    /* Pressure Transducer initialization */
    ESP_ERROR_CHECK(ads1256_init(&trans_cfg, trans_handle));

    ESP_LOGI(TAG_ACQ, "ADS2 initialized");
}

void task_max(void *pvParameters) {
    // MAX INIT
    // ...

    // LOGICA DE COLETA DOS MAX
    while (true) {

        if ((sys_temp_g.status && FULL_ACQ) || (sys_temp_g.status && TEMP_ACQ)) {
            // adiciona max_read_result();
            sys_temp_g.max1 = 0;
            sys_temp_g.max2 = 0;
            sys_temp_g.max3 = 0;
        }
        check_status(&sys_temp_g);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void task_acquire(void *pvParameters) {
    ads1256_handle_t loadcell_handle;
    ads1256_handle_t transducer_handle;

    int32_t current_loadcell;
    int32_t current_transducer;

    loadcell_init(&loadcell_handle);
    transducer_init(&transducer_handle);

    while (true) {
        // SAVE DATA TO PSRAM
        if (sys_temp_g.status |= FULL_ACQ) {
            ads1256_start_conversion(loadcell_handle);
            ads1256_start_conversion(transducer_handle);

            // Load Cell + Pressure Transducer reading
            ads1256_read_result(loadcell_handle, &current_loadcell);
            ads1256_read_result(transducer_handle, &current_transducer);
        }

        data_g[sys_temp_g.sample].loadcell  = current_loadcell;
        data_g[sys_temp_g.sample].trans     = current_transducer;
        data_g[sys_temp_g.sample].max1      = sys_temp_g.max1;
        data_g[sys_temp_g.sample].max2      = sys_temp_g.max2;
        data_g[sys_temp_g.sample].max3      = sys_temp_g.max3;
        data_g[sys_temp_g.sample].status    = sys_temp_g.sample;
        data_g[sys_temp_g.sample].timestamp = (uint32_t)esp_timer_get_time();

        sys_temp_g.sample++; // hmm
    }

    ESP_ERROR_CHECK(ads1256_delete(loadcell_handle));
    ESP_ERROR_CHECK(ads1256_delete(transducer_handle));
    vTaskDelete(NULL);
}