#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main (int argc, char *argv[]) {
	
	if (argc < 4) {
		printf("Wrong number of arguments, format is \"monitor time word file1.txt ... fileN.txt\"\n");
		exit(1);
	}
	int i, pid, fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);
	for(i=2; i<argc; ++i){
		switch(pid){
			case 0:
				if(fork()>0){
					//
				}else{
					close(fd1[0]);
					dup2(fd1[1], STDOUT_FILENO);
					execlp("tail", "tail", "--follow=name", "-n 0", argv[i], NULL);
			
				}
				if(fork() > 0){
					//
				}else{
					close(fd1[1]);
					close(fd2[0]);
					dup2(fd1[0], STDIN_FILENO);
					dup2(fd2[1], STDOUT_FILENO);
					execlp("grep", "grep", "--line-buffered", argv[2], NULL);
				}
				char results[256], customTime[128];
				time_t currTime;
				currTime = time(NULL);
				struct tm *localTime=localtime (&currTime);
		
				strftime(customTime, sizeof(customTime), "%Y-%m-%dT%H:%M:%S", localTime);
				
				memset(&results[0], 0, sizeof(results));
				read(fd2[STDIN_FILENO], &results, sizeof(results));

				printf("%s - %s - %s", customTime, argv[i], results);
				
			default:
				pid=fork();
				wait(NULL);
		}
	}
	return 0;
}
