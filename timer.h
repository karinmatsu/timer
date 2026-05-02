#pragma once

#define SCALE_MINUTES 0
#define SCALE_HOURS 1

#define TIME_ENDED 0

int timer_update_timer();
void timer_set_time(int time, short time_scale);
void timer_initialize();
