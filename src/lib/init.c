#define _GNU_SOURCE

#include "../common/pack_util.h"
#include "patch_unistd.h"
#include "patch_stat.h"

#include <stdlib.h>
#include <string.h>

__attribute__((constructor))
void _librucksack_init() {
	// Use a constructor so this function will be executed before main()

	_unistd_get_original_links();

	char* env_enable_list = getenv("LIBRUCKSACK_ENABLE_FILELIST");
	if(env_enable_list != NULL && strcmp(env_enable_list, "1") == 0) {
		list_pack_entries();
		return;
	}
}
