#ifndef __tracker_h__
#define __tracker_h__

#include "../common/pack_entry.h"

#include <stdbool.h>

typedef struct track_entry{
	int fd;
	bool is_closed;
	const char* path;
	int cursor;
	pack_entry* packref;
} track_entry;

int start_track(const char* path, int real_fd, pack_entry* packref);
bool is_tracked(int fd);
track_entry* get_tracked(int fd);
void stop_track(int fd);
void list_tracked();

#endif // __tracker_h__
