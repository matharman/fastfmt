#ifndef FASTFMT_H
#define FASTFMT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "fastfmt_preproc.h"

#define FASTFMT_IN_SECTION(_section) __attribute__((section(_section)))
#define FASTFMT_STRINGIFY(_str)      FASTFMT_STRINGIFY_(_str)
#define FASTFMT_STRINGIFY_(_str)     #_str

#define FASTFMT_UNIQUE_SECTION ".fastfmt.strings." __FILE__ "." FASTFMT_STRINGIFY(__LINE__)

#if 0
// clang-format off
#define FASTFMT_BUILD_FORMAT_STRING(_name, _fmt, ...) \
       static const char _name[] FASTFMT_IN_SECTION(FASTFMT_UNIQUE_SECTION) __attribute__((unused)) = \
       "{\"filename\":\"" __FILE__ "\"," \
       "\"lineNumber\":" FASTFMT_STRINGIFY(__LINE__) "," \
       "\"argCount\":" FASTFMT_STRINGIFY(FASTFMT_PP_NARG(__VA_ARGS__)) "," \
       "\"formatString\":\"" _fmt "\"" \
       "}"
// clang-format on
#else
    #define FASTFMT_BUILD_FORMAT_STRING(_name, _fmt, _nargs)                      \
        static const char _name[] FASTFMT_IN_SECTION(FASTFMT_UNIQUE_SECTION)      \
            __attribute__((unused)) =                                             \
                "##FASTFMT##" __FILE__                                            \
                ";" FASTFMT_STRINGIFY(__LINE__) ";" FASTFMT_STRINGIFY(_nargs) ";" \
                                                                              "\"" _fmt "\""
#endif

// For compile-time checks only
static inline int fastfmt_printf_like(const char *format, ...)
    __attribute__((format(printf, 1, 2)));

static inline int fastfmt_printf_like(const char *format, ...) {
    return 0;
}

#define FF_LOG_LEVEL_OFF 0
#define FF_LOG_LEVEL_ERR 1
#define FF_LOG_LEVEL_WRN 2
#define FF_LOG_LEVEL_INF 3
#define FF_LOG_LEVEL_DBG 4
#define FF_LOG_LEVEL_TRC 5

#define FASTFMT_LOG_LEVEL_SET(_lvl) static const char __log_filter __attribute__((unused)) = _lvl

#define FASTFMT_LOG_SELECT_OUTPUT_FN(arg) \
    _Generic((arg) + 0, \
            _Bool: fastfmt_emit_bool, \
            char *: fastfmt_emit_string, \
            const char *: fastfmt_emit_string, \
            float: fastfmt_emit_float, \
            double: fastfmt_emit_double, \
            char: fastfmt_emit_int8_t, \
            unsigned char: fastfmt_emit_int8_t, \
            short: fastfmt_emit_int16_t, \
            unsigned short: fastfmt_emit_int16_t, \
            int: fastfmt_emit_int32_t, \
            unsigned int: fastfmt_emit_int32_t, \
            long: fastfmt_emit_int64_t, \
            unsigned long: fastfmt_emit_int64_t, \
            long long: fastfmt_emit_int64_t, \
            unsigned long long: fastfmt_emit_int64_t, \
            void *: fastfmt_emit_ptr, \
	    default: fastfmt_emit_ptr)

#define FASTFMT_LOG_OUTPUT_ARG(arg) FASTFMT_LOG_SELECT_OUTPUT_FN(arg)(arg)

typedef int (*fastfmt_bytesink)(void *context, const void *bytes, size_t len);
void fastfmt_init_sink(fastfmt_bytesink callback, void *context);
uint32_t fastfmt_string_offset(const char *str);
int fastfmt_start_frame(void);
int fastfmt_end_frame(void);
int fastfmt_emit_bytes(const void *data, size_t len);

void fastfmt_emit_log_level(char lvl);
void fastfmt_emit_bool(_Bool arg);
void fastfmt_emit_float(float arg);
void fastfmt_emit_double(double arg);
void fastfmt_emit_string(const char *arg);
void fastfmt_emit_long(long arg);
void fastfmt_emit_int8_t(int8_t arg);
void fastfmt_emit_int16_t(uint16_t arg);
void fastfmt_emit_int32_t(int32_t arg);
void fastfmt_emit_int64_t(int64_t arg);
void fastfmt_emit_ptr(const void *arg);

