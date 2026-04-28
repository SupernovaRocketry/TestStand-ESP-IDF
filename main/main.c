#include "header.h"

static const char *TAG_MAIN = "MAIN";

static void setup_memory(void) {
    // sdkconfig -> + Support for external, SPI-connected RAM
    ads_data_g = (ads_data_t *)heap_caps_aligned_alloc(4, ADS_SAMPLES * sizeof(ads_data_t), MALLOC_CAP_SPIRAM);
    max_data_g = (max_data_t *)heap_caps_aligned_alloc(4, MAX_SAMPLES * sizeof(max_data_t), MALLOC_CAP_SPIRAM);

    if (ads_data_g == NULL) {
        ESP_LOGE(TAG_MAIN, "Failed to allocate PSRAM for ADS data");
        // IMPLEMENTAR ERROR HANDLING -------------------------------
        return;
    }

    if (max_data_g == NULL) {
        ESP_LOGE(TAG_MAIN, "Failed to allocate PSRAM for MAX data");
        // IMPLEMENTAR ERROR HANDLING -------------------------------
        return;
    }
}

static void setup_peripherals(void) {
    // SPI bus configuration
    spi_bus_config_t spi_bus_cfg = {
        .mosi_io_num     = MOSI,
        .miso_io_num     = MISO,
        .sclk_io_num     = CLK,
        .quadwp_io_num   = -1,
        .quadhd_io_num   = -1,
        .max_transfer_sz = SD_BUFFER_SIZE,
    };

    // SPI host setup
    spi_host_device_t host = SPI_HOST;

    // SPI dma setup
    spi_dma_chan_t dma_chan = DMA_CHAN;

    // SPI bus initialization (2 ADS1256, 1 MAX31865 and 2 MAX6675)
    ESP_ERROR_CHECK(spi_bus_initialize(host, &spi_bus_cfg, dma_chan));

    // DRDY config with ISR
    gpio_config_t drdy_conf = {
        .pin_bit_mask = (1ULL << LOADCELL_DRDY),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_NEGEDGE // Trigger when DRDY goes LOW
    };

    // Apply DDRY config
    ESP_ERROR_CHECK(gpio_config(&drdy_conf));

    // Start ISR
    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
}

void app_main(void) {
    ESP_LOGI(TAG_MAIN, "Starting main application");

    // TESTAR

    ESP_LOGI(TAG_MAIN,
             "Free Heap: %u bytes\n"
             "  MALLOC_CAP_8BIT      %7zu bytes\n"
             "  MALLOC_CAP_DMA       %7zu bytes\n"
             "  MALLOC_CAP_SPIRAM    %7zu bytes\n"
             "  MALLOC_CAP_INTERNAL  %7zu bytes\n"
             "  MALLOC_CAP_DEFAULT   %7zu bytes\n"
             "  MALLOC_CAP_IRAM_8BIT %7zu bytes\n"
             "  MALLOC_CAP_RETENTION %7zu bytes\n",
             xPortGetFreeHeapSize(), heap_caps_get_free_size(MALLOC_CAP_8BIT), heap_caps_get_free_size(MALLOC_CAP_DMA),
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM), heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
             heap_caps_get_free_size(MALLOC_CAP_DEFAULT), heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT),
             heap_caps_get_free_size(MALLOC_CAP_RETENTION));

    // TESTAR

    setup_memory();
    setup_peripherals();
    vTaskDelay(pdMS_TO_TICKS(150)); // Wait for peripherals to stabilize

    /* Create Mutexes */
    // xDATAMutex = xSemaphoreCreateMutex();

    // TESTAR

    ESP_LOGI(TAG_MAIN,
             "Free Heap: %u bytes\n"
             "  MALLOC_CAP_8BIT      %7zu bytes\n"
             "  MALLOC_CAP_DMA       %7zu bytes\n"
             "  MALLOC_CAP_SPIRAM    %7zu bytes\n"
             "  MALLOC_CAP_INTERNAL  %7zu bytes\n"
             "  MALLOC_CAP_DEFAULT   %7zu bytes\n"
             "  MALLOC_CAP_IRAM_8BIT %7zu bytes\n"
             "  MALLOC_CAP_RETENTION %7zu bytes\n",
             xPortGetFreeHeapSize(), heap_caps_get_free_size(MALLOC_CAP_8BIT), heap_caps_get_free_size(MALLOC_CAP_DMA),
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM), heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
             heap_caps_get_free_size(MALLOC_CAP_DEFAULT), heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT),
             heap_caps_get_free_size(MALLOC_CAP_RETENTION));

    // TESTAR

    /* Create Tasks */
    // Verificar parametros de criação da task
    xTaskCreatePinnedToCore(task_ads, "ADS", configMINIMAL_STACK_SIZE * 8, NULL, 10, &xTaskAds, 1);
    xTaskCreatePinnedToCore(task_max, "MAX", configMINIMAL_STACK_SIZE * 8, NULL, 3, NULL, 0);
    // task sd
    // task littlefs
    // task lora
}