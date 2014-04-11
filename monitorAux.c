#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]){
	
	if (argc != 3) {
		printf("Wrong number of arguments, expected \"monitorAux <word> <file>\"\n");
		exit(1);
	}
	
	int fd1[2], fd2[2], s;
	char results[256], customTime[128];
	time_t currTime;
	struct tm *localTime;
	
	//generating pipes
	pipe(fd1);
	pipe(fd2);
	
	//forking tail process
	if(fork() == 0){
		close(fd1[READ]);
		dup2(fd1[WRITE], STDOUT_FILENO);
		
		execlp("tail", "tail", "-f", "-n 0", argv[2], NULL);
	}else{
		//forking grep process
		if(fork() == 0){
			close(fd1[WRITE]);
			dup2(fd1[READ], STDIN_FILENO);
		
			close(fd2[READ]);
			dup2(fd2[WRITE], STDOUT_FILENO);
		
			execlp("grep", "grep", "--line-buffered", argv[1], NULL);
		}else{
			close(fd2[WRITE]);
			while(1){
				//read new modifications from buffer
				memset(&results[0], 0, sizeof(results));
				s=read(fd2[READ], &results, sizeof(results));
				results[s-1]='\0';
	
				//generate timestamp
				currTime = time(NULL);
				localTime = localtime (&currTime);
				strftime(customTime, sizeof(customTime), "%Y-%m-%dT%H:%M:%S", localTime);
	
				//print modifications
				printf("%s - %s - %s\n", customTime, argv[2], results);
			}
		}
	}
	return 0;
}
