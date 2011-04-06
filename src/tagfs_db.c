#include "tagfs_db.h"
#include "tagfs_debug.h"

const char *db_get_file_location(int file_id) {
	DEBUG(ENTRY);
	if(file_id == 1) {
		DEBUG(EXIT);
		return "/home/keith/Programming/FUSE/tagfs/src/Files/How fast.ogg";
       }
	else if(file_id == 2) {
		DEBUG(EXIT);
		return "/home/keith/Programming/FUSE/tagfs/src/Files/Josh Woodward - Swansong.ogg";
	}
	else {
		DEBUG(EXIT);
		return "/home/keith/Programming/FUSE/tagfs/src/Files/sample_iTunes.mov";
	}
}
