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

WINDOW *g_menu, *g_timer;

void prepare_stdscr()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	curs_set(0);
	timeout(0); // non-blocking getch()
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
	refresh();
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
			

		case OPT_RESUME:
			
	}
}

void handle_io()
{
	int ch = getch();

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
			update_menu();
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
	
	build_menu(&g_menu);

	timer_initialize();
	timer_set_time(atoi(argv[1]), SCALE_MINUTES);
	
	g_running = 1;
	
	while(g_running) 
	{
		handle_io();
		update_menu();
		timer_update_timer();		
		refresh();
		sleep(0.5); // avoid CPU burning
	}
	
	delwin(g_timer);	
	delwin(g_menu);
	endwin();
	endwin();
	return 0;
}
