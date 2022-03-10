#ifndef PREPROC_H
#define PREPROC_H

#ifdef __cplusplus
extern "C" {
#endif

#define PP_NARG(...)  PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)

#define PP_RSEQ_N() 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N

#define PP_MAP_0(map) 0

#ifdef __cplusplus
}
#endif

#endif  // PREPROC_H
