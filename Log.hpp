#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdio>

template <typename ...Args>
void log_e(const char *fmt, const Args &...args) {
    std::fprintf(stderr, fmt, args...);
}

template <typename ...Args>
void log_i(const char *fmt, const Args &...args) {
    std::fprintf(stdout, fmt, args...);
}

template <typename ...Args>
void log_d(const char *fmt, const Args &...args) {
    #ifndef NDEBUG
    printf(fmt, args...);
    #endif
}