#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: \"teste filename.txt\"\n");
	}
	int fd = open(argv[1], O_RDWR | O_APPEND);
	char buf[5];
	buf[0] = '\n';
	buf[1] = 'e';
	write(fd, buf, 2);
	close(fd);
	return 0;
}
