#ifndef PREPROC_H
#define PREPROC_H

#ifdef __cplusplus
extern "C" {
#endif

#define FASTFMT_PP_NARG(...)  FASTFMT_PP_NARG_(__VA_ARGS__, FASTFMT_PP_RSEQ_N())
#define FASTFMT_PP_NARG_(...) FASTFMT_PP_ARG_N(__VA_ARGS__)

#define FASTFMT_PP_RSEQ_N() 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FASTFMT_PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N

#define FASTFMT_PP_MAP_0(_map)
#define FASTFMT_PP_MAP_1(_map, X)       _map(X)
#define FASTFMT_PP_MAP_2(_map, X, ...)  _map(X) FASTFMT_PP_MAP_1(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_3(_map, X, ...)  _map(X) FASTFMT_PP_MAP_2(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_4(_map, X, ...)  _map(X) FASTFMT_PP_MAP_3(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_5(_map, X, ...)  _map(X) FASTFMT_PP_MAP_4(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_6(_map, X, ...)  _map(X) FASTFMT_PP_MAP_5(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_7(_map, X, ...)  _map(X) FASTFMT_PP_MAP_6(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_8(_map, X, ...)  _map(X) FASTFMT_PP_MAP_7(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_9(_map, X, ...)  _map(X) FASTFMT_PP_MAP_8(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_10(_map, X, ...) _map(X) FASTFMT_PP_MAP_9(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_11(_map, X, ...) _map(X) FASTFMT_PP_MAP_10(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_12(_map, X, ...) _map(X) FASTFMT_PP_MAP_11(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_13(_map, X, ...) _map(X) FASTFMT_PP_MAP_12(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_14(_map, X, ...) _map(X) FASTFMT_PP_MAP_13(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_15(_map, X, ...) _map(X) FASTFMT_PP_MAP_14(_map, __VA_ARGS__)

#define FASTFMT_PP_GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, \
                             _map, ...)                                                            \
    _map

#define FASTFMT_PP_FOR_EACH(_map, ...)                                                           \
    FASTFMT_PP_GET_MACRO(_0, _map, FASTFMT_PP_MAP_15, FASTFMT_PP_MAP_14, FASTFMT_PP_MAP_13,      \
                         FASTFMT_PP_MAP_12, FASTFMT_PP_MAP_11, FASTFMT_PP_MAP_10,                \
                         FASTFMT_PP_MAP_9, FASTFMT_PP_MAP_8, FASTFMT_PP_MAP_7, FASTFMT_PP_MAP_6, \
                         FASTFMT_PP_MAP_5, FASTFMT_PP_MAP_4, FASTFMT_PP_MAP_3, FASTFMT_PP_MAP_2, \
                         FASTFMT_PP_MAP_1, FASTFMT_PP_MAP_0)                                     \
    (_map, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // PREPROC_H
