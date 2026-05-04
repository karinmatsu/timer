src = main.c timer.c
flags = -lncurses -Wall
target = cron
cc = gcc

all:
	$(cc) $(src) $(flags) -o $(target)

set:
	sudo cp $(target) /usr/local/bin/
