#define _GNU_SOURCE

#include "tracker.h"

#include "../common/pack_entry.h"
#include "../common/pack_util.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

int (*original_stat)(const char *pathname, struct stat *statbuf);
int (*original_fstat)(int fd, struct stat *statbuf);
int (*original_lstat)(const char *pathname, struct stat *statbuf);
int (*original_fstatat)(int dirfd, const char *pathname, struct stat *statbuf, int flags);

void _stat_get_original_links() {
	original_stat = dlsym(RTLD_NEXT, "stat");
	original_fstat = dlsym(RTLD_NEXT, "fstat");
	original_lstat = dlsym(RTLD_NEXT, "lstat");
	original_fstatat = dlsym(RTLD_NEXT, "fstatat");
}

int stat(const char *pathname, struct stat *statbuf) {
	pack_entry* pe = get_pack_entry(pathname);
	if(pe != NULL) {
		// this path is packed, give out correct size
		// TODO: override also access rights, dates, etc.
		statbuf->st_mode = S_IFREG; // regular file
		statbuf->st_size = pe->size;
		return 0;
	}
	return original_stat(pathname, statbuf);
}

int fstat(int fd, struct stat *statbuf) {
	track_entry* te = get_tracked(fd);
	if(te != NULL) {
		// this fd is tracked, read size from pack
		// TODO: override also access rights, dates, etc.
		pack_entry* pe = te->packref;
		statbuf->st_mode = S_IFREG; // regular file
		statbuf->st_size = pe->size;
		return 0;
	}
	return original_fstat(fd, statbuf);
}

int lstat(const char *pathname, struct stat *statbuf) {
	pack_entry* pe = get_pack_entry(pathname);
	if(pe != NULL) {
		// this path is packed, give out correct size
		// TODO: override also access rights, dates, etc.
		statbuf->st_mode = S_IFREG; // regular file
		statbuf->st_size = pe->size;
		return 0;
	}
	return original_lstat(pathname, statbuf);
}

int fstatat(int dirfd, const char *pathname, struct stat *statbuf, int flags) {
	pack_entry* pe = get_pack_entry(pathname);
	if(pe != NULL) {
		// TODO: could be challenging to implement relative stats
		return 0;
	}
	return original_fstatat(dirfd, pathname, statbuf, flags);
}
