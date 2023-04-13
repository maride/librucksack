#include "pack_util.h"

#include <string.h>
#include <stdio.h>

int sizeof_pack_entry(const pack_entry* p) {
	return sizeof(size_t) + MAXPATHLEN * sizeof(char) + p->size;
}

// get_pack_entry iterates over the packed binary to search for path's data
pack_entry* get_pack_entry(const char* path) {
	void* pack_start = &_binary_pack_start;
	void* pack_end = &_binary_pack_end;

	void* cur_ptr = pack_start;
	while((void*)cur_ptr < pack_end) {
		pack_entry* cur_ptr_pe = (pack_entry*)cur_ptr;
		if(strcmp((const char*)cur_ptr_pe->path, path) == 0) {
			// found the right entry, return it
			return cur_ptr;
		}
		// the search continues
		cur_ptr += sizeof_pack_entry(cur_ptr);
	}

	// none found, none to return
	return NULL;
}

// list_pack_entries prints out a list of pack entries compiled into the binary
void list_pack_entries() {
	void* pack_start = &_binary_pack_start;
	void* pack_end = &_binary_pack_end;

	printf("+-Packed Path------------------------------------------------------+-Size----+\n");

	void* cur_ptr = pack_start;
	while((void*)cur_ptr < pack_end) {
		pack_entry* cur_ptr_pe = (pack_entry*)cur_ptr;
		printf("| %-64.64s | %.7li |\n", (char*)&cur_ptr_pe->path, cur_ptr_pe->size);
		cur_ptr += sizeof_pack_entry(cur_ptr_pe);
	}
	printf("+------------------------------------------------------------------+---------+\n");
}
