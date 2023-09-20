#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 10240

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Usage: %s <file to read using stdio>\n", argv[0]);
		return 1;
	}

	FILE* fh = fopen(argv[1], "r");
	char buf[BUFSIZE];
	size_t numread = fread(&buf, 1, BUFSIZE, fh);
	fclose(fh);

	write(1, buf, numread);
}