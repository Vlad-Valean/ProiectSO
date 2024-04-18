compile:
	gcc -Wall -o vdir vdir.c
	./setup.sh r
	./setup.sh c

test:
	./vdir ../target ../dir1 ../dir2 ../dir3 ../dir4 ../dir5 ../dir6 ../dir7

remove:
	./setup.sh r

create:
	./setup.sh c
