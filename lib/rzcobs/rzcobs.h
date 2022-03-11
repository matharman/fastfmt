#ifndef RZCOBS_H
#define RZCOBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef int (*rzcobs_byte_sink)(void *context, const void *buf, size_t len);

typedef struct {
    rzcobs_byte_sink sink;
    void *sink_context;
    uint8_t run;
    uint8_t zeros;
    uint8_t started;
} RZCobWriter;

void rzcob_writer_init(RZCobWriter *w, rzcobs_byte_sink sink, void *sink_context);
int rzcob_start_frame(RZCobWriter *w);
int rzcob_end_frame(RZCobWriter *w);
int rzcob_write(RZCobWriter *w, const void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // RZCOBS_H
