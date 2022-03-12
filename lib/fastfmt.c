#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "fastfmt.h"
#include "rzcobs.h"

static const struct fastfmt_header {
    uint32_t magic;
    uint32_t version;
} m_header FASTFMT_IN_SECTION(".fastfmt.header") __attribute__((used)) = {
    .magic = 0x4D324D44,
    .version = 0x000001,
};

uint32_t fastfmt_string_offset(const char *str) {
    extern char __start_fastfmt;
    return str - &__start_fastfmt;
}

int fastfmt_printf_like(const char *format, ...) {
    (void)format;
    return 0;
}

void fastfmt_emit_log_level(char lvl) {
    fastfmt_emit_bytes(&lvl, sizeof(lvl));
}

void fastfmt_emit_bool(_Bool arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_float(float arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_double(double arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_string(const char *arg) {
    // TODO prefix for string
    fastfmt_emit_bytes(arg, strlen(arg));
}

void fastfmt_emit_long(long arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_int8_t(int8_t arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_int16_t(uint16_t arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_int32_t(int32_t arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_int64_t(int64_t arg) {
    fastfmt_emit_bytes(&arg, sizeof(arg));
}

void fastfmt_emit_ptr(const void *arg) {
    uintptr_t data = (uintptr_t)arg;
    fastfmt_emit_bytes(&data, sizeof(data));
}

static RZCobWriter writer;

int fastfmt_start_frame(void) {
    return rzcob_start_frame(&writer);
}

int fastfmt_end_frame(void) {
    return rzcob_end_frame(&writer);
}

int fastfmt_emit_bytes(const void *data, size_t len) {
    return rzcob_write(&writer, data, len);
}

void fastfmt_init_sink(fastfmt_bytesink callback, void *context) {
    rzcob_writer_init(&writer, callback, context);
}

_Static_assert(FASTFMT_PP_NARG() == 0, "PP_NARG match fail");
_Static_assert(FASTFMT_PP_NARG(1) == 1, "PP_NARG match fail");
_Static_assert(FASTFMT_PP_NARG(1, 2, 3, 4, 5) == 5, "PP_NARG match fail");
