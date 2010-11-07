/* Converted to D from .\fuse_common.h by htod */
module fuse_common;

/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU LGPLv2.
  See the file COPYING.LIB.
*/

/** @file */

//#if !defined(_FUSE_H_) && !defined(_FUSE_LOWLEVEL_H_)
//#error "Never include <fuse_common.h> directly; use <fuse.h> or <fuse_lowlevel.h> instead."
//#endif
//
//C     #ifndef _FUSE_COMMON_H_
//C     #define _FUSE_COMMON_H_

//C     #include "fuse_opt.h"

public import core.sys.posix.fcntl;
public import core.sys.posix.sys.stat;
public import core.sys.posix.sys.time;
public import core.sys.posix.sys.types;
public import core.sys.posix.inttypes;

public import fuse_opt;
// C #include <stdint.h>

/** Major version of FUSE library interface */
//C     #define FUSE_MAJOR_VERSION 2

const FUSE_MAJOR_VERSION = 2;
/** Minor version of FUSE library interface */
//C     #define FUSE_MINOR_VERSION 8

const FUSE_MINOR_VERSION = 8;
//C     #define FUSE_MAKE_VERSION(maj, min)  ((maj) * 10 + (min))
//C     #define FUSE_VERSION FUSE_MAKE_VERSION(FUSE_MAJOR_VERSION, FUSE_MINOR_VERSION)

/* This interface uses 64 bit off_t */
//C     #if _FILE_OFFSET_BITS != 64
//C     #error Please add -D_FILE_OFFSET_BITS=64 to your compile flags!
//C     #endif

//C     #ifdef __cplusplus
//C     extern "C" {
//C     #endif

/**
 * Information about open files
 *
 * Changed in version 2.5
 */
//C     struct fuse_file_info {
	/** Open flags.	 Available in open() and release() */
//C     	int flags;

	/** Old file handle, don't use */
//C     	unsigned long fh_old;

	/** In case of a write operation indicates if this was caused by a
	    writepage */
//C     	int writepage;

	/** Can be filled in by open, to use direct I/O on this file.
	    Introduced in version 2.4 */
//C     	unsigned int direct_io : 1;

	/** Can be filled in by open, to indicate, that cached file data
	    need not be invalidated.  Introduced in version 2.4 */
//C     	unsigned int keep_cache : 1;

	/** Indicates a flush operation.  Set in flush operation, also
	    maybe set in highlevel lock operation and lowlevel release
	    operation.	Introduced in version 2.6 */
//C     	unsigned int flush : 1;

	/** Can be filled in by open, to indicate that the file is not
	    seekable.  Introduced in version 2.8 */
//C     	unsigned int nonseekable : 1;

	/** Padding.  Do not use*/
//C     	unsigned int padding : 28;

	/** File handle.  May be filled in by filesystem in open().
	    Available in all other file operations */
	//uint64_t fh;

	/** Lock owner id.  Available in locking operations and flush */
	//uint64_t lock_owner;
//C     };
struct fuse_file_info
{
    int flags;
    uint fh_old;
    int writepage;
    uint __bitfield1;
    uint direct_io() { return (__bitfield1 >> 0) & 0x1; }
    uint direct_io(uint value) { __bitfield1 = (__bitfield1 & 0xfffffffffffffffe) | (value << 0); return value; }
    uint keep_cache() { return (__bitfield1 >> 1) & 0x1; }
    uint keep_cache(uint value) { __bitfield1 = (__bitfield1 & 0xfffffffffffffffd) | (value << 1); return value; }
    uint flush() { return (__bitfield1 >> 2) & 0x1; }
    uint flush(uint value) { __bitfield1 = (__bitfield1 & 0xfffffffffffffffb) | (value << 2); return value; }
    uint nonseekable() { return (__bitfield1 >> 3) & 0x1; }
    uint nonseekable(uint value) { __bitfield1 = (__bitfield1 & 0xfffffffffffffff7) | (value << 3); return value; }
    uint padding() { return (__bitfield1 >> 4) & 0xfffffff; }
    uint padding(uint value) { __bitfield1 = (__bitfield1 & 0xffffffff0000000f) | (value << 4); return value; }

	 // FIXME: htoD would not convert this struct with the last two fields.
	 //  WHY? I believe the padding of the original struct makes this
	 //  reasonable to add.
	/** File handle.  May be filled in by filesystem in open().
	    Available in all other file operations */
	uint64_t fh;

	/** Lock owner id.  Available in locking operations and flush */
	uint64_t lock_owner;

}

/**
 * Capability bits for 'fuse_conn_info.capable' and 'fuse_conn_info.want'
 *
 * FUSE_CAP_ASYNC_READ: filesystem supports asynchronous read requests
 * FUSE_CAP_POSIX_LOCKS: filesystem supports "remote" locking
 * FUSE_CAP_ATOMIC_O_TRUNC: filesystem handles the O_TRUNC open flag
 * FUSE_CAP_EXPORT_SUPPORT: filesystem handles lookups of "." and ".."
 * FUSE_CAP_BIG_WRITES: filesystem can handle write size larger than 4kB
 * FUSE_CAP_DONT_MASK: don't apply umask to file mode on create operations
 */
//C     #define FUSE_CAP_ASYNC_READ	(1 << 0)
//C     #define FUSE_CAP_POSIX_LOCKS	(1 << 1)
//C     #define FUSE_CAP_ATOMIC_O_TRUNC	(1 << 3)
//C     #define FUSE_CAP_EXPORT_SUPPORT	(1 << 4)
//C     #define FUSE_CAP_BIG_WRITES	(1 << 5)
//C     #define FUSE_CAP_DONT_MASK	(1 << 6)

