#include "global.h"

static const char *TAG_SD       = "SD";
static const char *TAG_LITTLEFS = "LittleFS";
static const char *TAG_LORA     = "LoRa";

static void sd_init(void) {
    esp_err_t     ret;
    sdmmc_card_t *card;

    // SDIO host driver (4-bit mode enabled, max frequency set to 20MHz)
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    // SDIO slot config
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

    // Options for mounting file system
    esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files              = MAX_SD_FILES,
        .allocation_unit_size   = SD_UNIT_SIZE,
    };

    // Mount filesystem
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
}

static void lora_init() {
    esp_err_t ret;

    ESP_LOGI(TAG_LORA, "Initializing LoRa");
    LoRaInit();
}

void task_sd(void *pvParameters) {
    ESP_LOGI(TAG_SD, "Initializing SD card");
    sd_init();

    xEventGroupWaitBits(xSystemEvent, SAVE_DATA, pdFALSE, pdTRUE, portMAX_DELAY);

    // ...
}

void task_lora() {

    while (true) {
    }
}