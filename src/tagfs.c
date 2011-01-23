/**
 * TagFS filesystem. Allows a user to browse their files through tags instead of the traditional hierarchical method. Implemented using Filesystem in Userspace (FUSE).
 * @file tagfs.h
 * @author Keith Woelke
 * @date 07/25/2010
 */

#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <string.h>

static int tagfs_getattr(const char *path, struct stat *buf) {
	int retstat = 0;

//	time_t rawtime;
//	rawtime = time(NULL);
//	char *time = ctime(&rawtime);
//	time[strlen(time) - 1] = '\0';
//
//	INFO("Retrieving attributes for %s", path);
//
//	if (valid_path_to_file(path)) {
//		retstat = stat(get_file_location(path), buf);
//	}
//	else if(valid_path_to_tag(path)) {
//		buf->st_mode = S_IFDIR | 0755;
//	}
//	else {
//		return -ENOENT;
//	}

	return retstat;
} /* tagfs_getattr */

static int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	INFO("Reading directory: %s", path);

	db_create_table();

//	char **file_array = NULL;
//	char **tag_array = NULL;
//	int i = 0;
//	int num_files = db_files_from_restricted_query(path, &file_array);
//	int num_tags = db_tags_from_query(path, &tag_array);
//
//	filler(buf, ".", NULL, 0);
//	filler(buf, "..", NULL, 0);
//
//	for(i = 0; i < num_files; i++) {
//		filler(buf, file_array[i], NULL, 0);
//	}
//
//	for(i = 0; i < num_tags; i++) {
//		filler(buf, tag_array[i], NULL, 0);
//	}
//
//	free_char_ptr_array(&file_array, num_files);
//	free_char_ptr_array(&tag_array, num_tags);

	db_delete_table();

	return 0;
} /* tagfs_readdir */

//int tagfs_unlink(const char *path) {
//	db_delete_file(path);
//
//	return 0;
//}
//
//int tagfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
//	int retstat = 0;
//
//	int fd = open(get_file_location(path), O_RDONLY);
//	retstat = pread(fd, buf, size, offset);
//
//	return retstat;
//}

static struct fuse_operations tagfs_oper = {
	.getattr = tagfs_getattr,
	.readdir = tagfs_readdir,
//	.unlink = tagfs_unlink,
//	.read = tagfs_read,
};

int main(int argc, char *argv[])
{
	char *full_db_path = NULL;
	int path_length = 0;
	int retstat = 0;
	struct tagfs_state tagfs_data;
	tagfs_data.dbpath = NULL;
	tagfs_data.execdir = NULL;
	tagfs_data.logfile = NULL;
	tagfs_data.logpath = NULL;

	debug_init(); /* initialize debug semaphore */

	printf("Initializing TagFS...\n");

	/* executable directory */
	tagfs_data.execdir = get_file_directory(realpath(argv[0], NULL)); /* get full path to executable directory and remove everything after last / */
	assert(tagfs_data.execdir != NULL);

	/* log path */
	path_length = strlen(tagfs_data.execdir) + strlen("log.txt"); 
	assert(path_length > 0);
	tagfs_data.logpath = malloc(path_length * sizeof(*tagfs_data.logpath) + 1);
	assert(tagfs_data.logpath != NULL);
	(void)snprintf(tagfs_data.logpath, path_length + 1, "%slog.txt", tagfs_data.execdir);
	tagfs_data.logfile = debug_open_log(tagfs_data.logpath); /* full path to log file */
	assert(tagfs_data.logfile != NULL);

	/* db path */
	path_length = strlen(tagfs_data.execdir) + strlen("tagfs.sl3"); 
	assert(path_length > 0);
	tagfs_data.dbpath = malloc(path_length * sizeof(*full_db_path) + 1);
	assert(tagfs_data.dbpath != NULL);
	(void)snprintf(tagfs_data.dbpath, path_length + 1, "%stagfs.sl3", tagfs_data.execdir);

	retstat = fuse_main(argc, argv, &tagfs_oper, &tagfs_data);

	debug_close_log(tagfs_data.logfile);

	assert(tagfs_data.execdir != NULL);
	free(tagfs_data.execdir);
	tagfs_data.execdir = NULL;

	assert(tagfs_data.dbpath != NULL);
	free(tagfs_data.dbpath);
	tagfs_data.dbpath = NULL;

	assert(tagfs_data.logpath != NULL);
	free(tagfs_data.logpath);
	tagfs_data.logpath = NULL;

	return retstat;
} /* main */
