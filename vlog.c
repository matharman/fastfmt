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

int my_logf(enum log_level level, uint32_t fmt_offset, int nargs, ...) {
    return 0;
}

int defmt_printf_like(const char *format, ...) {
    return 0;
}
