all: monitorMain.c monitorAux.c monitorFile.c
	mkdir -p bin
	gcc  monitorMain.c -o bin/monitor -Wall
	gcc  monitorAux.c -o bin/monitorAux -Wall
	gcc  monitorFile.c -o bin/monitorFile -Wall
