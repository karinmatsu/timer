#ifndef TIMER_H
#define TIMER_H

#define SCALE_MINUTES 0
#define SCALE_HOURS 1

#define TIME_ENDED 0
#define INTERR 1

/*
build timer window. 
should be called before all other timer function.
*/
void timer_build(void);
/*
build the window and initilize it's variables.
should be called after initialize the timer 
window with timer_build()
*/
void timer_initialize(void);
/*
used to specify how long the timer should count.
'time' will be interpreted based on 'time_scale'.
should be called before timer_update_window().
*/
int timer_set_time(int time, short time_scale);

/*update the timer window and it's current time.
Uses the current machine time to calculate the time.
*/
int timer_update_time(void);

void timer_pause(void);
void timer_resume(void);
void timer_delwin(void);
void timer_reset(void);
#endif
