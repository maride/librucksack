#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Usage: %s <file to stat>\n", argv[0]);
		return 1;
	}

	struct stat sbuf1;
	struct stat sbuf2;
	struct stat sbuf3;

	// stat
	stat(argv[1], &sbuf1);
	printf("stat @ %s:\t UID/GUID %i/%i, size %li\n", argv[1], sbuf1.st_uid, sbuf1.st_gid, sbuf1.st_size);

	// fstat
	int fh = open(argv[1], 0);
	fstat(fh, &sbuf2);
	printf("fstat @ %s:\t UID/GUID %i/%i, size %li\n", argv[1], sbuf2.st_uid, sbuf2.st_gid, sbuf2.st_size);

	// lstat
	lstat(argv[1], &sbuf3);
	printf("lstat @ %s:\t UID/GUID %i/%i, size %li\n", argv[1], sbuf3.st_uid, sbuf3.st_gid, sbuf3.st_size);	
}