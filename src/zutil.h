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

//Horn codes
#define HORN_FACTORY_RESET	21
#define HORN_OK			22
#define HORN_CRASH_EXPANDER	23
#define HORN_CRASH_NOI2C	24
#define HORN_CRASH_NONET	25
#define HORN_CRASH_NOPARTICLE	26
#define HORN_CRASH_NOBLYNK	27
#define HORN_CRASH_NOPMCOUNTER	28
#define HORN_CRASH_NOPMDATA	29
#define HORN_CRASH_LOGIC	31
#define HORN_CRASH_SEETEXT	32

void crash(int code);
void reboot(int code);
void panic(int code);

void crash(char *msg);
void reboot(char *msg);
void panic(char *msg);

#endif
