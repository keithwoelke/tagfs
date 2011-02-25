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
log_timestamp(); 

#define DEBUG(...) \
log_timestamp(); 

#define WARN(...) \
log_timestamp(); 

#define ERROR(...) \
log_timestamp(); 

#define ENTRY "---> %s", __FUNCTION__

#define EXIT "<--- %s", __FUNCTION__

/**
 * Prints the current date and time to the log.
 */
void log_timestamp();

#endif
