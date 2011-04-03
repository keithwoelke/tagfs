/**
 * TagFS Filesystem. Allows a user to browse their files through tags instead of the traditional hierarchical method. Implemented using Filesystem in USErspace (FUSE).
 *
 * @file tagfs.c
 * @author Keith Woelke
 * @date 07/25/2010
 */

#include "tagfs_params.h"

#include <fuse.h>

int tagfs_getattr(const char *path, struct stat *statbuf) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_readlink(const char *path, char *link, size_t size) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_mknod(const char *path, mode_t mode, dev_t dev) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_mkdir(const char *path, mode_t mode) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_unlink(const char *path) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_rmdir(const char *path) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_symlink(const char *path, const char *link) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_rename(const char *path, const char *newpath) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_link(const char *path, const char *newpath) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_chmod(const char *path, mode_t mode) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_chown(const char *path, uid_t uid, gid_t gid) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_truncate(const char *path, off_t newsize) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_utime(const char *path, struct utimbuf *ubuf) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_open(const char *path, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_statfs(const char *path, struct statvfs *statv) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_flush(const char *path, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_release(const char *path, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_getxattr(const char *path, const char *name, char *value, size_t size) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_listxattr(const char *path, char *list, size_t size) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_removexattr(const char *path, const char *name) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_opendir(const char *path, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_releasedir(const char *path, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

void *tagfs_init(struct fuse_conn_info *conn) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

void tagfs_destroy(void *userdata) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_access(const char *path, int mask) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi) {
	DEBUG(ENTRY);
	int retstat = 0;

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

struct fuse_operations tagfs_oper = {
  .getattr = tagfs_getattr,
  .readlink = tagfs_readlink,
  .mknod = tagfs_mknod,
  .mkdir = tagfs_mkdir,
  .unlink = tagfs_unlink,
  .rmdir = tagfs_rmdir,
  .symlink = tagfs_symlink,
  .rename = tagfs_rename,
  .link = tagfs_link,
  .chmod = tagfs_chmod,
  .chown = tagfs_chown,
  .truncate = tagfs_truncate,
  .utime = tagfs_utime,
  .open = tagfs_open,
  .read = tagfs_read,
  .write = tagfs_write,
  .statfs = tagfs_statfs,
  .flush = tagfs_flush,
  .release = tagfs_release,
  .fsync = tagfs_fsync,
  .setxattr = tagfs_setxattr,
  .getxattr = tagfs_getxattr,
  .listxattr = tagfs_listxattr,
  .removexattr = tagfs_removexattr,
  .opendir = tagfs_opendir,
  .readdir = tagfs_readdir,
  .releasedir = tagfs_releasedir,
  .fsyncdir = tagfs_fsyncdir,
  .init = tagfs_init,
  .destroy = tagfs_destroy,
  .access = tagfs_access,
  .create = tagfs_create,
  .ftruncate = tagfs_ftruncate,
  .fgetattr = tagfs_fgetattr
};

int main(int argc, char *argv[]) {
	struct tagfs_state tagfs_data;

	return fuse_main(argc, argv, &tagfs_oper, &tagfs_data);
} /* main */
