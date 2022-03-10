#ifndef PREPROC_H
#define PREPROC_H

#ifdef __cplusplus
extern "C" {
#endif

#define PP_NARG(...)  PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)

#define PP_RSEQ_N() 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N

#define PP_MAP_0(_map)
#define PP_MAP_1(_map, X)       _map(X)
#define PP_MAP_2(_map, X, ...)  _map(X) PP_MAP_1(_map, __VA_ARGS__)
#define PP_MAP_3(_map, X, ...)  _map(X) PP_MAP_2(_map, __VA_ARGS__)
#define PP_MAP_4(_map, X, ...)  _map(X) PP_MAP_3(_map, __VA_ARGS__)
#define PP_MAP_5(_map, X, ...)  _map(X) PP_MAP_4(_map, __VA_ARGS__)
#define PP_MAP_6(_map, X, ...)  _map(X) PP_MAP_5(_map, __VA_ARGS__)
#define PP_MAP_7(_map, X, ...)  _map(X) PP_MAP_6(_map, __VA_ARGS__)
#define PP_MAP_8(_map, X, ...)  _map(X) PP_MAP_7(_map, __VA_ARGS__)
#define PP_MAP_9(_map, X, ...)  _map(X) PP_MAP_8(_map, __VA_ARGS__)
#define PP_MAP_10(_map, X, ...) _map(X) PP_MAP_9(_map, __VA_ARGS__)
#define PP_MAP_11(_map, X, ...) _map(X) PP_MAP_10(_map, __VA_ARGS__)
#define PP_MAP_12(_map, X, ...) _map(X) PP_MAP_11(_map, __VA_ARGS__)
#define PP_MAP_13(_map, X, ...) _map(X) PP_MAP_12(_map, __VA_ARGS__)
#define PP_MAP_14(_map, X, ...) _map(X) PP_MAP_13(_map, __VA_ARGS__)
#define PP_MAP_15(_map, X, ...) _map(X) PP_MAP_14(_map, __VA_ARGS__)

#define PP_GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _map, \
                     ...)                                                                        \
    _map

#define PP_FOR_EACH(_map, ...)                                                                   \
    PP_GET_MACRO(_0, _map, PP_MAP_15, PP_MAP_14, PP_MAP_13, PP_MAP_12, PP_MAP_11, PP_MAP_10,     \
                 PP_MAP_9, PP_MAP_8, PP_MAP_7, PP_MAP_6, PP_MAP_5, PP_MAP_4, PP_MAP_3, PP_MAP_2, \
                 PP_MAP_1, PP_MAP_0)                                                             \
    (_map, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // PREPROC_H
