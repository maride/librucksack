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

int (*original_open)(const char *pathname, int flags);
ssize_t (*original_read)(int fd, void *buf, size_t count);
ssize_t (*original_write)(int fd, const void *buf, size_t count);
int (*original_close)(int fd);
int (*original_access)(const char *pathname, int mode);

void _unistd_get_original_links() {
	original_open = dlsym(RTLD_NEXT, "open");
	original_read = dlsym(RTLD_NEXT, "read");
	original_write = dlsym(RTLD_NEXT, "write");
	original_close = dlsym(RTLD_NEXT, "close");
	original_access = dlsym(RTLD_NEXT, "access");
}

int open(const char *pathname, int flags, ...) {
	pack_entry* pe = get_pack_entry(pathname);
	if(pe != NULL) {
		// this path is packed, track it
		int fd = original_open("/dev/null", 0);
		start_track(pathname, fd, pe);
		list_tracked();
		return fd;
	} else {
		// path is not packed, passthrough
		return original_open(pathname, flags);
	}
}

ssize_t read(int fd, void *buf, size_t count) {
	track_entry* te = get_tracked(fd);
	if(te != NULL) {
		// this fd is tracked, read from pack
		pack_entry* pe = te->packref;

		// $count bytes should be read, check cursor position
		int left = pe->size - te->cursor;
		if(left > count) {
			// read only $count bytes
			memcpy(buf, &pe->data, count);
			te->cursor += count;
			return count;
		} else {
			// read only left bytes
			memcpy(buf, &pe->data, left);
			te->cursor += left;
			return left;
		}
	}
	return original_read(fd, buf, count);
}

ssize_t write(int fd, const void *buf, size_t count) {
	if(is_tracked(fd)) {
		// librucksack doesn't support writing, let's fake it
		return count;
	}
	return original_write(fd, buf, count);
}

int close(int fd) {
	if(is_tracked(fd)) {
		stop_track(fd);
		list_tracked();
	}
	return original_close(fd);
}

int access(const char* pathname, int mode) {
	pack_entry* pe = get_pack_entry(pathname);
	if(pe != NULL) {
		// this path is packed, fake access mode
		return 0; // success
	} else {
		// path is not packed, passthrough
		return original_access(pathname, mode);
	}
}
