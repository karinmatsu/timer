#pragma once

#define SCALE_MINUTES 0
#define SCALE_HOURS 1

#define TIME_ENDED 0
#define INTERR 1

int timer_update_time();
void timer_set_time(int time, short time_scale);
void timer_initialize();
void timer_pause();
void timer_delwin();
void timer_resume();
