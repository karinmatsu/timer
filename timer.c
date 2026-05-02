#include "timer.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define TIMER_LINES 3
#define TIMER_COLS 19
#define NUM_PADDING 15

WINDOW *g_timer_win;

static void draw_finish_message();
static WINDOW *build_timer();
static void draw_time(int time);

int g_timer_interrupt, g_time_upper_limit, g_timer_finished, g_time_backup;
clock_t g_sys_time_base;

static WINDOW *build_timer()
{
	WINDOW *timer = newwin(TIMER_LINES, TIMER_COLS, (LINES - TIMER_LINES)/2, (COLS - TIMER_COLS)/2);
	box(timer, 0, 0);
	wrefresh(timer);
	return timer;
}

static void draw_finish_message()
{
	werase(g_timer_win);
	char *end_msg = "time ended!";
	
	mvwprintw(g_timer_win, TIMER_LINES/2, (TIMER_COLS - strlen(end_msg))/2, "%s", end_msg);	
	box(g_timer_win, 0, 0);
	wrefresh(g_timer_win);
}

static void draw_time(int time)
{
	werase(g_timer_win);
	box(g_timer_win, 0, 0);
	mvwprintw(g_timer_win, TIMER_LINES/2, (TIMER_COLS - NUM_PADDING)/2, "%0*ld", NUM_PADDING, (long) time);	
	wrefresh(g_timer_win);
}

void draw_paused_state()
{
	mvwprintw(g_timer_win, 0, 1, "paused");
	wrefresh(g_timer_win);	
}

/* 
============================
INTERFACE
============================
*/

void timer_initialize()
{
	g_timer_finished = 0;
	g_timer_interrupt = 0;
	g_time_backup = 0;
	time(&g_sys_time_base);
	
	g_timer_win = build_timer();
}

void timer_set_time(int time, short time_scale)
{
	switch(time_scale)
	{
		case SCALE_MINUTES:
			g_time_upper_limit = time * 60;
			break;
	}
}

int timer_update_timer()
{
	if (g_timer_finished) return TIME_ENDED;
	if (g_timer_interrupt) return INTERR;
	
	time_t current_sys_time;
	time(&current_sys_time);
	g_time_backup = difftime(current_sys_time, g_sys_time_base);
	
	if (g_time_backup >= g_time_upper_limit)
	{
		g_timer_finished = 1;
		draw_finish_message();
		return TIME_ENDED;
	}

	draw_time(g_time_backup);		
	return 1;
}

void timer_pause()
{
	draw_paused_state();
	g_timer_interrupt = 1;
}
