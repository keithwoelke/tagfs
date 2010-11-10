module fuse_helper;

import core.sys.posix.fcntl;
import core.sys.posix.sys.stat;
import core.sys.posix.sys.time;
import core.sys.posix.sys.types;
public import core.stdc.stdint;

/// fuse_lowlevel.h
alias ulong fuse_ino_t;

/// fuse.c
//struct fuse_config {
//	uint uid;
//	uint gid;
//	uint  umask;
//	double entry_timeout;
//	double negative_timeout;
//	double attr_timeout;
//	double ac_attr_timeout;
//	int ac_attr_timeout_set;
//	int noforget;
//	int debug_flag;
//	int hard_remove;
//	int use_ino;
//	int readdir_ino;
//	int set_mode;
//	int set_uid;
//	int set_gid;
//	int direct_io;
//	int kernel_cache;
//	int auto_cache;
//	int intr;
//	int intr_signal;
//	int help;
//	char *modules;
//};
//
///// fuse.c
//struct node {
//	node *name_next;
//	node *id_next;
//	fuse_ino_t nodeid;
//	uint generation;
//	int refctr;
//	node *parent;
//	char *name;
//	uint64_t nlookup;
//	int open_count;
//	timespec stat_updated;
//	timespec mtime;
//	off_t size;
//	lock *locks;
//	//FIXME: This was originally 
//	//  uint is_hidden : 1;
//	//  uint cache_valid : 1;
//   uint __bitfield1;
//	uint is_hidden() { return (__bitfield1 >> 0) & 0x1; }
//	uint is_hidden(uint value) { __bitfield1 = (__bitfield1 & 0xfffffffffffffffe) | (value << 0); return value; }
//	uint cache_valid() { return (__bitfield1 >> 1) & 0x1; }
//	uint cache_valid(uint value) { __bitfield1 = (__bitfield1 & 0xfffffffffffffffd) | (value << 1); return value; }
//
//	int treelock;
//	int ticket;
//};
//
///// fuse.c
//struct lock {
//	int type;
//	off_t start;
//	off_t end;
//	pid_t pid;
//	uint64_t owner;
//	lock *next;
//};
struct lock;
//
///// fuse.c
//struct lock_queue_element {
//       lock_queue_element *next;
//       pthread_cond_t cond;
//};
struct lock_queue_element;
//
///// fuse.c
//struct fusemod_so {
//	void *handle;
//	int ctr;
//};
struct fusemod_so;
