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
static void draw_time(double time);

int g_timer_interrupt, g_time_goal, g_timer_finished;
clock_t g_time_t0, g_time_pause_adjust, g_pause_t0;

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

static void draw_time(double time)
{
	werase(g_timer_win);
	box(g_timer_win, 0, 0);
	mvwprintw(g_timer_win, TIMER_LINES/2, (TIMER_COLS - NUM_PADDING)/2, "%0*ld", NUM_PADDING, (long)time);	
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
	g_timer_finished = -1;
	g_timer_interrupt = -1;
	
	g_time_pause_adjust = 0;
	g_pause_t0 = 0;
	time(&g_time_t0);
	
	g_timer_win = build_timer();
}

void timer_set_time(int time, short time_scale)
{
	switch(time_scale)
	{
		case SCALE_MINUTES:
			g_time_goal = time * 60;
			break;
	}
}

int timer_update_time()
{
	if (g_timer_finished == TIME_ENDED) return TIME_ENDED;
	if (g_timer_interrupt == INTERR) return INTERR;

	time_t time_t1;
	time(&time_t1);

	double elapsed_time = difftime(time_t1 - g_time_pause_adjust, g_time_t0);
	
	if (elapsed_time >= g_time_goal)
	{
		if (g_timer_finished != TIME_ENDED)
			system("notify-send 'session finished' 'take a break!'");
			
		g_timer_finished = TIME_ENDED;
		draw_finish_message();
		return TIME_ENDED;
	}

	draw_time(elapsed_time);		

	return 1;
}

void timer_pause()
{
	if (g_timer_interrupt == INTERR) return;

	if (g_timer_finished != TIME_ENDED)
		draw_paused_state();

	g_timer_interrupt = INTERR;
	time(&g_pause_t0);
}

void timer_resume()
{
	if (g_timer_interrupt == -1) return;
	if (g_timer_finished == TIME_ENDED) return;
	
	g_timer_interrupt = -1;
	clock_t t1;
	time(&t1);
	// how many time since pause
	g_time_pause_adjust += t1 - g_pause_t0;
}

void timer_delwin()
{
	delwin(g_timer_win);	
}


