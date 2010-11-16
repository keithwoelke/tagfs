#include <sys/stat.h>

int set_filePath(const char *path, struct stat *buf) {
	return stat(path, buf);
}

void set_tagPath(struct stat *buf) {
		buf->st_mode = S_IFDIR | 0755;
		buf->st_nlink = 2;
}
