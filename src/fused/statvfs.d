module core.sys.posix.statvfs;

import core.sys.posix.sys.types;

struct fsid_t {
	int[2] val;
}

struct statvfs {
	ulong f_bsize;
	ulong f_frsize;
	fsblkcnt_t f_blocks;
	fsblkcnt_t f_bfree;
	fsblkcnt_t f_bavail;
	fsfilcnt_t f_files;
	fsfilcnt_t f_ffree;
	fsfilcnt_t f_favail;
	fsid_t f_fsid;
	ulong f_flag;
	ulong f_namemax;
	uint f_spare[6];
}; 