/**
 * Ioctl flags
 *
 * FUSE_IOCTL_COMPAT: 32bit compat ioctl on 64bit machine
 * FUSE_IOCTL_UNRESTRICTED: not restricted to well-formed ioctls, retry allowed
 * FUSE_IOCTL_RETRY: retry with new iovecs
 *
 * FUSE_IOCTL_MAX_IOV: maximum of in_iovecs + out_iovecs
 */
//C     #define FUSE_IOCTL_COMPAT	(1 << 0)
//C     #define FUSE_IOCTL_UNRESTRICTED	(1 << 1)
//C     #define FUSE_IOCTL_RETRY	(1 << 2)

//C     #define FUSE_IOCTL_MAX_IOV	256

const FUSE_IOCTL_MAX_IOV = 256;
/**
 * Connection information, passed to the ->init() method
 *
 * Some of the elements are read-write, these can be changed to
 * indicate the value requested by the filesystem.  The requested
 * value must usually be smaller than the indicated value.
 */
//C     struct fuse_conn_info {
	/**
	 * Major version of the protocol (read-only)
	 */
//C     	unsigned proto_major;

	/**
	 * Minor version of the protocol (read-only)
	 */
//C     	unsigned proto_minor;

	/**
	 * Is asynchronous read supported (read-write)
	 */
//C     	unsigned async_read;

	/**
	 * Maximum size of the write buffer
	 */
//C     	unsigned max_write;

	/**
	 * Maximum readahead
	 */
//C     	unsigned max_readahead;

	/**
	 * Capability flags, that the kernel supports
	 */
//C     	unsigned capable;

	/**
	 * Capability flags, that the filesystem wants to enable
	 */
//C     	unsigned want;

	/**
	 * For future use.
	 */
//C     	unsigned reserved[25];
//C     };
struct fuse_conn_info
{
    uint proto_major;
    uint proto_minor;
    uint async_read;
    uint max_write;
    uint max_readahead;
    uint capable;
    uint want;
    uint [25]reserved;
}

//C     struct fuse_session;
extern (C) struct fuse_session;
//C     struct fuse_chan;
extern (C) struct fuse_chan;
//C     struct fuse_pollhandle;
extern (C) struct fuse_pollhandle;

/**
 * Create a FUSE mountpoint
 *
 * Returns a control file descriptor suitable for passing to
 * fuse_new()
 *
 * @param mountpoint the mount point path
 * @param args argument vector
 * @return the communication channel on success, NULL on failure
 */
//C     struct fuse_chan *fuse_mount(const char *mountpoint, struct fuse_args *args);
extern (C):
fuse_chan * fuse_mount(char *mountpoint, fuse_args *args);

/**
 * Umount a FUSE mountpoint
 *
 * @param mountpoint the mount point path
 * @param ch the communication channel
 */
//C     void fuse_unmount(const char *mountpoint, struct fuse_chan *ch);
void  fuse_unmount(char *mountpoint, fuse_chan *ch);

/**
 * Parse common options
 *
 * The following options are parsed:
 *
 *   '-f'	     foreground
 *   '-d' '-odebug'  foreground, but keep the debug option
 *   '-s'	     single threaded
 *   '-h' '--help'   help
 *   '-ho'	     help without header
 *   '-ofsname=..'   file system name, if not present, then set to the program
 *		     name
 *
 * All parameters may be NULL
 *
 * @param args argument vector
 * @param mountpoint the returned mountpoint, should be freed after use
 * @param multithreaded set to 1 unless the '-s' option is present
 * @param foreground set to 1 if one of the relevant options is present
 * @return 0 on success, -1 on failure
 */
//C     int fuse_parse_cmdline(struct fuse_args *args, char **mountpoint,
//C     		       int *multithreaded, int *foreground);
int  fuse_parse_cmdline(fuse_args *args, char **mountpoint, int *multithreaded, int *foreground);

/**
 * Go into the background
 *
 * @param foreground if true, stay in the foreground
 * @return 0 on success, -1 on failure
 */
//C     int fuse_daemonize(int foreground);
int  fuse_daemonize(int foreground);

/**
 * Get the version of the library
 *
 * @return the version
 */
//C     int fuse_version(void);
int  fuse_version();

/**
 * Destroy poll handle
 *
 * @param ph the poll handle
 */
//C     void fuse_pollhandle_destroy(struct fuse_pollhandle *ph);
void  fuse_pollhandle_destroy(fuse_pollhandle *ph);

/* ----------------------------------------------------------- *
 * Signal handling					       *
 * ----------------------------------------------------------- */

/**
 * Exit session on HUP, TERM and INT signals and ignore PIPE signal
 *
 * Stores session in a global variable.	 May only be called once per
 * process until fuse_remove_signal_handlers() is called.
 *
 * @param se the session to exit
 * @return 0 on success, -1 on failure
 */
//C     int fuse_set_signal_handlers(struct fuse_session *se);
int  fuse_set_signal_handlers(fuse_session *se);

/**
 * Restore default signal handlers
 *
 * Resets global session.  After this fuse_set_signal_handlers() may
 * be called again.
 *
 * @param se the same session as given in fuse_set_signal_handlers()
 */
//C     void fuse_remove_signal_handlers(struct fuse_session *se);
void  fuse_remove_signal_handlers(fuse_session *se);
