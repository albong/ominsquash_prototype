#ifndef LOGGING_H
#define LOGGING_H

//Level 0: log info, warnings, and errors
//Level 1: log warnings and errors
//Level 2: log errors
#define LOGGING_LEVEL 0

//INF=INFO, WAR=WARNING, ERR=ERROR.  I have a thing about equal lengths.
#define LOG_INF(fmt, ...) (LOGGING_LEVEL > 0) ?: (printf("INF: " fmt "\n", __VA_ARGS__))
#define LOG_WAR(fmt, ...) (LOGGING_LEVEL > 1) ?: (printf("WAR: " fmt "\n", __VA_ARGS__))
#define LOG_ERR(fmt, ...) (LOGGING_LEVEL > 2) ?: (printf("ERR: " fmt "\n", __VA_ARGS__))

#endif
