remove:
	./scripts/setup.sh r

create:
	./scripts/setup.sh c

refresh:
	make remove
	make create

comp:
	make refresh
	mkdir ./artefacts
	gcc -c ./src/main.c -o ./artefacts/main.o
	gcc -c ./src/processes_l/processes_l.c -o ./artefacts/processes_l.o
	gcc -c ./src/file_l/file_l.c -o ./artefacts/file_l.o
	gcc -c ./src/path_l/path_l.c -o ./artefacts/path_l.o
	gcc -o snapdir ./artefacts/main.o ./artefacts/processes_l.o ./artefacts/file_l.o ./artefacts/path_l.o

test_one:
	make comp
	./snapdir -o ../target -s ../quarantine ../dir1

test_all:
	make comp
	./snapdir -o ../target -s ../quarantine ../dir1 ../dir2 ../dir3 ../dir4 ../dir5 ../dir6 ../dir7 ../dir8 ../dir9

m?=wip
git_up:
	git add .
	git commit -m "$(m)"
	git push
	