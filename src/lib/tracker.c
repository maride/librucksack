#include "tracker.h"

#include "../common/pack_util.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// MAXFH is the number of tracked file handles.
// The current Linux limit is 1024. But keep in mind that there is no garbage collection in librucksack; closed fh's still remain in the list.
#define MAXFH 2048
track_entry track_array[MAXFH];
int track_array_size = 0;

// start_track starts to track a file handle which was opened for a packed file
//  path	is the path as presented to open()
//  fd		is the fd to reference this handle with
//  packref is a pointer to the actually packed entry
int start_track(const char* path, int fd, pack_entry* packref) {
	// check if there's enough space for another fd
	if(track_array_size >= MAXFH) {
		// Hit size limit. This means new fh's will be untracked and thus unpatched.
		// The binary will be able to access the on-disk files from this point on.
		// If you reach this point: raising MAXFH might already be enough to fix it.
		printf("[\033[33mRUCKSACK\033[0m] MAXFH (%i) reached, unable to handle new file handles.\n", MAXFH);
		return 0;
	}

	// create new element
	track_array[track_array_size].fd = fd;
	track_array[track_array_size].is_closed = false;
	track_array[track_array_size].path = path;
	track_array[track_array_size].cursor = 0;
	track_array[track_array_size].packref = packref;
	
	// update size
	track_array_size++;

	// return a fd number
	return fd;
}

// is_tracked checks if the given fd is tracked by the tracker
// this means that it corresponds to a file which is packed
bool is_tracked(int fd) {
	return get_tracked(fd) != 0;
}

// get_tracked returns the track_entry, if the given fd is tracked
track_entry* get_tracked(int fd) {
	for(int i = 0; i < track_array_size; i++) {
		if(track_array[i].fd == fd && !track_array[i].is_closed) {
			return &track_array[i];
		}
	}
	return NULL;
}

// stop_track marks the given fd as closed
void stop_track(int fd) {
	track_entry* te = get_tracked(fd);
	if(te != 0) {
		te->is_closed = true;
	}
}

// list_tracked prints a list of all tracked FDs to stdout
void list_tracked() {
	char* env_enable_list = getenv("LIBRUCKSACK_ENABLE_TRACKLIST");
	if(env_enable_list == 0 || strcmp(env_enable_list, "1") != 0) {
		// do not list anything
		return;
	}

	printf("+-Path-------------------------------------------------------+-State--+-FD-+\n");
	for(int i = 0; i < track_array_size; i++) {
		track_entry te = track_array[i];
		printf("| %-58.58s | %s | %.2i |\n", te.path, te.is_closed ? "closed" : "active", te.fd);
	}
	printf("+------------------------------------------------------------+--------+----+\n");
}

