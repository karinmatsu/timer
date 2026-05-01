#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#define TIMER_LINES 3
#define TIMER_COLS 20
#define MENU_LINES 5
#define MENU_COLS 10
#define NUM_PADDING 16

enum opts {OPT_QUIT, OPT_CONTINUE, OPT_PAUSE};
char *g_menu_opts[] = {"resume", "pause ", "quit  "};

void prepare_screen()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	curs_set(0);
	timeout(0); // non-blocking getch()
}

WINDOW *build_timer()
{
	WINDOW *timer = newwin(TIMER_LINES, TIMER_COLS, (LINES - TIMER_LINES)/2, (COLS - TIMER_COLS)/2);
	refresh();
	box(timer, 0, 0);
	wrefresh(timer);
	return timer;
}

void update_timer(WINDOW *timer, int time, int padding)
{
	werase(timer);
	box(timer, 0, 0);
	mvwprintw(timer, TIMER_LINES/2, (TIMER_COLS - padding)/2, "%0*d", NUM_PADDING, time);	
	wrefresh(timer);
}

void usage_n_die()
{
	puts("usage: bin_name <time>");
	exit(1);	
}

void check_args(int argc, char **argv)
{
	if (argc == 1) usage_n_die();
	int minutes = atoi(argv[1]);
	if (minutes == 0) usage_n_die();
	
}

int update_menu(WINDOW *menu, int cur_opt)
{
	int ch = getch();

	switch(ch)
	{
		case 's':
			cur_opt++;
			break;

		case 'w':
			cur_opt--;
			break;
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (i == cur_opt)
		{
			wattron(menu, A_REVERSE);
			mvwprintw(menu, i+1, 1, "%s", g_menu_opts[i]);
			wattroff(menu, A_REVERSE);
			continue;
		}
		
		mvwprintw(menu, i+1, 1, "%s", g_menu_opts[i]);
	}		

	wrefresh(menu);
	return cur_opt;
}

void *build_menu()
{
	WINDOW *menu = newwin(MENU_LINES, MENU_COLS, LINES - MENU_LINES, (COLS - MENU_COLS)/2);
	refresh();
	box(menu, 0, 0);
	wrefresh(menu);
	return menu;
}

int main(int argc, char **argv)
{
	check_args(argc, argv);
	prepare_screen();
	
	WINDOW *timer = build_timer();
	WINDOW *menu = build_menu();
	
	int time = 60 * atoi(argv[1]);
	int cur_opt = 0;
	
	for (int i = 0; i < time; i++) 
	{
		cur_opt = update_menu(menu, cur_opt);

		update_timer(timer, i, NUM_PADDING);
		sleep(1);
	}
			
	system("notify-send 'session ended' 'great work!'");

	char *end_msg = "time ended!";
	
	mvwprintw(timer, TIMER_LINES/2, (TIMER_COLS - strlen(end_msg))/2, "%s", end_msg);	
	wrefresh(timer);
	getch();

	delwin(timer);	
	endwin();
	return 0;
}
