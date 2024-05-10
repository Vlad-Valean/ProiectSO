comp:
	gcc -Wall -o vdir vdir.c

test_one:
	make refresh
	./vdir -o ../target -s ../quarantine ../dir1

test_all:
	make refresh
	./vdir -o ../target -s ../quarantine ../dir1 ../dir2 ../dir3 ../dir4 ../dir5 ../dir6 ../dir7 ../dir8 ../dir9

remove:
	./scripts/setup.sh r

create:
	./scripts/setup.sh c

refresh:
	make comp
	make remove
	make create

m?=wip
git_up:
	git add .
	git commit -m "$(m)"
	git push
	