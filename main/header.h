#ifndef HEADER_H
#define HEADER_H

// INCLUDES
#include <math.h>
#include <stdatomic.h>
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
#include <esp_system.h>
#include <esp_timer.h>

#include <driver/gpio.h>
#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <hal/spi_types.h>

#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#include "esp_ads1256.h"

// GPIO
#define BUZZER_GPIO
#define LED_GPIO
#define IGNITOR_GPIO
#define SQUIB_GPIO
#define MOSI GPIO_NUM_11
#define MISO GPIO_NUM_13
#define CLK  GPIO_NUM_12
#define SD_DAT0
#define SD_DAT1
#define SD_DAT2
#define SD_DAT3
#define SD_CLK
#define SD_CMD
#define LOADCELL_CS   GPIO_NUM_39
#define LOADCELL_DRDY GPIO_NUM_21
#define TRANS_CS      GPIO_NUM_42
#define TRANS_DRDY    GPIO_NUM_35
#define MAX1_CS
#define MAX2_CS
#define MAX3_CS
#define MAX3_DRDY
#define LORA_CS
#define LORA_DI0
#define LORA_BUSY
#define LORA_RESET

// ADS1256 CONFIG
#define ADS_DRDY_TIMEOUT_MS 1
#define ADS_SAMPLE_RATE_MS  1

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

// STATUS FLAGS
// ...

// DATA STRUCTURES - SENSORS
enum SENSOR_BIT {
    LOADCELL_BIT,
    TRANS_BIT,
    MAX1_BIT,
    MAX2_BIT,
    MAX3_BIT,
};

typedef struct {
    int32_t raw;
} ads1256_sample_t;

typedef struct {
    int16_t temperature;
} max_sample_t;

// definir nome melhor que ads1, 2 e max1, 2 e 3
typedef struct {
    uint32_t timestamp;

    ads1256_sample_t loadcell;
    ads1256_sample_t trans;

    max_sample_t max1;
    max_sample_t max2;
    max_sample_t max3;
} data_t;

// DATA STRUCTURES - STORAGE
typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    int32_t  thrust;
    int32_t  pressure;

    // substituir por locais dos max
    int16_t temperature1;
    int16_t temperature2;
    int16_t temperature3;
} save_t;

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    int32_t  thrust;
    int32_t  pressure;

    // substituir por locais dos max
    int16_t temperature1;
    int16_t temperature2;
    int16_t temperature3;
} send_t;

// separar em vários sample_g ??
extern data_t           data_g;
extern ads1256_sample_t ads1256_sample_g[2];
extern max_sample_t     max6675_sample_g[2];
extern max_sample_t     max31865_sample_g;

// TASK HANDLES
extern TaskHandle_t xTaskLora;
extern TaskHandle_t xTaskAcquire;

// RINGBUFFER
extern RingbufHandle_t xSDBuffer;
extern RingbufHandle_t xLittleFSBuffer;

// MUTEXES
extern portMUX_TYPE xDATAMutex;
extern portMUX_TYPE xADS1Mutex;
extern portMUX_TYPE xADS2Mutex;
extern portMUX_TYPE xMAX1Mutex;
extern portMUX_TYPE xMAX2Mutex;
extern portMUX_TYPE xMAX3Mutex;

// Event group for NVS counter synchronization
extern EventGroupHandle_t xNVSCounterEvent;
// Event group for LittleFS and SD format synchronization
extern EventGroupHandle_t xFormatEvent;

// TASKS
void ads_task(void *pvParameters);
void max_task(void *pvParameters);
void task_acquire(void *pvParameters);
void task_sd(void *pvParameters);
void task_littlefs(void *pvParameters);
void task_lora(void *pvParameters);
void task_log(void *pvParameters);

#endif