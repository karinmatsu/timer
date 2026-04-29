#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#define TIMER_LINES 3
#define TIMER_COLS 20

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

int main(int , char **argv)
{
	initscr();
	prepare_screen();
	WINDOW *timer = set_timer();

	int time = 60 * atoi(argv[1]);
	int total_digits = 1;
	for (int i = 0; i < time; i++)
	{
		werase(timer);
		box(timer, 0, 0);
		mvwprintw(timer, TIMER_LINES/2, (TIMER_COLS - total_digits)/2, "%d", i);	
		wrefresh(timer);
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
