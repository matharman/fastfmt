#include "rzcobs.h"

void rzcob_writer_init(RZCobWriter *w, rzcobs_byte_sink sink, void *sink_context) {
    w->sink = sink;
    w->sink_context = sink_context;
    w->run = 0;
    w->zeros = 0;
    w->started = 0;
}

static inline int rzcob_buffer_put(RZCobWriter *w, uint8_t b);
static int rzcob_write_byte(RZCobWriter *w, uint8_t byte);

int rzcob_start_frame(RZCobWriter *w) {
    if (!w->started) {
        w->started = 1;
        rzcob_write_byte(w, 0x00);
    }

    return 0;
}

int rzcob_end_frame(RZCobWriter *w) {
    if (w->run == 0) {
        // Do nothing
    } else if (w->run >= 1 && w->run <= 6) {
        int err = rzcob_buffer_put(w, (w->zeros | (0xFF << w->run)) & 0x7F);
        if (err != 0) {
            return err;
        }
    } else {
        int err = rzcob_buffer_put(w, (w->run - 7) | 0x80);
        if (err != 0) {
            return err;
        }
    }

    rzcob_buffer_put(w, 0x00);

    w->run = 0;
    w->zeros = 0;
    return 0;
}

int rzcob_write(RZCobWriter *w, const void *buf, size_t len) {
    const uint8_t *data = buf;
    for (size_t i = 0; i < len; i++) {
        int err = rzcob_write_byte(w, data[i]);
        if (err != 0) {
            return err;
        }
    }

    return 0;
}

static inline int rzcob_buffer_put(RZCobWriter *w, uint8_t b) {
    if (w->sink) {
        return w->sink(w->sink_context, &b, 1);
    }

    return -1;
}

static int rzcob_write_byte(RZCobWriter *w, uint8_t byte) {
    if (w->run < 7) {
        if (byte == 0) {
            w->zeros |= 1 << w->run;
        } else {
            int err = rzcob_buffer_put(w, byte);
            if (err != 0) {
                return err;
            }
        }

        w->run += 1;
        if (w->run == 7 && w->zeros != 0x00) {
            int err = rzcob_buffer_put(w, w->zeros);
            if (err != 0) {
                return err;
            }
            w->run = 0;
            w->zeros = 0;
        }
    } else if (byte == 0) {
        int err = rzcob_buffer_put(w, (w->run - 7) | 0x80);
        if (err != 0) {
            return err;
        }
        w->run = 0;
        w->zeros = 0;
    } else {
        int err = rzcob_buffer_put(w, byte);
        if (err != 0) {
            return err;
        }
        w->run += 1;
        if (w->run == 134) {
            int err = rzcob_buffer_put(w, 0xFF);
            if (err != 0) {
                return err;
            }
            w->run = 0;
            w->zeros = 0;
        }
    }

    return 0;
}
