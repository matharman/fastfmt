#ifndef PREPROC_H
#define PREPROC_H

#define FASTFMT_PP_ARG16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, ...) a15
#define FASTFMT_PP_NARG(...) \
    FASTFMT_PP_ARG16(dummy, ##__VA_ARGS__, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define FASTFMT_CONCAT(a, ...)  FASTFMT_CONCAT_(a, ##__VA_ARGS__)
#define FASTFMT_CONCAT_(a, ...) a##__VA_ARGS__

// clang-format off
#define FASTFMT_PP_MAP_0(_map)
#define FASTFMT_PP_MAP_1(_map, X)       _map(X);
#define FASTFMT_PP_MAP_2(_map, X, ...)  _map(X); FASTFMT_PP_MAP_1(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_3(_map, X, ...)  _map(X); FASTFMT_PP_MAP_2(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_4(_map, X, ...)  _map(X); FASTFMT_PP_MAP_3(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_5(_map, X, ...)  _map(X); FASTFMT_PP_MAP_4(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_6(_map, X, ...)  _map(X); FASTFMT_PP_MAP_5(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_7(_map, X, ...)  _map(X); FASTFMT_PP_MAP_6(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_8(_map, X, ...)  _map(X); FASTFMT_PP_MAP_7(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_9(_map, X, ...)  _map(X); FASTFMT_PP_MAP_8(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_10(_map, X, ...) _map(X); FASTFMT_PP_MAP_9(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_11(_map, X, ...) _map(X); FASTFMT_PP_MAP_10(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_12(_map, X, ...) _map(X); FASTFMT_PP_MAP_11(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_13(_map, X, ...) _map(X); FASTFMT_PP_MAP_12(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_14(_map, X, ...) _map(X); FASTFMT_PP_MAP_13(_map, __VA_ARGS__)
#define FASTFMT_PP_MAP_15(_map, X, ...) _map(X); FASTFMT_PP_MAP_14(_map, __VA_ARGS__)
// clang-format on

#define FASTFMT_MAP_(N, _map, ...)     FASTFMT_CONCAT(FASTFMT_PP_MAP_, N)(_map, __VA_ARGS__)
#define FASTFMT_MAP(_map, ...)         FASTFMT_MAP_(FASTFMT_PP_NARG(__VA_ARGS__), _map, __VA_ARGS__)
#define FASTFMT_PP_FOR_EACH(_map, ...) FASTFMT_MAP(_map, ##__VA_ARGS__)

#endif  // PREPROC_H
