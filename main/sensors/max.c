#include "global.h"

static const char *TAG_MAX = "MAX";

void task_max(void *pvParameters) {
    // MAX INIT
    // ...

    // LOGICA DE COLETA DOS MAX
    while (true) {

        if ((sys_data_g.status & FULL_ACQ) || (sys_data_g.status & PART_ACQ)) {
            // adiciona max_read_result();
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}