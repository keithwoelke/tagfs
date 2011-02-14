/**
 * TagFS Filesystem. Allows a user to browse their files through tags instead of the traditional hierarchical method. Implemented using Filesystem in USErspace (FUSE).
 *
 * @file tagfs.c
 * @author Keith Woelke
 * @date 07/25/2010
 */

#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"
#include "tagfs_params.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

int tagfs_getattr(const char *path, struct stat *statbuf) {
	DEBUG(ENTRY);
	int retstat = 0;

	INFO("Retrieving attributes for %s", path);

	db_set_directory_contents("/Video/ogg", "directory_contents"); /* TODO: This is just for testing purposes */
	exit(EXIT_SUCCESS);

	/* TODO: This all needs to be uncommented/implemented at some point */
/*	if (valid_path_to_file(path)) {
		retstat = stat(db_get_file_location(path), statbuf);
	}
	else if(valid_path_to_tag(path)) {*/ /* TODO: Set proper folder info? */
		statbuf->st_mode = S_IFDIR | 0755;
//	}
//	else {
//		retstat = -ENOENT;
//	}

	DEBUG(EXIT);
	return retstat;
} /* tagfs_getattr */

int tagfs_readlink(const char *path, char *link, size_t size) {
	DEBUG(ENTRY);
	int retstat = 0;

	INFO("TODO: %s", __FUNCTION__); /* TODO: This needs to be implemented */

	DEBUG(EXIT);
	return retstat;
} /* tagfs_readlink */

int tagfs_mknod(const char *path, mode_t mode, dev_t dev) {
	DEBUG(ENTRY);
	int retstat = 0;

	INFO("TODO: %s", __FUNCTION__); /* TODO: This needs to be implemented */

	DEBUG(EXIT);
	return retstat;
} /* tagfs_mknod */

static int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	INFO("Reading directory %s", path);

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	/* TODO: Fill files and folders */

	DEBUG(EXIT);
	return retstat;
} /* tagfs_readdir */

void *tagfs_init(struct fuse_conn_info *conn) {
	const char *db_name = "tagfs.sl3"; /* TODO: Does this always place in current directory? */
	const char *log_name = "log_file.txt";

	printf("Initializing TagFS Filesystem...\n");
	printf("Opening log file: %s\n", log_name);
	TAGFS_DATA->log_file = fopen("log_file.txt", "w"); /* TODO: Fix so this does not place in working directory */

	DEBUG(ENTRY);

	TAGFS_DATA->db_path = malloc(strlen(db_name) * sizeof(*TAGFS_DATA->db_path) + 1);
	snprintf(TAGFS_DATA->db_path, strlen(db_name) + 1, "%s", db_name);

	db_connect();

	DEBUG(EXIT);
	return TAGFS_DATA;
} /* tagfs_init */

void tagfs_destroy(void *userdata) {
	DEBUG(ENTRY);
	struct tagfs_state *tagfs_data = (struct tagfs_state *)userdata;

	DEBUG("Finalizing data...");

	assert(tagfs_data->db_conn != NULL);
	db_disconnect();
	tagfs_data->db_conn = NULL;
	
	assert(tagfs_data->db_path != NULL);
	free(tagfs_data->db_path);
	tagfs_data->db_path = NULL;

	DEBUG(EXIT);

	assert(tagfs_data->log_file != NULL);
	fclose(tagfs_data->log_file);
	tagfs_data->log_file = NULL;
} /* tagfs_destroy */

struct fuse_operations tagfs_oper = {
	.getattr = tagfs_getattr,
	.readlink = tagfs_readlink,
	.mknod = tagfs_mknod,
//	.mkdir = bb_mkdir, /* TODO: Stub/implement this */
//	.unlink = bb_unlink, /* TODO: Stub/implement this */
//	.rmdir = bb_rmdir, /* TODO: Stub/implement this */
//	.symlink = bb_symlink, /* TODO: Stub/implement this */
//	.rename = bb_rename, /* TODO: Stub/implement this */
//	.link = bb_link, /* TODO: Stub/implement this */
//	.chmod = bb_chmod, /* TODO: Stub/implement this */
//	.chown = bb_chown, /* TODO: Stub/implement this */
//	.truncate = bb_truncate, /* TODO: Stub/implement this */
//	.utime = bb_utime, /* TODO: Stub/implement this */
//	.open = bb_open, /* TODO: Stub/implement this */
//	.read = bb_read, /* TODO: Stub/implement this */
//	.write = bb_write, /* TODO: Stub/implement this */
//	.statfs = bb_statfs, /* TODO: Stub/implement this */
//	.flush = bb_flush, /* TODO: Stub/implement this */
//	.release = bb_release, /* TODO: Stub/implement this */
//	.fsync = bb_fsync, /* TODO: Stub/implement this */
//	.setxattr = bb_setxattr, /* TODO: Stub/implement this */
//	.getxattr = bb_getxattr, /* TODO: Stub/implement this */
//	.listxattr = bb_listxattr, /* TODO: Stub/implement this */
//	.removexattr = bb_removexattr, /* TODO: Stub/implement this */
//	.opendir = bb_opendir, /* TODO: Stub/implement this */
	.readdir = tagfs_readdir,
//	.releasedir = bb_releasedir, /* TODO: Stub/implement this */
//	.fsyncdir = bb_fsyncdir, /* TODO: Stub/implement this */
	.init = tagfs_init,
	.destroy = tagfs_destroy,
//	.access = bb_access, /* TODO: Stub/implement this */
//	.create = tagfs_create, /* TODO: Stub/implement this */
//	.ftruncate = bb_ftruncate, /* TODO: Stub/implement this */
//	.fgetattr = bb_fgetattr /* TODO: Stub/implement this */
}; /* tagfs_oper */

int main(int argc, char *argv[]) {
	struct tagfs_state tagfs_data;

	return fuse_main(argc, argv, &tagfs_oper, &tagfs_data);
} /* main */
