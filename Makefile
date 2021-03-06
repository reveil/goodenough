exe_name := gdeh
vg_opts := --leak-check=full --track-origins=yes

ifdef DEBUG
debug := -g
else
debug :=
endif

all: goodenough.c
	gcc -Wall $(debug) -o $(exe_name) goodenough.c

clean:
	rm -f $(exe_name) goodenough.o

vg: all
	valgrind $(vg_opts) ./$(exe_name) 9090 google.pl 80

err: all
	valgrind $(vg_opts) ./$(exe_name) 80 google.pl 80