#define FASTFMT_LOG_LEVEL_TO_CHAR(_lvl)               \
    ({                                                \
        typeof((_lvl)) __ll2ch_var = (_lvl);          \
        char __ll2ch_char = '$';                      \
        if (__ll2ch_var == FF_LOG_LEVEL_ERR) {        \
            __ll2ch_char = 'E';                       \
        } else if (__ll2ch_var == FF_LOG_LEVEL_WRN) { \
            __ll2ch_char = 'W';                       \
        } else if (__ll2ch_var == FF_LOG_LEVEL_INF) { \
            __ll2ch_char = 'I';                       \
        } else if (__ll2ch_var == FF_LOG_LEVEL_DBG) { \
            __ll2ch_char = 'D';                       \
        } else if (__ll2ch_var == FF_LOG_LEVEL_TRC) { \
            __ll2ch_char = 'T';                       \
        }                                             \
        __ll2ch_char;                                 \
    })

#define _FASTFMT_LOG_EMIT_ARGS(_lvl, _fmt, ...)                                        \
    do {                                                                               \
        FASTFMT_BUILD_FORMAT_STRING(interned_fmt, _fmt, FASTFMT_PP_NARG(__VA_ARGS__)); \
        (void)fastfmt_printf_like(_fmt, ##__VA_ARGS__);                                \
        if (_lvl <= __log_filter) {                                                    \
            fastfmt_start_frame();                                                     \
            fastfmt_emit_log_level(FASTFMT_LOG_LEVEL_TO_CHAR(_lvl));                   \
            fastfmt_emit_int32_t(fastfmt_string_offset(interned_fmt));                 \
            FASTFMT_PP_FOR_EACH(FASTFMT_LOG_OUTPUT_ARG, ##__VA_ARGS__);                \
            fastfmt_end_frame();                                                       \
        }                                                                              \
    } while (0)

#define _FASTFMT_LOG_EMIT_0(_lvl, _fmt, ...)                                           \
    do {                                                                               \
        FASTFMT_BUILD_FORMAT_STRING(interned_fmt, _fmt, FASTFMT_PP_NARG(__VA_ARGS__)); \
        (void)fastfmt_printf_like(_fmt, ##__VA_ARGS__);                                \
        if (_lvl <= __log_filter) {                                                    \
            fastfmt_start_frame();                                                     \
            fastfmt_emit_log_level(FASTFMT_LOG_LEVEL_TO_CHAR(_lvl));                   \
            fastfmt_emit_int32_t(fastfmt_string_offset(interned_fmt));                 \
            fastfmt_end_frame();                                                       \
        }                                                                              \
    } while (0)

#define _FASTFMT_LOG_NARGS_POSTFIX_IMPL(_ignored, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                                        _11, _12, _13, _14, N, ...)                            \
    N

#define _FASTFMT_LOG_NARGS_POSTFIX(...)                                                          \
    _FASTFMT_LOG_NARGS_POSTFIX_IMPL(__VA_ARGS__, ARGS, ARGS, ARGS, ARGS, ARGS, ARGS, ARGS, ARGS, \
                                    ARGS, ARGS, ARGS, ARGS, ARGS, ARGS, ARGS, 0, ~)

#define _FASTFMT_LOG_INTERNAL_X(N, ...) FASTFMT_CONCAT(_FASTFMT_LOG_EMIT_, N)(__VA_ARGS__)

#define _FASTFMT_LOG_PRIVATE(_lvl, ...)                                                      \
    do {                                                                                     \
        _FASTFMT_LOG_INTERNAL_X(_FASTFMT_LOG_NARGS_POSTFIX(__VA_ARGS__), _lvl, __VA_ARGS__); \
    } while (0)

#define FASTFMT_LOG_ERR(...) _FASTFMT_LOG_PRIVATE(FF_LOG_LEVEL_ERR, __VA_ARGS__)
#define FASTFMT_LOG_WRN(...) _FASTFMT_LOG_PRIVATE(FF_LOG_LEVEL_WRN, __VA_ARGS__)
#define FASTFMT_LOG_INF(...) _FASTFMT_LOG_PRIVATE(FF_LOG_LEVEL_INF, __VA_ARGS__)
#define FASTFMT_LOG_DBG(...) _FASTFMT_LOG_PRIVATE(FF_LOG_LEVEL_DBG, __VA_ARGS__)
#define FASTFMT_LOG_TRC(...) _FASTFMT_LOG_PRIVATE(FF_LOG_LEVEL_TRC, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // FASTFMT_H
