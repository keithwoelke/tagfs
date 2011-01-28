/**
 * TagFS Filesystem. Allows a user to browse their files through tags instead of the traditional hierarchical method. Implemented using Filesystem in USErspace (FUSE).
 * @file tagfs.c
 * @author Keith Woelke
 * @date 07/25/2010
 */

#include "tagfs_debug.h"
#include "tagfs_params.h"

#include <errno.h>

int tagfs_getattr(const char *path, struct stat *statbuf) {
	int retstat = 0;

	INFO("Retrieving attributes for %s", path);

	if (valid_path_to_file(path)) {
		retstat = stat(get_file_location(path), statbuf);
	}
	else if(valid_path_to_tag(path)) {
		statbuf->st_mode = S_IFDIR | 0755;
	}
	else {
		retstat = -ENOENT;
	}

	return retstat;
} /* tagfs_getattr */

int tagfs_readlink(const char *path, char *link, size_t size) {
	int retstat = 0;

	INFO("TODO: %s", __FUNCTION__);

	return retstat;
} /* tagfs_readlink */

int tagfs_mknod(const char *path, mode_t mode, dev_t dev) {
	int retstat = 0;

	INFO("TODO: %s", __FUNCTION__);

	return retstat;
} /* tagfs_mknod */

static int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;

	INFO("TODO: %s", __FUNCTION__);

	return retstat;
} /* tagfs_readdir */

struct fuse_operations tagfs_oper = {
	.getattr = tagfs_getattr,
	.readlink = tagfs_readlink,
	.mknod = tagfs_mknod,
//	.mkdir = bb_mkdir,
//	.unlink = bb_unlink,
//	.rmdir = bb_rmdir,
//	.symlink = bb_symlink,
//	.rename = bb_rename,
//	.link = bb_link,
//	.chmod = bb_chmod,
//	.chown = bb_chown,
//	.truncate = bb_truncate,
//	.utime = bb_utime,
//	.open = bb_open,
//	.read = bb_read,
//	.write = bb_write,
//	/** Just a placeholder, don't set */ // huh???
//	.statfs = bb_statfs,
//	.flush = bb_flush,
//	.release = bb_release,
//	.fsync = bb_fsync,
//	.setxattr = bb_setxattr,
//	.getxattr = bb_getxattr,
//	.listxattr = bb_listxattr,
//	.removexattr = bb_removexattr,
//	.opendir = bb_opendir,
	.readdir = tagfs_readdir,
//	.releasedir = bb_releasedir,
//	.fsyncdir = bb_fsyncdir,
//	.init = bb_init,
//	.destroy = bb_destroy,
//	.access = bb_access,
//	.create = bb_create,
//	.ftruncate = bb_ftruncate,
//	.fgetattr = bb_fgetattr
}; /* tagfs_oper */

int main(int argc, char *argv[]) {
	struct tagfs_state tagfs_data;

	printf("Initializing TagFS Filesystem...\n");

	debug_init();

	tagfs_data.logfile = fopen("logfile.txt", "w");

	return fuse_main(argc, argv, &tagfs_oper, &tagfs_data);
} /* main */
