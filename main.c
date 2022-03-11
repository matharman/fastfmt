#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "defmt.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

void hexdump(const char *tag, const uint8_t *data, size_t len) {
    printf("%s----------\n", tag);
    for (size_t i = 0; i < len; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t log_out[2048];
    LOG_ERR("Error level: %d", LOG_LEVEL_ERR);
    size_t len = my_vlog_flush_buffer(log_out, sizeof(log_out));
    hexdump("error", log_out, len);

    LOG_WRN("Warning level: %d", LOG_LEVEL_WRN);
    len = my_vlog_flush_buffer(log_out, sizeof(log_out));
    hexdump("warning", log_out, len);

    LOG_INF("Info level: %d", LOG_LEVEL_INF);
    len = my_vlog_flush_buffer(log_out, sizeof(log_out));
    hexdump("info", log_out, len);

    LOG_DBG("Debug level: %d", LOG_LEVEL_DBG);
    len = my_vlog_flush_buffer(log_out, sizeof(log_out));
    hexdump("debug", log_out, len);

    LOG_TRC("Trace level: %d", LOG_LEVEL_TRC);
    len = my_vlog_flush_buffer(log_out, sizeof(log_out));
    hexdump("trace", log_out, len);

    LOG_INF("Info float: %f", 0.3f);
    len = my_vlog_flush_buffer(log_out, sizeof(log_out));
    hexdump("info float", log_out, len);

    return 0;
}
