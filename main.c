#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "defmt.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void) {
    LOG_ERR("Error level: %d", LOG_LEVEL_ERR);
    LOG_WRN("Warning level: %d", LOG_LEVEL_WRN);
    LOG_INF("Info level: %d", LOG_LEVEL_INF);
    LOG_DBG("Debug level: %d", LOG_LEVEL_DBG);
    LOG_TRC("Trace level: %d", LOG_LEVEL_TRC);
    return 0;
}
