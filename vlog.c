#include <stdio.h>
#include <stdint.h>

#include "defmt.h"

static const struct my_vlog_header {
    uint32_t magic;
    uint32_t version;
} m_header IN_SECTION(".defmt.header") = {
    .magic = 0x4D324D44,
    .version = 0x000001,
};

uint32_t my_logf_string_offset(const char *str) {
    extern char __start_defmt;
    return str - &__start_defmt;
}

int defmt_printf_like(const char *format, ...) {
    return 0;
}

void my_vlog_emit_float(float arg) {
    printf("emit float: %f\n", arg);
}

void my_vlog_emit_double(double arg) {
    printf("emit double: %lf\n", arg);
}

void my_vlog_emit_string(const char *arg) {
    printf("emit string: %s\n", arg ? arg : "nil");
}

void my_vlog_emit_int32_t(int32_t arg) {
    printf("emit int32: %d\n", arg);
}

void my_vlog_emit_int64_t(int64_t arg) {
    printf("emit int64: %ld\n", arg);
}
