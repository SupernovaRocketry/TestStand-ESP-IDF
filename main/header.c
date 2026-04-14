#include "header.h"

spi_device_handle_t spi_bus_handle = NULL;

// DATA MANAGEMENT
data_t           *data_g         = NULL;
volatile uint32_t current_sample = 0;
volatile int16_t  current_temp1  = 0;
volatile int16_t  current_temp2  = 0;
volatile int16_t  current_temp3  = 0;

// MUTEXES
SemaphoreHandle_t xDATAMutex = NULL;
SemaphoreHandle_t xADSMutex  = NULL;
SemaphoreHandle_t xMAXMutex  = NULL;

// EVENT HANDLE
EventGroupHandle_t xNVSCounterEvent = NULL;
EventGroupHandle_t xFormatEvent     = NULL;
