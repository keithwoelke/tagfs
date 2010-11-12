#define FUSE_USE_VERSION 26

#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <errno.h>
#include <fuse.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int tagfs_getattr(const char *path, struct stat *buf)
{
	int retstat = 0;

	if (valid_path_to_file(path)) {
		retstat = stat(get_file_location(path), buf);
	}
	else if(valid_path_to_tag(path)) {
		buf->st_mode = S_IFDIR | 0755;
		buf->st_nlink = 2;
	}
	else {
		return -ENOENT;
	}

	return retstat;
}

static int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	char **file_array = NULL;
	char **tag_array = NULL;
	int i = 0;
	int num_files = db_files_from_restricted_query(path, &file_array);
	int num_tags = db_tags_from_query(path, &tag_array);

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	for(i = 0; i < num_files; i++) {
		filler(buf, file_array[i], NULL, 0);
	}

	for(i = 0; i < num_tags; i++) {
		filler(buf, tag_array[i], NULL, 0);
	}

	free_char_ptr_array(&file_array, num_files);
	free_char_ptr_array(&tag_array, num_tags);

	return 0;
}

int tagfs_unlink(const char *path) {
	db_delete_file(path);

	return 0;
}

int tagfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;

	int fd = open(get_file_location(path), O_RDONLY);
	retstat = pread(fd, buf, size, offset);

	return retstat;
}

static struct fuse_operations tagfs_oper = {
	.getattr = tagfs_getattr,
	.readdir = tagfs_readdir,
	.unlink = tagfs_unlink,
	.read = tagfs_read,
};

int main(int argc, char *argv[])
{
	debug_init();

	return fuse_main(argc, argv, &tagfs_oper, NULL);
}
