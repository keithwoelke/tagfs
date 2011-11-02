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

#include <assert.h>
#include <errno.h>
#include <fuse.h>
#include <string.h>

/*
 * Get file attributes.
 *
 * Similar to stat(). The 'st_dev' and 'st_blksize' fields are ignored. 
 * The 'st_ino' field is ignored except if the 'use_ino' mount option is given.
 */
int tagfs_getattr(const char *path, struct stat *statbuf) {
	char *file_location = NULL;
	int file_id = 0;
	int retstat = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	INFO("Retrieving attributes for %s", path);

	if (valid_path_to_file(path)) {
		file_id = file_id_from_path(path);
		file_location = db_get_file_location(file_id);

		/* read information from actual file */
		retstat = stat(file_location, statbuf);

		free_single_ptr((void **)&file_location);
	}
	else if(valid_path_to_folder(path)) {
		statbuf->st_mode = S_IFDIR | 0755; /* TODO: Set hard links, etc. */
	}
	else {
		retstat = -ENOENT;
	}

	DEBUG(EXIT);
	return retstat;
} /* tagfs_getattr */

int tagfs_readlink(const char *path, char *link, size_t size) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_mknod(const char *path, mode_t mode, dev_t dev) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_mkdir(const char *path, mode_t mode) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_unlink(const char *path) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_rmdir(const char *path) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_symlink(const char *path, const char *link) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_rename(const char *path, const char *newpath) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_link(const char *path, const char *newpath) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_chmod(const char *path, mode_t mode) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_chown(const char *path, uid_t uid, gid_t gid) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_truncate(const char *path, off_t newsize) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_utime(const char *path, struct utimbuf *ubuf) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_open(const char *path, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_statfs(const char *path, struct statvfs *statv) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_flush(const char *path, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_release(const char *path, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_getxattr(const char *path, const char *name, char *value, size_t size) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_listxattr(const char *path, char *list, size_t size) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_removexattr(const char *path, const char *name) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_opendir(const char *path, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

/**
 * Read directory
 *
 * This supersedes the old getdir() interface. New applications should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and passes zero 
 * to the filler function's offset. The filler function will not return '1' 
 * (unless an error happens), so the whole directory is read in a single readdir * operation. This works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the directory 
 * entries. It uses the offset parameter and always passes non-zero offset to 
 * the filler function. When the buffer is full (or an error happens) the filler * function will return '1'.
 *
 * Introduced in version 2.3
 */ 
int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	char **path_array = NULL;
	char *file_name = NULL;
	char *folder_name = NULL;
	int *files = NULL;
	int *folders = NULL;
	int file_id = 0;
	int filler_ret = 0;
	int i = 0;
	int num_files = 0;
	int num_folders = 0;
	int path_count = 0;
	int retstat = 0;
	int tag_id = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	INFO("Reading directory %s", path);

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	/* add files */
	num_files = files_at_location(path, &files);

	if(num_files > 0) {
		for(i = 0; i < num_files; i++) {
			file_id = files[i];
			file_name = file_name_from_id(file_id);

			filler_ret = filler(buf, file_name, NULL, 0);
			if(filler_ret != 0) {
				DEBUG("filler returned %d", filler_ret);
				WARN("An error occured while loading files. Out of memory?");
				retstat = -ENOMEM;
			}

			free_single_ptr((void **)&file_name);
		}
	}

	/* add folders */
	num_folders = folders_at_location(path, files, num_files, &folders);
	free_single_ptr((void **)&files);

	if(num_folders > 0) {
		path_count = path_to_array(path, &path_array);

		for(i = 0; i < num_folders; i++) {
			tag_id = folders[i];
			folder_name = tag_name_from_tag_id(tag_id);

			/* filter tags out of path */
			if(!array_contains_string((const char **)path_array, folder_name, path_count)) {
				filler_ret = filler(buf, folder_name, NULL, 0);
			}
			
			if(filler_ret != 0) {
				DEBUG("filler returned %d", filler_ret);
				WARN("An error occured while loading tags. Out of memory?");
				retstat = -ENOMEM;
			}

			free_single_ptr((void **)&folder_name);
		}

		free_single_ptr((void **)&folders);

		if(path_array != NULL) {
			free_double_ptr((void ***)&path_array, path_count);
		}
	}

	DEBUG(EXIT);
	return retstat;
} /* tagfs_readdir */

int tagfs_releasedir(const char *path, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of fuse_context to all
 * file operations and as a parameter to the destroy() method.
 *
 * Introduced in version 2.3 Changed in version 2.6
 */
void *tagfs_init(struct fuse_conn_info *conn) {
	const char *db_name = "tagfs.sl3";
	const char *log_name = "log_file.txt";
	const char *log_path = NULL;
	int db_dir_length = 0;
	int dir_length = 0;
	int log_dir_length = 0;
	int written = 0; /* number of characters written by snprintf */

	/* get directory length */
	dir_length = strlen(TAGFS_DATA->exec_dir) + strlen("/");

	/* set log path */
	log_dir_length = strlen(log_name) + dir_length;
	log_path = malloc(log_dir_length * sizeof(*log_path) + 1);
	assert(log_path != NULL);
	written = snprintf((char *)log_path, log_dir_length + 1, "%s/%s", TAGFS_DATA->exec_dir, log_name);
	assert(written == log_dir_length);

	printf("Initializing TagFS Filesystem...\n");
	printf("Opening log file: %s\n", log_path);
	TAGFS_DATA->log_file = fopen(log_path, "w");

	DEBUG(ENTRY);

	/* set database path */
	db_dir_length = strlen(db_name) + dir_length;
	TAGFS_DATA->db_path = malloc(db_dir_length * sizeof(*TAGFS_DATA->db_path) + 1);
	written = snprintf((char *)TAGFS_DATA->db_path, db_dir_length + 1, "%s/%s", TAGFS_DATA->exec_dir, db_name);
	assert(written == db_dir_length);
	
	free_single_ptr((void **)&log_path);

	DEBUG(EXIT);
	return TAGFS_DATA;
} /* tagfs_init */

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */
 void tagfs_destroy(void *userdata) {
	DEBUG(ENTRY);
	struct tagfs_state *tagfs_data = (struct tagfs_state *)userdata;

	DEBUG("Finalizing data...");

	free_single_ptr((void **)&tagfs_data->exec_dir);
	free_single_ptr((void **)&tagfs_data->db_path);

	DEBUG(EXIT);

	assert(tagfs_data->log_file != NULL);
	fclose(tagfs_data->log_file);
	tagfs_data->log_file = NULL;
} /* tagfs_destroy */

int tagfs_access(const char *path, int mask) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

int tagfs_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi) {
	int retstat = 0;

	DEBUG(ENTRY);

	ERROR("TODO: %s", __FUNCTION__);

	DEBUG(EXIT);
	return retstat;
}

/* TODO: Implement these */
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
	/* .open = tagfs_open, */
	/* .read = tagfs_read, */
	.write = tagfs_write,
	/* .statfs = tagfs_statfs, */
	/* .flush = tagfs_flush, */
	/* .release = tagfs_release, */
	.fsync = tagfs_fsync,
	.setxattr = tagfs_setxattr,
	/* .getxattr = tagfs_getxattr, */
	.listxattr = tagfs_listxattr,
	.removexattr = tagfs_removexattr,
	/* .opendir = tagfs_opendir, */
	.readdir = tagfs_readdir,
	/* .releasedir = tagfs_releasedir, */
	.fsyncdir = tagfs_fsyncdir,
	.init = tagfs_init,
	.destroy = tagfs_destroy,
	/* .access = tagfs_access, */
	/* .create = tagfs_create, */
	.ftruncate = tagfs_ftruncate,
	/* .fgetattr = tagfs_fgetattr */
};

int main(int argc, char *argv[]) {
	struct tagfs_state tagfs_data;

	debug_init();
	tagfs_data.exec_dir = get_exec_dir(argv[0]);

	return fuse_main(argc, argv, &tagfs_oper, &tagfs_data);
} /* main */
