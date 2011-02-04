/**
 * Debug related functions and macros.
 * @file tagfs_debug.h
 * @author Keith Woelke
 * @date 10/14/2010
 */

#ifndef TAGFS_DEBUG_H
#define TAGFS_DEBUG_H

#include "tagfs_debug.h"
#include "tagfs_params.h"

#include <pthread.h>
#include <stdlib.h>

#define INFO(...) \
debug_wait(); \
fprintf(TAGFS_DATA->logfile, "%u: ", (unsigned int)pthread_self()); \
log_timestamp(); \
fprintf(TAGFS_DATA->logfile, ": "); \
fprintf(TAGFS_DATA->logfile, __VA_ARGS__); \
fprintf(TAGFS_DATA->logfile, "\n"); \
fflush(TAGFS_DATA->logfile); \
debug_post()


#define DEBUG(...) \
debug_wait(); \
fprintf(TAGFS_DATA->logfile, "%u: ", (unsigned int)pthread_self()); \
log_timestamp(); \
fprintf(TAGFS_DATA->logfile, ": "); \
fprintf(TAGFS_DATA->logfile, "DEBUG: "); \
fprintf(TAGFS_DATA->logfile, __VA_ARGS__); \
fprintf(TAGFS_DATA->logfile, "\n"); \
fflush(TAGFS_DATA->logfile); \
debug_post()

#define WARN(...) \
debug_wait(); \
fprintf(TAGFS_DATA->logfile, "%u: ", (unsigned int)pthread_self()); \
log_timestamp(); \
fprintf(TAGFS_DATA->logfile, ": "); \
fprintf(TAGFS_DATA->logfile, "WARNING: "); \
fprintf(TAGFS_DATA->logfile, __VA_ARGS__); \
fprintf(TAGFS_DATA->logfile, "\n"); \
fflush(TAGFS_DATA->logfile); \
debug_post()

#define ERROR(...) \
debug_wait(); \
fprintf(TAGFS_DATA->logfile, "%u: ", (unsigned int)pthread_self()); \
log_timestamp(); \
fprintf(TAGFS_DATA->logfile, ": "); \
fprintf(TAGFS_DATA->logfile, "%s(%d): ERROR: ", __FILE__, __LINE__); \
fprintf(TAGFS_DATA->logfile, __VA_ARGS__); \
fprintf(TAGFS_DATA->logfile, "\n"); \
fflush(TAGFS_DATA->logfile); \
debug_post(); \
exit(EXIT_FAILURE)

#define ENTRY "---> %s", __FUNCTION__

#define EXIT "<--- %s", __FUNCTION__

/**
 * Initializes the debug semaphore.
 */
void debug_init();

/**
 * Locks the debug semaphore.
 */
void debug_wait();

/**
 * Releases the debug semaphore.
 */
void debug_post();

/**
 * Returns the current date and time.
 *
 * @return The current date and time and a string.
 */
void log_timestamp();

#endif
