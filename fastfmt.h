#ifndef FASTFMT_H
#define FASTFMT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include "fastfmt_preproc.h"

#define IN_SECTION(_section) __attribute__((section(_section)))
#define STRINGIFY(_str)      STRINGIFY_(_str)
#define STRINGIFY_(_str)     #_str

// clang-format off
#define BUILD_FORMAT_STRING(_name, _fmt, ...) \
IN_SECTION(".fastfmt.strings") static const char _name[] = \
       "{\"filename\":\"" __FILE__ "\"," \
       "\"lineNumber\":" STRINGIFY(__LINE__) "," \
       "\"argCount\":" STRINGIFY(PP_NARG(__VA_ARGS__)) "," \
       "\"formatString\":\"" _fmt "\"" \
       "}"
// clang-format on

// For compile-time checks only
int fastfmt_printf_like(const char *format, ...) __attribute__((format(printf, 1, 2)));

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERR 1
#define LOG_LEVEL_WRN 2
#define LOG_LEVEL_INF 3
#define LOG_LEVEL_DBG 4
#define LOG_LEVEL_TRC 5

#define LOG_MODULE_DECLARE(_module, _lvl)  LOG_MODULE_REGISTER(_module, _lvl)
#define LOG_MODULE_REGISTER(_module, _lvl) static const uint8_t __log_filter = _lvl

#define LOG_SELECT_OUTPUT_FN(arg) \
    _Generic((arg), \
            char *: fastfmt_emit_string, \
            const char *: fastfmt_emit_string, \
            float: fastfmt_emit_float, \
            double: fastfmt_emit_double, \
            int64_t: fastfmt_emit_int64_t, \
            default: fastfmt_emit_int32_t)

#define LOG_OUTPUT_ARG(arg) LOG_SELECT_OUTPUT_FN(arg)(arg)

typedef int (*fastfmt_bytesink)(void *context, const void *bytes, size_t len);
void fastfmt_init_sink(fastfmt_bytesink callback, void *context);
uint32_t fastfmt_string_offset(const char *str);
int fastfmt_start_frame(void);
int fastfmt_end_frame(void);
int fastfmt_emit_bytes(const void *data, size_t len);

void fastfmt_emit_log_level(char lvl);
void fastfmt_emit_float(float arg);
void fastfmt_emit_double(double arg);
void fastfmt_emit_string(const char *arg);
void fastfmt_emit_int32_t(int32_t arg);
void fastfmt_emit_int64_t(int64_t arg);

#define LOG_LEVEL_TO_CHAR(_lvl)                    \
    ({                                             \
        typeof((_lvl)) __ll2ch_var = (_lvl);       \
        char __ll2ch_char = '$';                   \
        if (__ll2ch_var == LOG_LEVEL_ERR) {        \
            __ll2ch_char = 'E';                    \
        } else if (__ll2ch_var == LOG_LEVEL_WRN) { \
            __ll2ch_char = 'W';                    \
        } else if (__ll2ch_var == LOG_LEVEL_INF) { \
            __ll2ch_char = 'I';                    \
        } else if (__ll2ch_var == LOG_LEVEL_DBG) { \
            __ll2ch_char = 'D';                    \
        } else if (__ll2ch_var == LOG_LEVEL_TRC) { \
            __ll2ch_char = 'T';                    \
        }                                          \
        __ll2ch_char;                              \
    })

// Always generate the format string for the metadata section -- no runtime overhead.
// Don't generate code for the log emission if it can be avoided.
#define _LOG_PRIVATE(_lvl, _fmt, ...)                                              \
    do {                                                                           \
        BUILD_FORMAT_STRING(interned_format_string, _fmt, ##__VA_ARGS__);          \
        fastfmt_printf_like(interned_format_string, ##__VA_ARGS__);                \
        if (_lvl <= __log_filter) {                                                \
            fastfmt_start_frame();                                                 \
            fastfmt_emit_log_level(LOG_LEVEL_TO_CHAR(_lvl));                       \
            uint32_t offset = fastfmt_string_offset(&interned_format_string[0]);   \
            printf("[%c] offset 0x%X nargs %d\n", LOG_LEVEL_TO_CHAR(_lvl), offset, \
                   PP_NARG(__VA_ARGS__));                                          \
            fastfmt_emit_int32_t(offset);                                          \
            PP_FOR_EACH(LOG_OUTPUT_ARG, ##__VA_ARGS__);                            \
            fastfmt_end_frame();                                                   \
        }                                                                          \
    } while (0)

#define LOG_ERR(_fmt, ...) _LOG_PRIVATE(LOG_LEVEL_ERR, _fmt, ##__VA_ARGS__)
#define LOG_WRN(_fmt, ...) _LOG_PRIVATE(LOG_LEVEL_WRN, _fmt, ##__VA_ARGS__)
#define LOG_INF(_fmt, ...) _LOG_PRIVATE(LOG_LEVEL_INF, _fmt, ##__VA_ARGS__)
#define LOG_DBG(_fmt, ...) _LOG_PRIVATE(LOG_LEVEL_DBG, _fmt, ##__VA_ARGS__)
#define LOG_TRC(_fmt, ...) _LOG_PRIVATE(LOG_LEVEL_TRC, _fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // FASTFMT_H
