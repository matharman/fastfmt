#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "fastfmt.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static size_t fastfmt_flush_buffer(void *dest, size_t len);
static void hexdump(const char *tag, const uint8_t *data, size_t len);

int main(void) {
    uint8_t log_out[2048];
    LOG_ERR("Error level: %d", LOG_LEVEL_ERR);
    size_t len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("error", log_out, len);

    LOG_WRN("Warning level: %d", LOG_LEVEL_WRN);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("warning", log_out, len);

    LOG_INF("Info level: %d", LOG_LEVEL_INF);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("info", log_out, len);

    LOG_DBG("Debug level: %d", LOG_LEVEL_DBG);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("debug", log_out, len);

    LOG_TRC("Trace level: %d", LOG_LEVEL_TRC);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("trace", log_out, len);

    LOG_INF("Info float: %f", 0.3f);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("info float", log_out, len);

    return 0;
}

static void hexdump(const char *tag, const uint8_t *data, size_t len) {
    printf("%s----------\n", tag);
    for (size_t i = 0; i < len; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
}

static uint8_t log_buffer[2048];
static uint8_t *write_ptr = log_buffer;

static size_t fastfmt_flush_buffer(void *dest, size_t len) {
    size_t used = write_ptr - log_buffer;
    size_t copy_len = len >= used ? used : len;
    memcpy(dest, log_buffer, copy_len);
    write_ptr = log_buffer;
    return copy_len;
}

static int bytesink(void *context, const void *buf, size_t len) {
    memcpy(write_ptr, buf, len);
    write_ptr += len;
    return 0;
}

__attribute__((constructor)) static void init_fastfmt(void) {
    fastfmt_init_sink(bytesink, NULL);
}
