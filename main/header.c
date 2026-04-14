#include "header.h"

spi_device_handle_t spi_bus_handle = NULL;

// DATA MANAGEMENT
data_t    *data_g     = NULL;
sys_temp_t sys_temp_g = {0};

// MUTEXES
SemaphoreHandle_t xDATAMutex = NULL;
SemaphoreHandle_t xADSMutex  = NULL;
SemaphoreHandle_t xMAXMutex  = NULL;

// EVENT HANDLE
EventGroupHandle_t xNVSCounterEvent = NULL;
EventGroupHandle_t xFormatEvent     = NULL;
