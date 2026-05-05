src = main.c timer.c util.c
flags = -lncurses -Wall -Wextra -Werror
target = cron
cc = gcc

all:
	$(cc) $(src) $(flags) -o $(target)

set:
	sudo cp $(target) /usr/local/bin/
