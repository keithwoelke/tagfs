#include <sys/stat.h>

int set_filePath(struct stat *buf) {
	return stat("/home/keith/Desktop/test", buf);
}

void set_tagPath(struct stat *buf) {
		buf->st_mode = S_IFDIR | 0755;
		buf->st_nlink = 2;
}
