/**
 * General functions that are not tagfs callbacks nor database functions.
 * @file tagfs_common.h
 * @author Keith Woelke
 * @date 30/07/2010
 */

#ifndef TAGFS_COMMON_H
#define TAGFS_COMMON_H

#include <stdbool.h>

/**
 * Counts the number if digits in an integer.
 *
 * @param num The number from which to count digits.
 * @return The number of digits in num.
 */
int num_digits(int num);

/**
 * Checks whether or not a given path is valid. This is specifically done by checking that every component of the path is valid and that every component is unique. Essentially, this is done with calls to valid_tags_in_path and unique_path.
 *
 * @param path a string representing a path in the file system.
 * @return true, if the path is valid. false, if the path is not valid.
 */
bool valid_path_to_tag(const char *path);

#endif
