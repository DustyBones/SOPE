#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]){
	
	if (argc <= 3) {
		printf("Wrong number of arguments, expected \"monitorFile <filesNumber> <filePid1> <fileName1> ...<filePidN> <fileNameN> \"\n");
		exit(1);
	}
	
	int i, j, fileNumber;
	fileNumber=strtod(argv[1], NULL);
	char* fileName[fileNumber];
	int pidAUX[fileNumber];
	
	for(i=0, j=2; i<fileNumber; ++i, ++j){
		fileName[j]=argv[j*2];
		pidAUX[j]=*argv[j*2+1];
	}
	
	while(fileNumber>0){
		for(i=0, j=0; i<fileNumber; ++i, ++j){
			if(pidAUX[j]!=0){
				if(fopen(fileName[i], "r")==NULL){
					if(pidAUX[i]!=-1){
						killpg(pidAUX[i], SIGUSR1);
					}
					fileNumber--;
					pidAUX[j]=0;
				}
			}
		}
		sleep(5);
	}
	kill(getppid(),SIGUSR2);
	return 0;
}
