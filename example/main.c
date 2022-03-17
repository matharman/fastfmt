#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "fastfmt.h"
FASTFMT_LOG_LEVEL_SET(FF_LOG_LEVEL_INF);

static size_t fastfmt_flush_buffer(void *dest, size_t len);
static void hexdump(const char *tag, const uint8_t *data, size_t len);

int main(void) {
    uint8_t log_out[2048];

    FASTFMT_LOG_INF("Info no args");
    size_t len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("info no args", log_out, len);

    FASTFMT_LOG_ERR("Error level: %d", FF_LOG_LEVEL_ERR);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("error", log_out, len);

    FASTFMT_LOG_WRN("Warning level: %d", FF_LOG_LEVEL_WRN);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("warning", log_out, len);

    FASTFMT_LOG_INF("Info level: %d", FF_LOG_LEVEL_INF);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("info", log_out, len);

    FASTFMT_LOG_DBG("Debug level: %d", FF_LOG_LEVEL_DBG);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("debug", log_out, len);

    FASTFMT_LOG_TRC("Trace level: %d", FF_LOG_LEVEL_TRC);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("trace", log_out, len);

    FASTFMT_LOG_INF("Info float: %f", 0.3f);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("info float", log_out, len);

    char *world = (char *)"world";
    FASTFMT_LOG_INF("Info many args %s %s %f %lf %lu %p", world, "hello", 0.3f, 0.6, UINT64_MAX,
                    &world);
    len = fastfmt_flush_buffer(log_out, sizeof(log_out));
    hexdump("info many args", log_out, len);

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
