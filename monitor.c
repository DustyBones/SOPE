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
	/*int fd = open(argv[3], O_RDWR | O_APPEND); //hardcoded values for initial testing
	if (fd < 0) {
		printf("File not found...\n");
	}*/
	int i, pid, fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);
	
	//pid=fork();
	for(i=3; i<argc; ++i){
		switch(pid){
			case -1:
			
			case 0:
				if(fork()>0){
					//
				}else{
					close(fd1[0]);
					dup2(fd1[1], STDOUT_FILENO);
					execlp("tail", "tail", "--follow=name", "-n 0", argv[i], NULL); //hardcoded value
			
				}
				if(fork() > 0){
					//
				}else{
					dup2(fd1[0], STDIN_FILENO);
					dup2(fd2[1], STDOUT_FILENO);
					execlp("grep", "grep", "--line-buffered", argv[2], NULL); //hardcoded value
				}
				char results[256], customTime[128];
				time_t currTime;
				currTime = time(NULL);
				struct tm *localTime=localtime (&currTime);
		
				strftime(customTime, sizeof(customTime), "%Y-%m-%dT%H:%M:%S", localTime);
		
				read(fd2[STDIN_FILENO], &results, sizeof(results));
		
				//results[strcspn ( results, "\n" )] = '\0';
				printf("%s - %s - %c%s", customTime, argv[i], '"', results);//hardcoded value
				
			default:
				pid=fork();
				int status;
				wait(&status);
		}
	}
	return 0;
}
