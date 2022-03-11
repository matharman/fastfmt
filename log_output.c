#include <stdint.h>
#include <string.h>

#include "rzcobs.h"

static uint8_t log_buffer[2048];
static uint8_t *write_ptr;
static RZCobWriter writer;

static int vlog_byte_sink(void *context, const void *buf, size_t len) {
    memcpy(write_ptr, buf, len);
    write_ptr += len;
    return 0;
}

__attribute__((constructor)) static void my_vlog_init(void) {
    write_ptr = log_buffer;
    rzcob_writer_init(&writer, vlog_byte_sink, NULL);
}

int my_vlog_start_frame(void) {
    return rzcob_start_frame(&writer);
}

int my_vlog_end_frame(void) {
    return rzcob_end_frame(&writer);
}

int my_vlog_emit_bytes(const void *data, size_t len) {
    return rzcob_write(&writer, data, len);
}

size_t my_vlog_flush_buffer(void *dest, size_t len) {
    size_t used = write_ptr - log_buffer;
    size_t copy_len = len >= used ? used : len;
    memcpy(dest, log_buffer, copy_len);
    write_ptr = log_buffer;
    return copy_len;
}
