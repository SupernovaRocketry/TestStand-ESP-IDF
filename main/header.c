#include "header.h"

spi_device_handle_t spi_bus_handle = NULL;

// TASKS
TaskHandle_t xTaskAcquire = NULL;
TaskHandle_t xTaskLora    = NULL;

// RINGBUFFER
RingbufHandle_t xSDBuffer;
RingbufHandle_t xLittleFSBuffer;

// MUTEXES
portMUX_TYPE xDATAMutex = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE xADS1Mutex = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE xADS2Mutex = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE xMAX1Mutex = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE xMAX2Mutex = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE xMAX3Mutex = portMUX_INITIALIZER_UNLOCKED;

// EVENT HANDLE
EventGroupHandle_t xNVSCounterEvent = NULL;
EventGroupHandle_t xFormatEvent     = NULL;

// DATA SETS
data_t           data_g              = {0};
ads1256_sample_t ads1256_sample_g[2] = {0};
max_sample_t     max6675_sample_g[2] = {0};
max_sample_t     max31865_sample_g   = {0};
