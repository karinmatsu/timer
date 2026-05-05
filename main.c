#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "timer.h"
#include "util.h"

#define MENU_LINES 5
#define MENU_COLS 8

enum opts {OPT_RESUME, OPT_PAUSE, OPT_QUIT};
char *g_menu_opts[] = {"resume", " stop ", " quit "};
int g_cur_opt, g_running;

WINDOW *g_menu;

void end_ncurses()
{
	timer_delwin();	
	delwin(g_menu);
	endwin();
}

void prepare_stdscr(void)
{
	if (initscr() == NULL) error_n_die();
	if (refresh() == ERR) error_n_die(); 
	if (cbreak() == ERR) error_n_die();
	if (noecho() == ERR) error_n_die();
	if (keypad(stdscr, 1) == ERR) error_n_die();
	if (curs_set(0) == ERR) error_n_die();
	timeout(0); /* non-blocking getch()*/
}

void check_args(int argc, char **argv)
{
	if (argc == 1 || argc < 3 || argc > 3) usage_n_die();
	// assuming user is smart and will put something different than 0.
	if (atoi(argv[2]) == 0) usage_n_die();
}	

void update_menu(void)
{	
	if (box(g_menu, 0, 0) == ERR) error_n_die();
	 
	for (int i = 0; i < 3; i++)
	{
		if (i == g_cur_opt)
		{
			if (wattron(g_menu, A_REVERSE) == ERR) error_n_die();
			mvwprintw(g_menu, i+1, 1, "%s", g_menu_opts[i]);
			if (wattroff(g_menu, A_REVERSE) == ERR) error_n_die();
			continue;
		}
		
		mvwprintw(g_menu, i+1, 1, "%s", g_menu_opts[i]);
	}		
	
	if (wrefresh(g_menu) == ERR) error_n_die();
}

void build_menu(void)
{
	g_menu = newwin(MENU_LINES, MENU_COLS, LINES - MENU_LINES, (COLS - MENU_COLS)/2);
	if (box(g_menu, 0, 0) == ERR) error_n_die();
	if (wrefresh(g_menu) == ERR) error_n_die();
}

void execute_opt(void)
{
	switch(g_cur_opt)
	{
		case OPT_QUIT:
			g_running = 0;
			break;

		case OPT_PAUSE:
			timer_pause();
			break;

		case OPT_RESUME:
			timer_resume();
			break;
	}
}

void handle_io(void)
{
	int ch = getch(); 

	if (ch == ERR) return;
	
	switch(ch)
	{
		case 's':
			if (g_cur_opt >= 2)
			{
				g_cur_opt = 2;
				break;	
			} 
			g_cur_opt++;
			break;

		case 'w':
			if (g_cur_opt <= 0)
			{
				g_cur_opt = 0;
				break;	
			} 
			g_cur_opt--;
			break;
			
		case 'j':
			execute_opt();
			break;

		default:
			return;
			break;
	}
}

short get_scale(char *opt) 
{
	if (strcmp(opt, "-m") == 0)
		return SCALE_MINUTES;
	else if (strcmp(opt, "-h") == 0)
		return SCALE_HOURS;

	return -1;
}

int main(int argc, char **argv)
{
	check_args(argc, argv);

	short scale = get_scale(argv[1]);
	
	if (scale == -1)
	{
		printf("unrecognized '%s' parameter.\n", argv[1]);
		usage_n_die();
	}	

	/*initialize curses mode*/
	prepare_stdscr();

	timer_initialize();

	if (timer_set_time(atoi(argv[2]), scale) != 0)
		exit(1);
	
	build_menu();

	g_running = 1;

	/*main loop*/
	while(g_running) 
	{
		handle_io();
		timer_update_time();			
		update_menu();
		usleep(1000);
	}

	end_ncurses();

	return EXIT_SUCCESS;
}
