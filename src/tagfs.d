module tagfs;

import tagfs_db;
import tagfs_common;
import tagfs_debug;
import std.stdio;

import fuse;
import std.c.stdio;
import std.c.string;
static import std.string;
import core.stdc.errno;
import std.c.stdlib;

extern(C) {
int set_filePath(stat_t *buf);
void set_tagPath(stat_t *buf);
	
int tagfs_getattr(const char *path, stat_t *buf)
{
	int retstat = 0;

	if (valid_path_to_file(path)) {
		writeln("Setting file");
		retstat = set_filePath(buf);
	}
	else if(valid_path_to_tag(path)) {
//		printf("2");
		writeln("tag");
		set_tagPath(buf);
	}
	else {
		writeln("Bad me");
//		printf("3");
		return -ENOENT;
	}

	return retstat;

//	int res = 0;
//
//	memset(stbuf, 0, sizeof(struct stat));
//	if (valid_path_to_tag(path)) {
//		stbuf->st_mode = S_IFDIR | 0755;
//		stbuf->st_nlink = 2;
//	}
//	else {
//		stbuf->st_mode = S_IFREG | 0444;
//		stbuf->st_nlink = 1;
//	}
//
//	return res;
}

static int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi)
{
//	printf("readdir");
	char **file_array = null;
	char **tag_array = null;
	int i = 0;
	int num_files = db_files_from_query(path, &file_array);
	int num_tags = db_tags_from_query(path, &tag_array);

	filler(buf, ".", null, 0);
	filler(buf, "..", null, 0);

	for(i = 0; i < num_files; i++) {
//		DEBUG(DL_NORMAL, "Adding file: %s", file_array[i]);
		filler(buf, file_array[i], null, 0);
	}

	for(i = 0; i < num_tags; i++) {
		filler(buf, tag_array[i], null, 0);
	}

	free_char_ptr_array(&file_array, num_files);
	free_char_ptr_array(&tag_array, num_tags);

	return 0;
}

int tagfs_unlink(const char *path) {
	printf("%s\n", path);
	db_delete_file(path);

	return 0;
}

//int tagfs_opendir(const char *path, struct fuse_file_info *fi) {
//	return 0;
//}

static fuse_operations tagfs_oper; /* = {
	.getattr = tagfs_getattr,
	.readdir = tagfs_readdir,
	.unlink = tagfs_unlink,
//	.opendir = tagfs_opendir,
};
*/
}

extern(D):
int main(string[] args)
{
	debug_init();
	tagfs_oper.getattr = &tagfs_getattr;
	tagfs_oper.readdir = &tagfs_readdir;
	tagfs_oper.unlink = &tagfs_unlink;

	char*[] argv;
	foreach(arg; args) {
		argv ~= (arg ~ "\0").dup.ptr;
	}
	//return fuse_main(argc, argv, &tagfs_oper, null);
	return fuse_main(argv.length, argv.ptr, &tagfs_oper);
}
