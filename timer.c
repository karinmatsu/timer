#include "timer.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define TIMER_LINES 3
#define TIMER_COLS 19
#define NUM_PADDING 15

WINDOW *g_timer_win;

static void draw_finish_message(WINDOW *timer);
static WINDOW *build_timer();
static void update_timer(time_t initial_time);

int g_running, g_timer_interrupt, g_time_upper_limit, g_timer_finished;

static WINDOW *build_timer()
{
	WINDOW *timer = newwin(TIMER_LINES, TIMER_COLS, (LINES - TIMER_LINES)/2, (COLS - TIMER_COLS)/2);
	box(timer, 0, 0);
	wrefresh(timer);
	return timer;
}

static void draw_finish_message(WINDOW *timer)
{
	char *end_msg = "time ended!";
	
	mvwprintw(timer, TIMER_LINES/2, (TIMER_COLS - strlen(end_msg))/2, "%s", end_msg);	
	box(timer, 0, 0);
	wrefresh(timer);
	getch();	
}

/* 
============================
INTERFACE
============================
*/

void timer_initialize()
{
	g_running = 0;
	g_timer_finished = 0;
	g_timer_interrupt = 0;

	g_timer_win = build_timer();
}

void timer_set_minutes(int minutes)
{
	g_time_upper_limit = 60 * minutes;
}

void update_timer(time_t initial_time)
{
	time_t current_time;
	time(&current_time);
	double diff = difftime(current_time, initial_time);
	
	if ( diff >= g_time_upper_limit)
	{
		werase(g_timer_win);
		draw_finish_message(g_timer_win);
		g_timer_finished = 1;
		system("notify-send 'session ended' 'great work!'");
		return;
	}
	
	werase(g_timer_win);
	box(g_timer_win, 0, 0);
	mvwprintw(g_timer_win, TIMER_LINES/2, (TIMER_COLS - NUM_PADDING)/2, "%0*ld", NUM_PADDING, (long) diff);	
	wrefresh(g_timer_win);
}
