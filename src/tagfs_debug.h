/**
 * Debug related functions and macros.
 *
 * @file tagfs_debug.h
 * @author Keith Woelke
 * @date 10/14/2010
 */

#ifndef TAGFS_DEBUG_H
#define TAGFS_DEBUG_H

#include "tagfs_debug.h"
#include "tagfs_params.h"

#include <stdio.h>
#include <stdlib.h>

#define INFO(...) \
log_timestamp(); \
fprintf(TAGFS_DATA->log_file, ": "); \
fprintf(TAGFS_DATA->log_file, __VA_ARGS__); \
fprintf(TAGFS_DATA->log_file, "\n"); \
fflush(TAGFS_DATA->log_file);

#define DEBUG(...) \
log_timestamp(); \
fprintf(TAGFS_DATA->log_file, ": "); \
fprintf(TAGFS_DATA->log_file, "[DEBUG] "); \
fprintf(TAGFS_DATA->log_file, __VA_ARGS__); \
fprintf(TAGFS_DATA->log_file, "\n"); \
fflush(TAGFS_DATA->log_file);

#define WARN(...) \
log_timestamp(); \
fprintf(TAGFS_DATA->log_file, ": "); \
fprintf(TAGFS_DATA->log_file, "%s(%d): [WARNING] ", __FILE__, __LINE__); \
fprintf(TAGFS_DATA->log_file, __VA_ARGS__); \
fprintf(TAGFS_DATA->log_file, "\n"); \
fflush(TAGFS_DATA->log_file);

#define ERROR(...) \
log_timestamp(); \
fprintf(TAGFS_DATA->log_file, ": "); \
fprintf(TAGFS_DATA->log_file, "%s(%d): [ERROR] ", __FILE__, __LINE__); \
fprintf(TAGFS_DATA->log_file, __VA_ARGS__); \
fprintf(TAGFS_DATA->log_file, "\n"); \
fflush(TAGFS_DATA->log_file); \
exit(EXIT_FAILURE)

#define ENTRY "---> %s", __FUNCTION__

#define EXIT "<--- %s", __FUNCTION__

/**
 * Prints the current date and time to the log.
 */
void log_timestamp();

#endif
