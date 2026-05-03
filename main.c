#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "timer.h"

#define MENU_LINES 5
#define MENU_COLS 8

enum opts {OPT_RESUME, OPT_PAUSE, OPT_QUIT};
char *g_menu_opts[] = {"resume", "pause ", "quit  "};
int g_cur_opt, g_running;

WINDOW *g_menu;

void prepare_stdscr()
{
	initscr();
	refresh(); 
	cbreak();
	noecho();
	keypad(stdscr, 1);
	curs_set(0);
	timeout(0); /* non-blocking getch()*/
}

void usage_n_die()
{
	puts("usage: bin_name <time>");
	exit(1);	
}

void check_args(int argc, char **argv)
{
	if (argc == 1) usage_n_die();
	if (atoi(argv[1]) == 0) usage_n_die();
}

void update_menu()
{	
	box(g_menu, 0, 0);
	for (int i = 0; i < 3; i++)
	{
		if (i == g_cur_opt)
		{
			wattron(g_menu, A_REVERSE);
			mvwprintw(g_menu, i+1, 1, "%s", g_menu_opts[i]);
			wattroff(g_menu, A_REVERSE);
			continue;
		}
		
		mvwprintw(g_menu, i+1, 1, "%s", g_menu_opts[i]);
	}		
	
	wrefresh(g_menu);
}

void build_menu(WINDOW **menu)
{
	*menu = newwin(MENU_LINES, MENU_COLS, LINES - MENU_LINES, (COLS - MENU_COLS)/2);
	box(*menu, 0, 0);
	wrefresh(*menu);
}

void execute_opt()
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

void handle_io()
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
	}
}

int main(int argc, char **argv)
{
	check_args(argc, argv);
	prepare_stdscr();

	timer_initialize();
	timer_set_time(atoi(argv[1]), SCALE_MINUTES);
	
	build_menu(&g_menu);
	update_menu();

	g_running = 1;

	/*main loop*/
	while(g_running) 
	{
		handle_io();
		timer_update_time();			
		update_menu();
		usleep(1000);
	}

	timer_delwin();	
	delwin(g_menu);
	endwin();
	return 0;
}
