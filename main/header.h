#ifndef HEADER_H
#define HEADER_H

// INCLUDES
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <esp_check.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_psram.h>
#include <esp_system.h>
#include <esp_timer.h>

#include <driver/gpio.h>
#include <driver/sdmmc_host.h>
#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <esp_vfs_fat.h>
#include <hal/spi_types.h>
#include <sdmmc_cmd.h>

#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <freertos/task.h>

#include "esp_ads1256.h"

// GPIO
#define BUZZER_GPIO   GPIO_NUM_4
#define IGNITOR_GPIO  GPIO_NUM_5
#define SQUIB_GPIO    GPIO_NUM_7
#define MOSI          GPIO_NUM_11
#define MISO          GPIO_NUM_13
#define CLK           GPIO_NUM_12
#define SD_DAT0       GPIO_NUM_35
#define SD_DAT1       GPIO_NUM_39
#define SD_DAT2       GPIO_NUM_37
#define SD_DAT3       GPIO_NUM_9
#define SD_CLK        GPIO_NUM_36
#define SD_CMD        GPIO_NUM_38
#define LOADCELL_CS   GPIO_NUM_8
#define LOADCELL_DRDY GPIO_NUM_21
#define LOADCELL_SYNC GPIO_NUM_48
#define TRANS_CS      GPIO_NUM_42
#define TRANS_DRDY    GPIO_NUM_35
#define TRANS_SYNC    GPIO_NUM_47
#define MAX1_CS       GPIO_NUM_10
#define MAX2_CS       GPIO_NUM_17
#define MAX3_CS       GPIO_NUM_18
#define MAX3_DRDY     GPIO_NUM_6
#define LORA_CS       GPIO_NUM_14
#define LORA_DIO1     GPIO_NUM_11
#define LORA_BUSY     GPIO_NUM_41
#define LORA_RESET    GPIO_NUM_40

// ADS1256 CONFIG
#define ADS_DRDY_TIMEOUT_MS 1000

// SPI CONFIG
#define SPI_HOST SPI2_HOST
#define DMA_CHAN SPI_DMA_CH_AUTO

// LORA CONFIG
#define LORA_BAUDRATE 115200
#define LORA_RATE_MS

// SD - LITTLEFS CONFIG
#define SD_BUFFER_SIZE       4096
#define MAX_SD_FILES         5
#define SD_UNIT_SIZE         32 * 1024
#define SD_MOUNT             "/sdcard"
#define LITTLEFS_BUFFER_SIZE 512
#define MAX_LFS_FILES        32

// MEMORY CONFIG
#define MAX_SAMPLES 700
#define ADS_SAMPLES 7000

// STATUS FLAGS
#define ARMED     (1 << 0)
#define FULL_ACQ  (1 << 1)
#define PART_ACQ  (1 << 2)
#define END_TEST  (1 << 3)
#define SAVE_SD   (1 << 4)
#define SEND_LORA (1 << 5)

// DATA STRUCTURES - SENSORS
enum SENSOR_BIT {
    LOADCELL_BIT,
    TRANS_BIT,
    MAX1_BIT,
    MAX2_BIT,
    MAX3_BIT,
};

// definir nome melhor que max1, 2 e 3
typedef struct __attribute__((packed)) {
    uint32_t timestamp; // 4 Bytes
    int32_t  loadcell;  // 4 Bytes
    int32_t  trans;     // 4 Bytes
} ads_data_t;           // 12 Bytes * 1k SPS = 12KB/s -> 72KB in total (6 seconds)

typedef struct __attribute__((packed)) {
    uint32_t timestamp; // 4 Bytes
    int16_t  max1;      // 2 Bytes
    int16_t  max2;      // 2 Bytes
    int16_t  max3;      // 2 Bytes
} max_data_t;           // 10 Bytes

typedef struct __attribute__((packed)) {
    volatile uint32_t ads_sample; // 4 Bytes
    volatile uint32_t max_sample; // 4 Bytes
    volatile uint16_t status;     // 2 Bytes
} sys_data_t;                     // 10 Bytes

// DATA MANAGEMENT
extern ads_data_t *ads_data_g;
extern max_data_t *max_data_g;
extern sys_data_t  sys_data_g;

// MUTEXES
// extern SemaphoreHandle_t xDATAMutex;
extern portMUX_TYPE xDATASpinlock;

// Event group for NVS counter synchronization
extern EventGroupHandle_t xNVSCounterEvent;
// Event group for LittleFS and SD format synchronization
extern EventGroupHandle_t xFormatEvent;

// TASK HANDLE
extern TaskHandle_t xTaskAds;

// TASKS
void task_ads(void *pvParameters);
void task_max(void *pvParameters);
void task_sd(void *pvParameters);
void task_littlefs(void *pvParameters);
void task_lora(void *pvParameters);
void task_log(void *pvParameters);

#endif