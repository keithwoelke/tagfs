module testfuse;

import std.string;
import std.c.stdlib;
import std.c.string;
import core.stdc.errno;


import fuse;
import fuse_common;
import fuse_opt;

char *hello_str; 
char *hello_path;


int main(string[] args)
{
	//char*[] argv;
	//hello_str = cast(char*) toStringz("Hello World!\n");
	//hello_path = cast(char*) toStringz("/hello");
	//foreach(arg; args) {
//		argv ~= cast(char*) toStringz(arg);
//	}
 //   return fuse_main(argv.length, argv.ptr, &hello_oper);
 return 0;
}

extern (C):

static int hello_getattr(const char *path, stat_t *stbuf)
{
    int res = 0;

    memset(stbuf, 0, stat_t.sizeof);
    if(strcmp(path, "/") == 0) {
        stbuf.st_mode = S_IFDIR | 0755;
        stbuf.st_nlink = 2;
    }
    else if(strcmp(path, hello_path) == 0) {
        stbuf.st_mode = S_IFREG | 0444;
        stbuf.st_nlink = 1;
        stbuf.st_size = strlen(hello_str);
    }
    else
        res = -ENOENT;

    return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, fuse_file_info *fi)
{
    cast(void) offset;
    cast(void) fi;

    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".".dup.ptr, null, 0);
    filler(buf, "..".dup.ptr, null, 0);
    filler(buf, hello_path + 1, null, 0);

    return 0;
}

static int hello_open(const char *path, fuse_file_info *fi)
{
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    if((fi.flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      fuse_file_info *fi)
{
    size_t len;
    cast(void) fi;
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    len = strlen(hello_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, hello_str + offset, size);
    } else
        size = 0;

    return size;
}

static fuse_operations hello_oper = {
    getattr:&hello_getattr,
    readdir:&hello_readdir,
    open	:&hello_open,
    read	:&hello_read,
};
