#ifndef LOGGING_H
#define LOGGING_H

//Level 0: log info, warnings, and errors
//Level 1: log warnings and errors
//Level 2: log errors
#define LOGGING_LEVEL 0

//INF=INFO, WAR=WARNING, ERR=ERROR.  I have a thing about equal lengths.
//the '##' before __VA_ARGS__ does some token nonsense that makes things work if there are no arguments
//removes the prior comma essentially.  M A G I C
#if LOGGING_LEVEL == 0
    #define LOG_INF(fmt, ...) printf("INFO %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define LOG_INF(fmt, ...)
#endif

#if LOGGING_LEVEL <= 1
    #define LOG_WAR(fmt, ...) printf("WARNING %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define LOG_WAR(fmt, ...)
#endif

#define LOG_ERR(fmt, ...) printf("ERROR " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
