/**
 * Debug related functions and macros.
 * @file tagfs_debug.h
 * @author Keith Woelke
 * @date 10/14/2010
 */

#ifndef TAGFS_DEBUG_H
#define TAGFS_DEBUG_H

#include "tagfs_params.h"

#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define INFO(...) debug_wait(); fprintf(TAGFS_DATA->logfile, "%u: ", (unsigned int)pthread_self()); fprintf(TAGFS_DATA->logfile, __VA_ARGS__); fprintf(TAGFS_DATA->logfile, "\n"); fflush(TAGFS_DATA->logfile); debug_post()

#define DEBUG(...) debug_wait(); fprintf(TAGFS_DATA->logfile, "%u: DEBUG: ", (unsigned int)pthread_self()); fprintf(TAGFS_DATA->logfile, __VA_ARGS__); fprintf(TAGFS_DATA->logfile, "\n"); fflush(TAGFS_DATA->logfile); debug_post()

#define WARN(...) debug_wait(); fprintf(TAGFS_DATA->logfile, "%u: WARNING: ", (unsigned int)pthread_self()); fprintf(TAGFS_DATA->logfile, __VA_ARGS__); fprintf(TAGFS_DATA->logfile, "\n"); fflush(TAGFS_DATA->logfile); debug_post()

#define ERROR(...) debug_wait(); fprintf(TAGFS_DATA->logfile, "%u: %s(%d): ERROR: ", (unsigned int)pthread_self(), __FILE__, __LINE__); fprintf(TAGFS_DATA->logfile, __VA_ARGS__); fprintf(TAGFS_DATA->logfile, "\n"); fflush(TAGFS_DATA->logfile); debug_post(); exit(EXIT_FAILURE)

/**
 * Opens the log file for writing.
 *
 * @param full_log_path Location of the log file.
 * @return A handle for the log file.
 */
FILE *debug_open_log();

/**
 * Closes the log file.
 *
 * @param logfile File handle of the log file.
 */
void debug_close_log();

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

#endif
