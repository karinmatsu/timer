#pragma once

#define SCALE_MINUTES 0
#define SCALE_HOURS 1

#define TIME_ENDED 0
#define INTERR 1

/*
build the window and initilize it's variables.
Should be callend before all other timer funcion.
*/
void timer_initialize();
/*
used to specify how long the timer should count.
'time' will be interpreted based on 'time_scale'.
should be called before timer_update_window().
*/
void timer_set_time(int time, short time_scale);

/*update the timer window and it's current time.
Uses the current machine time to calculate the time.
*/
int timer_update_time();
void timer_pause();
void timer_delwin();
void timer_resume();
