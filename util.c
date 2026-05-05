#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "util.h"

void error_n_die(void)
{
	endwin();
	fprintf(stderr, "Error: %s()\nline: %d file: %s\n", __FUNCTION__, __LINE__, __FILE__);
	exit(1);
}

void usage_n_die(void)
{
	puts("usage: cron [args] <time>");
	exit(1);	
}
