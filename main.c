#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

#define TIMER_LINES 3
#define TIMER_COLS 19
#define MENU_LINES 5
#define MENU_COLS 8
#define NUM_PADDING 15

enum opts {OPT_RESUME, OPT_PAUSE, OPT_QUIT};
char *g_menu_opts[] = 
{"resume", "pause ", "quit  "};
WINDOW *g_menu, *g_timer;
int g_running, g_timer_finished, g_cur_opt, g_time_upper_limit;

void draw_finish_message(WINDOW *timer)
{
	char *end_msg = "time ended!";
	
	mvwprintw(timer, TIMER_LINES/2, (TIMER_COLS - strlen(end_msg))/2, "%s", end_msg);	
	box(timer, 0, 0);
	wrefresh(timer);
	getch();	
}

void prepare_stdscr()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	curs_set(0);
	timeout(0); // non-blocking getch()
}

void build_timer(WINDOW **timer)
{
	*timer = newwin(TIMER_LINES, TIMER_COLS, (LINES - TIMER_LINES)/2, (COLS - TIMER_COLS)/2);
	refresh();
	box(*timer, 0, 0);
	wrefresh(*timer);
}

void update_timer(time_t initial_time)
{
	time_t current_time;
	time(&current_time);
	double diff = difftime(current_time, initial_time);
	
	if ( diff >= g_time_upper_limit)
	{
		werase(g_timer);
		draw_finish_message(g_timer);
		g_timer_finished = 1;
		system("notify-send 'session ended' 'great work!'");
		return;
	}
	
	werase(g_timer);
	box(g_timer, 0, 0);
	mvwprintw(g_timer, TIMER_LINES/2, (TIMER_COLS - NUM_PADDING)/2, "%0*ld", NUM_PADDING, (long) diff);	
	wrefresh(g_timer);
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
	
	build_timer(&g_timer);
	build_menu(&g_menu);
	
	g_time_upper_limit = atoi(argv[1]);
	time_t sys_time_base;
	time(&sys_time_base);

	g_running = 1;
	g_timer_finished = 0;
	
	while(g_running) 
	{
		handle_io();
		update_menu();

		if (g_timer_finished == 0)
			update_timer(sys_time_base);

		usleep(100); // avoid CPU burning
	}
	
	delwin(g_timer);	
	delwin(g_menu);
	endwin();
	endwin();
	return 0;
}
