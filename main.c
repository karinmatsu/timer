#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#define TIMER_LINES 3
#define TIMER_COLS 20
#define NUM_PADDING 10

void prepare_screen()
{
	cbreak();
	noecho();
	keypad(stdscr, 1);
	curs_set(0);
}

WINDOW *set_timer()
{
	WINDOW *timer = newwin(TIMER_LINES, TIMER_COLS, (LINES - TIMER_LINES)/2, (COLS - TIMER_COLS)/2);
	refresh();
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

void usage()
{
	puts("usage: bin_name [time]");
	exit(1);	
}

/*
1-9 10-99 100-999 1000-9999
9    89     899      8999
9
*/
int main(int argc, char **argv)
{
	if (argc == 1) usage();

	int minutes = atoi(argv[1]);
		
	if (minutes == 0) usage();
		
	initscr();
	prepare_screen();
	WINDOW *timer = set_timer();

	
	int time = 60 * atoi(argv[1]);
	
	for (int i = 0; i < time; i++) 
	{ 
		update_timer(timer, i, NUM_PADDING);
		sleep(1);
	}
			
	system("notify-send 'boa!' 'sessão finalizada'");

	char *end_msg = "time ended!";
	
	mvwprintw(timer, TIMER_LINES/2, (TIMER_COLS - strlen(end_msg))/2, "%s", end_msg);	
	wrefresh(timer);
	getch();

	delwin(timer);	
	endwin();
	return 0;
}
