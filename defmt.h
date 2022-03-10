#ifndef DEFMT_H
#define DEFMT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "preproc.h"

#define IN_SECTION(_section) __attribute__((section(_section)))
#define STRINGIFY(_str)      STRINGIFY_(_str)
#define STRINGIFY_(_str)     #_str

#define BUILD_FORMAT_STRING(_name, _lvl, _fmt, ...) \
    IN_SECTION(".defmt.strings")                    \
    static const char _name[] =                     \
        STRINGIFY(_lvl) ";" __FILE__                \
                        ";" STRINGIFY(__LINE__) ";" STRINGIFY(PP_NARG(__VA_ARGS__)) ";" _fmt "\n"

// For compile-time checks only
int defmt_printf_like(const char *format, ...) __attribute__((format(printf, 1, 2)));

enum log_level {
    LOG_LEVEL_ERR = 0,
    LOG_LEVEL_WRN = 1,
    LOG_LEVEL_INF = 2,
    LOG_LEVEL_DBG = 3,
    LOG_LEVEL_TRC = 4,
};

#define LOG_MODULE_REGISTER(_module, _lvl) static const enum log_level _private_log_filter = _lvl
#define LOG_MODULE_DECLARE(_module, _lvl)  LOG_MODULE_REGISTER(_module, _lvl)

#define LOG_SELECT_OUTPUT_FN(arg) \
    _Generic((arg), \
            char *: my_vlog_emit_string, \
            const char *: my_vlog_emit_string, \
            float: my_vlog_emit_float, \
            double: my_vlog_emit_double, \
            int64_t: my_vlog_emit_int64_t, \
            default: my_vlog_emit_int32_t)

#define LOG_OUTPUT_ARG(arg) LOG_SELECT_OUTPUT_FN(arg)(arg)

uint32_t my_logf_string_offset(const char *str);
void my_vlog_emit_float(float arg);
void my_vlog_emit_double(double arg);
void my_vlog_emit_string(const char *arg);
void my_vlog_emit_int32_t(int32_t arg);
void my_vlog_emit_int64_t(int64_t arg);

#define LOG_PRIVATE(_lvl, _fmt, ...)                                                 \
    do {                                                                             \
        BUILD_FORMAT_STRING(interned_format_string, _lvl, _fmt, ##__VA_ARGS__);      \
        defmt_printf_like(interned_format_string, ##__VA_ARGS__);                    \
        if (_lvl <= _private_log_filter) {                                           \
            uint32_t offset = my_logf_string_offset(&interned_format_string[0]);     \
            printf("[%c] offset %u nargs %d\n", _lvl, offset, PP_NARG(__VA_ARGS__)); \
            PP_FOR_EACH(LOG_OUTPUT_ARG, ##__VA_ARGS__);                              \
        }                                                                            \
    } while (0);

#define LOG_ERR(_fmt, ...) LOG_PRIVATE(0, _fmt, ##__VA_ARGS__)
#define LOG_WRN(_fmt, ...) LOG_PRIVATE(1, _fmt, ##__VA_ARGS__)
#define LOG_INF(_fmt, ...) LOG_PRIVATE(2, _fmt, ##__VA_ARGS__)
#define LOG_DBG(_fmt, ...) LOG_PRIVATE(3, _fmt, ##__VA_ARGS__)
#define LOG_TRC(_fmt, ...) LOG_PRIVATE(4, _fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // DEFMT_H
