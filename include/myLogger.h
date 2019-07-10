#ifndef MOOD_LIGHT_MY_LOGGER_H
#define MOOD_LIGHT_MY_LOGGER_H

#include <cstdio>
#include <ctime>

#define LOG_NAME "log.txt"

#define MY_LOGGER_LOG(...)                                                              \
    do {                                                                                \
        std::time_t t = std::time(nullptr);                                             \
        char mbstr[100] = { 0 };                                                        \
        std::strftime(mbstr, sizeof(mbstr), "%Y/%m/%d %H:%M:%S", std::localtime(&t));   \
        FILE *f = fopen(LOG_NAME, "a");                                                 \
        fprintf(f, "%s : ", mbstr);                                                     \
        fprintf(f, __VA_ARGS__);                                                        \
        fclose(f);                                                                      \
        fprintf(stderr, __VA_ARGS__);                                                   \
    } while(0);

#define MY_LOGGER_PRINT(...)                                                            \
    do {                                                                                \
        FILE *f = fopen(LOG_NAME, "a");                                                 \
        fprintf(f, __VA_ARGS__);                                                        \
        fclose(f);                                                                      \
        fprintf(stderr, __VA_ARGS__);                                                   \
    } while(0);

#endif /* !MOOD_LIGHT_MY_LOGGER_H */
