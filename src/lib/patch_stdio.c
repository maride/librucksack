#define _GNU_SOURCE

#include "tracker.h"

#include "../common/pack_entry.h"
#include "../common/pack_util.h"

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

FILE* (*original_fopen)(const char *pathname, const char *mode);
FILE* (*original_fdopen)(int fd, const char *mode);
FILE* (*original_freopen)(const char *pathname, const char *mode, FILE *stream);
size_t (*original_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t (*original_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);

void _stdio_get_original_links() {
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fdopen = dlsym(RTLD_NEXT, "fdopen");
	original_freopen = dlsym(RTLD_NEXT, "freopen");
	original_fread = dlsym(RTLD_NEXT, "fread");
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
}

FILE* fopen(const char *pathname, const char *mode) {
	pack_entry* pe = get_pack_entry(pathname);
	if(pe != NULL) {
		// this path is packed, track it
		FILE* fd = original_fopen("/dev/null", "r");
		start_track(pathname, fd->_fileno, pe);
		list_tracked();
		return fd;
	} else {
		// path is not packed, passthrough
		return original_fopen(pathname, mode);
	}
}

FILE* fdopen(int fd, const char *mode) {
	return original_fdopen(fd, mode);
	track_entry* te = get_tracked(fd);
	if(te != NULL) {
		// this fd is tracked
		pack_entry* pe = te->packref;
		// TODO
	} else {
		// fd is not tracked, passthrough
		return original_fdopen(fd, mode);
	}
	
}

FILE* freopen(const char *pathname, const char *mode, FILE *stream) {
	// TODO
	return original_freopen(pathname, mode, stream);
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	track_entry* te = get_tracked(stream->_fileno);
	printf("[RUCKSACK] read from %i\n", stream->_fileno);
	if(te != NULL) {
		// this fd is tracked, read from pack
		pack_entry* pe = te->packref;

		// $count bytes should be read, check cursor position
		int left = pe->size - te->cursor;
		if(left > size * nmemb) {
			// read only $count bytes
			memcpy(ptr, &pe->data, size * nmemb);
			te->cursor += size * nmemb;
			return size * nmemb;
		} else {
			// read only left bytes
			memcpy(ptr, &pe->data, left);
			te->cursor += left;
			return left;
		}
	}
	return original_fread(ptr, size, nmemb, stream);
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	if(is_tracked(stream->_fileno)) {
		// librucksack doesn't support writing, let's fake it
		return size;
	}
	return original_fwrite(ptr, size, nmemb, stream);
}

// TODO: fseek, rewind
