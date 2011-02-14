/**
 * General functions that are neither tagfs callbacks nor database functions.
 *
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
int num_digits(unsigned int num);

/**
 * Converts a path into an array of tokens. The path is delimated using the "/" character and each token is inserted into an array. The number of elements in the array is returned and the pointer passed in by the user will be allocated for the array of elements. The caller is responsible for freeing both the array pointer, and the pointers of each token in the array. The function free_char_ptr_array can be used for this purpose.
 *
 * @param path a string representing a path in the file system.
 * @param array a pointer to an array of strings (char pointers).
 */
int path_to_array(const char *path, char ***array);

/**
 * Responsible for freeing the memory in an array of strings that have been alloc'd. Also frees the array pointer.
 *
 * @param array a pointer to the array of pointers to be free'd.
 * @param count the number of elements in the array to be free'd.
 */
void free_char_ptr_array(char ***array, int count);

/**
 * Counts the number of tags in a path. Uses the standand path delimiter "/" to calculate the total. Function uses strtok_r, but does not modify the path that is passed in as a parameter.
 *
 * @param path a string representing a path in the file system.
const int num_tags_in_path(const char* path);
 * @return the number of path elements delimited by the "/" character.
 */
int num_tags_in_path(const char* path);

#endif
