#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int* pidAUX;
int pidMonitorFile;
int	fileNumber;

void alarmHandler(int singo){
	int i;
	for(i = 0; i < fileNumber; ++i){
		if(pidAUX[i] != -1){
			killpg(pidAUX[i], SIGUSR1);
		}
	}
	kill(getpid(), SIGUSR2);
	
	exit(0);
}

void zombieHandler(int signo){
	int status;
	wait(&status);
}

void terminateHandler(int signo){
	kill(pidMonitorFile, SIGUSR1);
	kill(getpid(), SIGUSR1);
}

char* getCWD() {
	char* cwd = (char*) malloc(1024*sizeof(char));
	getcwd(cwd, 1024);
	return cwd;
}

char* getMonitorAuxPath(char* cwd){
	char* monitorAuxPath = malloc(strlen(cwd)+1);
	monitorAuxPath[0] = '\0';
	strcat(monitorAuxPath, cwd);
	strcat(monitorAuxPath, "/monitorAux");
	return monitorAuxPath;
}
char* getMonitorFilePath(char* cwd){
	char* fileMonitorPath = malloc(strlen(cwd)+1);
	fileMonitorPath[0] = '\0';
	strcat(fileMonitorPath, cwd);
	strcat(fileMonitorPath, "/fileMonitor");
	return fileMonitorPath;
}

int main (int argc, char *argv[]) {
	
	if (argc < 4) {
		printf("Wrong number of arguments, expected \"monitor <time> <word> <file1> ... <fileN>\"\n");
		exit(1);
	}
	//generate auxiliar processes
	int a[argc-3];
	pidAUX=a;
	int i, j;
	
	//fork a monitor for each file
	for(i=3, j=0; i<argc; ++i, ++j){
		if((pidAUX[j]=fork())==0){
			setpgid(getpid(),getpid());
			execlp(getMonitorAuxPath(getCWD()), "monitorAux", argv[2], argv[i], NULL);
		}
	}
	
	//fork file monitor
	char* args[2+2*(argc-3)];
	if((pidMonitorFile=fork())==0){
		args[0]="monitorFile";
		*(args[1])=argc-3;
		for(i=3, j=1; i<argc; ++i, ++j){
			args[j*2]=argv[i];
			*(args[j*2+1])=pidAUX[j-1];
		}
		execlp(getMonitorFilePath(getCWD()), *(args), NULL);
	}
	
	//install alarm
	struct sigaction act;
	act.sa_handler = alarmHandler;
	sigemptyset (&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, NULL);

	//start alarm
	alarm(strtoul(argv[1], NULL, 0));
	
	struct sigaction zombie;
	zombie.sa_handler = zombieHandler;
	sigemptyset (&zombie.sa_mask);
	zombie.sa_flags = 0;
	sigaction(SIGCHLD, &zombie, NULL);

	struct sigaction terminate; 
	terminate.sa_handler = terminateHandler;
	sigemptyset (&terminate.sa_mask);
	terminate.sa_flags = 0;
	sigaction(SIGUSR2, &terminate, NULL);

	while(1){
	};
	
	return 0;
}
