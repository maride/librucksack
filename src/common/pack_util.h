#ifndef __pack_util_h__
#define __pack_util_h__

#include "pack_entry.h"

#include <stdbool.h>

// references to the linked pack.o
extern void *_binary_pack_start;
extern void *_binary_pack_end;

pack_entry* get_pack_entry(const char* path);
void list_pack_entries(); 

#endif // __pack_util_h__
