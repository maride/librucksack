#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 10240

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Usage: %s <file to read using unistd>\n", argv[0]);
		return 1;
	}

	int fh = open(argv[1], 0);
	char buf[BUFSIZE];
	int numread = read(fh, &buf, BUFSIZE);
	close(fh);

	write(1, buf, numread);
}