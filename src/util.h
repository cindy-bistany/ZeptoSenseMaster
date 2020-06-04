//
#ifndef UTIL_H
#define UTIL_H

#include "zstate.h"

void beep(String s);
void blinkpanic();

void deepSleep(Zstate *st);

void blynk_status_message(String msg);
void blynk_debug_message(String msg);

#endif
