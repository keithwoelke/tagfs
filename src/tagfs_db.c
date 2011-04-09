#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <string.h>

const char *db_get_file_location(int file_id) {
	DEBUG(ENTRY);

	DEBUG(EXIT);
}

//const char *db_get_file_location(int file_id) {
//	DEBUG(ENTRY);
//	char *path = NULL;
//
//	if(file_id == 1) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg");
//		DEBUG("file id is %d for path %s.\n", 1, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else if(file_id == 2) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg");
//		DEBUG("file id is %d for path %s.\n", 2, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else if(file_id == 3) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov");
//		DEBUG("file id is %d for path %s.\n", 3, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else if(file_id == 4) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt");
//		DEBUG("file id is %d for path %s.\n", 4, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else {
//		DEBUG("%d is not recognized.", file_id);
//		return path;
//	}
//}
