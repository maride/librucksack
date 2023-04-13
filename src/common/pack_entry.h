#ifndef __pack_entry_h__
#define __pack_entry_h__

#include <stddef.h>

#define MAXPATHLEN 256

typedef struct {
	char path[MAXPATHLEN];
	size_t size;
	void* data;
} pack_entry;

#endif // __pack_entry_h__
