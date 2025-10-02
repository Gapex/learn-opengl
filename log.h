//
// Created by gao on 2025/10/2.
//

#ifndef LEARN_OPENGL_LOG_H
#define LEARN_OPENGL_LOG_H

#include <cstdio>

#define LOGD(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        fprintf(stdout, "[D] [%s:%d %s]" fmt "\n", __FILE_NAME__, __LINE__, __func__, ##__VA_ARGS__);                  \
    } while (0)

#define LOGE(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        fprintf(stderr, "[E] [%s:%d %s]" fmt "\n", __FILE_NAME__, __LINE__, __func__, ##__VA_ARGS__);                  \
    } while (0)

#endif // LEARN_OPENGL_LOG_H