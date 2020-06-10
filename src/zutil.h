//
#ifndef ZUTIL_H
#define ZUTIL_H

#include "application.h"
#include "Wire.h"

#define BUZZER D7

#define ZDEBUG 1

#ifdef ZDEBUG
#define debug(...) Serial1.print(__VA_ARGS__)
#define debugf(...) Serial1.printf(__VA_ARGS__)
#else
#define debug(...)
#define debugf(...)
#endif

void crash(char *msg);
void reboot(char *msg);
void panic(char *msg);

#endif
