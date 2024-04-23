comp:
	gcc -Wall -o vdir vdir.c

test_one:
	./vdir ../target ../dir1

test_all:
	./vdir ../target ../dir1 ../dir2 ../dir3 ../dir4 ../dir5 ../dir6 ../dir7 ../dir8 ../dir9

remove:
	./setup.sh r

create:
	./setup.sh c

refresh:
	make comp
	make remove
	make create

m?=com
git_up:
	git add .
	git commit -m "$(m)"
	git push
	