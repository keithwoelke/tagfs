#include "tagfs_common.h"
#include "tagfs_debug.h"
#include "tagfs_db.h"

const char *get_exec_dir(const char *exec_name) {
	const char *exec_dir = NULL;

	exec_dir = realpath(exec_name, NULL);

	return exec_dir;
} /* get_exec_dir */
