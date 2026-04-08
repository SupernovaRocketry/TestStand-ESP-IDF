#include "header.h"

static const char *TAG_MAIN = "MAIN";

static void setup_peripherals(void)
{
    // SPI bus configuration
    spi_bus_config_t spi_bus_config = {
        .mosi_io_num = MOSI,
        .miso_io_num = MISO,
        .sclk_io_num = CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SD_BUFFER_SIZE,
    };

    // SPI host setup
    spi_host_device_t host = SPI_HOST;

    // SPI dma setup
    spi_dma_chan_t dma_chan = DMA_CHAN;

    // SPI bus initialization (2 ADS1256, 1 MAX31865 and 2 MAX6675)
    ESP_ERROR_CHECK(spi_bus_initialize(host, &spi_bus_config, dma_chan));
}

void app_main(void)
{
    ESP_LOGI(TAG_MAIN, "Starting main application");
    setup_peripherals();
}