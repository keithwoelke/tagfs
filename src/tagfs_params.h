/**
 * TagFS parameters and global settings.
 * @file tagfs_params.h
 * @author Keith Woelke
 * @date 1/22/2011
 */

#ifndef TAGFS_PARAMS_H
#define TAGFS_PARAMS_H

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * Maintain tagfs state.
 */
struct tagfs_state {
	FILE *logfile;
	bool debug;
	char *dbpath;
	char *execdir;
	char *logpath;
};
#define TAGFS_DATA ((struct tagfs_state *) fuse_get_context()->private_data)

#endif
