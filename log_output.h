#ifndef LOG_OUTPUT_H
#define LOG_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

int my_vlog_start_frame(void);
int my_vlog_end_frame(void);
int my_vlog_emit_bytes(const void *data, size_t len);

size_t my_vlog_flush_buffer(void *dest, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // LOG_OUTPUT_H
