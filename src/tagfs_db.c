#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <string.h>

const char *db_get_file_location(int file_id) {
	DEBUG(ENTRY);
	char *path = NULL;

	if(file_id == 1) {
		path = malloc(strlen("/home/keith/Programming/FUSE/tagfs/src/Files/How fast.ogg") + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/tagfs/src/Files/How fast.ogg") + 1, "%s", "/home/keith/Programming/FUSE/tagfs/src/Files/How fast.ogg");
		DEBUG("file id is %d\n", 1);
		DEBUG(EXIT);
		return path;
	}
	else if(file_id == 2) {
		path = malloc(strlen("/home/keith/Programming/FUSE/tagfs/src/Files/Josh Woodward - Swansong.ogg") + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/tagfs/src/Files/Josh Woodward - Swansong.ogg") + 1, "%s", "/home/keith/Programming/FUSE/tagfs/src/Files/Josh Woodward - Swansong.ogg");
		DEBUG("file id is %d\n", 2);
		DEBUG(EXIT);
		return path;
	}
	else if(file_id == 3) {
		path = malloc(strlen("/home/keith/Programming/FUSE/tagfs/src/Files/sample_iTunes.mov") + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/tagfs/src/Files/sample_iTunes.mov") + 1, "%s", "/home/keith/Programming/FUSE/tagfs/src/Files/sample_iTunes.mov");
		DEBUG("file id is %d\n", 3);
		DEBUG(EXIT);
		return path;
	}
	else if(file_id == 4) {
		path = malloc(strlen("/home/keith/Programming/FUSE/tagfs/src/Files/Moby Dick.txt") + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/tagfs/src/Files/Moby Dick.txt") + 1, "%s", "/home/keith/Programming/FUSE/tagfs/src/Files/Moby Dick.txt");
		DEBUG("file id is %d\n", 3);
		DEBUG(EXIT);
		return path;
	}
	else {
		DEBUG("%d is not recognized.", file_id);
		return path;
	}
}
