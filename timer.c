#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "timer.h"
#include "util.h"

#define TIMER_LINES 3
#define TIMER_COLS 19
#define NUM_PADDING 15

WINDOW *g_timer_win;

static void draw_finish_message(void);
static WINDOW *build_timer(void);
static void draw_time(double time);

static int g_timer_interrupt, g_time_goal, g_timer_finished;
static clock_t g_time_t0, g_time_pause_adjust, g_pause_t0;

static WINDOW *build_timer(void)
{
	WINDOW *timer = newwin(TIMER_LINES, TIMER_COLS, (LINES - TIMER_LINES)/2, (COLS - TIMER_COLS)/2);

	if (timer == NULL) error_n_die();
	if (box(timer, 0, 0) == ERR) error_n_die();
	if (wrefresh(timer) == ERR) error_n_die();
	
	return timer;
}

static void draw_finish_message(void)
{
	if (werase(g_timer_win) == ERR) error_n_die();
	char *end_msg = "time ended!";
	
	mvwprintw(g_timer_win, TIMER_LINES/2, (TIMER_COLS - strlen(end_msg))/2, "%s", end_msg);	
	if (box(g_timer_win, 0, 0) == ERR) error_n_die();
	if (wrefresh(g_timer_win) == ERR) error_n_die();
}

static void draw_time(double time)
{
	if (werase(g_timer_win) == ERR) error_n_die();
	if (box(g_timer_win, 0, 0) == ERR) error_n_die();
	mvwprintw(g_timer_win, TIMER_LINES/2, (TIMER_COLS - NUM_PADDING)/2, "%0*ld", NUM_PADDING, (long)time);	
	if (wrefresh(g_timer_win) == ERR) error_n_die();
}

static void draw_paused_state(void)
{
	mvwprintw(g_timer_win, 0, 1, "paused");
	if (wrefresh(g_timer_win) == ERR) error_n_die();	
}

static void send_notification()
{
	short retval = system("notify-send 'session finished' 'take a break!'");

	if (retval == -1) return;
		error_n_die();
}

/* 
============================
INTERFACE
============================
*/

void timer_initialize(void)
{
	g_timer_finished = -1;
	g_timer_interrupt = -1;
	
	g_time_pause_adjust = 0;
	g_pause_t0 = 0;
	if (time(&g_time_t0) == -1) error_n_die();
	
	g_timer_win = build_timer();
}

int timer_set_time(int time, short time_scale)
{
	switch(time_scale)
	{
		case SCALE_MINUTES:
			g_time_goal = time * 60;
			return 0;
			break;
			
		case SCALE_HOURS:
			g_time_goal = time * 120;
			return 0;
			break;
			
		default:
			return -1;
	}
}

int timer_update_time(void)
{
	if (g_timer_finished == TIME_ENDED) return TIME_ENDED;
	if (g_timer_interrupt == INTERR) return INTERR;

	time_t t1;
	if (time(&t1) == -1) error_n_die();

	double elapsed_time = difftime(t1 - g_time_pause_adjust, g_time_t0);
	
	if (elapsed_time >= g_time_goal)
	{
		if (g_timer_finished != TIME_ENDED)
			send_notification();	
		
		g_timer_finished = TIME_ENDED;
		draw_finish_message();
		return TIME_ENDED;
	}

	draw_time(elapsed_time);		

	return 1;
}

void timer_pause(void)
{
	if (g_timer_interrupt == INTERR) return;

	if (g_timer_finished != TIME_ENDED)
		draw_paused_state();

	g_timer_interrupt = INTERR;
	if (time(&g_pause_t0) == -1) error_n_die();
}

void timer_resume(void)
{
	if (g_timer_interrupt == -1) return;
	if (g_timer_finished == TIME_ENDED) return;
	
	g_timer_interrupt = -1;
	clock_t t1;
	if (time(&t1) == -1) error_n_die();
	// how many time since pause
	g_time_pause_adjust += t1 - g_pause_t0;
}

void timer_delwin(void)
{
	if (delwin(g_timer_win) == ERR) error_n_die();	
}
