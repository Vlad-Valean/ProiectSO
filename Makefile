run:
	gcc -Wall -o vdir vdir.c
	./setup.sh r
	./setup.sh c

remove:
	./setup.sh r

create:
	./setup.sh c
