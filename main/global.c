#include "global.h"

// DATA MANAGEMENT
ads_data_t *ads_data_g = NULL;
max_data_t *max_data_g = NULL;
sys_data_t  sys_data_g = {0};

// MUTEXES
// SemaphoreHandle_t xDATAMutex = NULL;
portMUX_TYPE xDATASpinlock = portMUX_INITIALIZER_UNLOCKED;

// EVENT HANDLE
EventGroupHandle_t xNVSCounterEvent = NULL;
EventGroupHandle_t xFormatEvent     = NULL;

// TASK HANDLE
TaskHandle_t xTaskAds = NULL;
