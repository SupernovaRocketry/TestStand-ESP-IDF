#include "global.h"

// SD & LITTLEFS CONFIG
#define SD_BUFFER_SIZE       4096
#define MAX_SD_FILES         5
#define SD_UNIT_SIZE         32 * 1024
#define SD_MOUNT             "/sdcard"
#define LITTLEFS_BUFFER_SIZE 512
#define MAX_LFS_FILES        32
#define FILENAME_LENGTH      32

// LORA CONFIG
#define LORA_FREQUENCY        915000000 // Hz
#define LORA_SPREADING_FACTOR 5
#define LORA_BANDWIDTH        SX126X_LORA_BW_125_0
#define LORA_CODING_RATE      SX126X_LORA_CR_4_5

static const char *TAG_SD       = "SD";
static const char *TAG_LITTLEFS = "LittleFS";
static const char *TAG_LORA     = "LoRa";

void task_sd(void *pvParameters) {
    esp_err_t     ret;
    sdmmc_card_t *card;

    ESP_LOGI(TAG_SD, "Initializing SD card");

    /* SDIO host driver (4-bit mode enabled, max frequency set to 20MHz) */
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    /* SDIO slot config */
    sdmmc_slot_config_t sd_cfg = {
        .clk     = SD_CLK,
        .cmd     = SD_CMD,
        .d0      = SD_DAT0,
        .d1      = SD_DAT1,
        .d2      = SD_DAT2,
        .d3      = SD_DAT3,
        .cd      = GPIO_NUM_NC,
        .gpio_wp = GPIO_NUM_NC,
        .width   = 4, // 4-bit mode
        .flags   = SDMMC_SLOT_FLAG_INTERNAL_PULLUP,
    };

    /* Options for mounting file system */
    esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files              = MAX_SD_FILES,
        .allocation_unit_size   = SD_UNIT_SIZE,
    };

    /* Mount filesystem */
    ESP_LOGI(TAG_SD, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(SD_MOUNT, &host, &sd_cfg, &mount_cfg, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG_SD, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(TAG_SD, "Failed to initialize the card (%s).", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG_SD, "Filesystem mounted");

    // TODO:
    //      Format mode
    //      add format mode
    // ...

    sdmmc_card_print_info(stdout, card);

    xEventGroupWaitBits(xSystemEvent, SAVE_DATA, pdFALSE, pdTRUE, portMAX_DELAY);

    /* Create log file */
    // ADICIONAR NVS COUNTER
    char log_name[FILENAME_LENGTH];
    snprintf(log_name, FILENAME_LENGTH, "%s/test%ld.bin", SD_MOUNT, 12345);
    ESP_LOGI(TAG_SD, "Creating file %s", log_name);

    FILE *f = fopen(log_name, "wb");
    if (!f) {
        ESP_LOGE(TAG_SD, "Failed to open file for writing");
        esp_vfs_fat_sdcard_unmount(SD_MOUNT, card);
        ESP_LOGI(TAG_SD, "Card unmounted");
        vTaskDelete(NULL);
    }
    // ...
}

static void lora_init(sx126x_handle_t *lora_handle) {
    /* SX1262 LoRa struct setup */
    sx126x_config_t lora_cfg = {
        .spi_host          = SPI_HOST,
        .ss                = LORA_CS,
        .reset             = LORA_RESET,
        .busy              = LORA_BUSY,
        .txen              = -1,
        .rxen              = -1,
        .frequency         = LORA_FREQUENCY,
        .tx_power          = 22,
        .tcxo_voltage      = 0.0f,
        .use_regulator_ldo = false,
        .spreading_factor  = LORA_SPREADING_FACTOR,
        .bandwidth         = LORA_BANDWIDTH,
        .coding_rate       = LORA_CODING_RATE,
        .preamble_length   = 12,    // 8 -> SF7-8 || 12 -> SF5-6
        .payload_len       = 0,     // Variable length packet
        .crc_on            = true,  // true -> drop garbage
        .invert_iq         = false, // false -> normal communication
    };

    /* SX1262 LoRa initialization */
    ESP_ERROR_CHECK(LoRaInit(&lora_cfg, lora_handle));
    LoRaDebugPrint(*lora_handle, false);
    ESP_ERROR_CHECK(LoRaBegin(*lora_handle));
    LoRaConfig(*lora_handle);

    ESP_LOGI(TAG_LORA, "SX1262 initialized");
}

void task_lora(void *pvParameters) {
    sx126x_handle_t lora_handle;
    bool            err;
    uint16_t        lost;

    lora_init(&lora_handle);

    // ADICIONAR ISR
    // ADICIONAR BATCH PACKET

    xEventGroupWaitBits(xSystemEvent, SEND_DATA, pdFALSE, pdTRUE, portMAX_DELAY);

    for (uint32_t i = 0; i < sys_data_g.ads_sample; i++) {
        err = LoRaSend(lora_handle, (uint8_t *)&ads_data_g[i], sizeof(ads_data_t), SX126x_TXMODE_SYNC);
        if (err)
            ESP_LOGI(TAG_LORA, "Sample %lu lost.", i);
    }

    lost = GetPacketLost(lora_handle);
    ESP_LOGI(TAG_LORA, "Samples lost: %u", lost);

    for (uint32_t i = 0; i < sys_data_g.max_sample; i++) {
        err = LoRaSend(lora_handle, (uint8_t *)&max_data_g[i], sizeof(max_data_t), SX126x_TXMODE_SYNC);
        if (err)
            ESP_LOGI(TAG_LORA, "Sample %lu lost.", i);
    }

    lost = GetPacketLost(lora_handle);
    ESP_LOGI(TAG_LORA, "Samples lost: %u", lost);

    vTaskDelete(NULL);
}