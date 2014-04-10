#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
	if (argc < 4) {
		printf("Wrong number of arguments, format is \"monitor time word file1.txt ... fileN.txt\"\n");
		exit(1);
	}
	int fd1 = open(argv[3], O_RDWR | O_APPEND); //hardcoded values for initial testing
	if (fd1 < 0) {
		printf("No file found\n");
	};
	int fd[2];
	pipe(fd);
	if (fork() > 0) {
		int status;
		wait(&status);
	} else {
		//dup2(fd[1], STDOUT_FILENO);
		//while(1) {
		execlp("tail", "tail", "--follow=name", "-n 0", argv[3], NULL); //hardcoded value
		//tive de usar follow=name porque o gedit gosta de mudar o descritor quando grava e por isso o tail deixa de
		//conseguir receber dados. Mesmo com follow=name o gedit nao da porque ele escreve tudo quando grava, para testar isto
		//usa-se o vi (talvez vim) ou ate mesmo um mini programa que faça write que eu inclui no github
	//}
	}
	return 0;
}
