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
 * Get the directory of where the TagFS is being executed from (assuming it is passed argv[0]). The returned path must be free'd by the caller.
 *
 * @param exec_name The path to the executable given from argv[0].
 * @return The path to the executable.
 */
const char *get_exec_dir(const char *exec_name);

void heapSort(int a[], int count);

void heapify(int a[], int count);

void siftDown(int a[], int start, int end);

void swap(int *a, int *b);

int array_intersection(int a[], int a_size, int b[], int b_size, int **result);

/**
 * Checks whether or not a given path is valid. This is specifically done by checking that every component of the path is valid and that every component is unique. Essentially, this is done with calls to valid_tags_in_path and unique_path.
 *
 * @param path A string representing a path in the file system.
 * @return true, if the path is valid. false, if the path is not valid.
 */
bool valid_path_to_tag(const char *path);

/**
 * Checks if a file is valid. If the files returned from a query contain the file that is being checked, then the file is assumed to be valid.
 * @param path a string representing a path in the file system
 * @return true, if the file is valid. false, if the file is not valid.
 */
bool valid_path_to_file(const char *path);

#endif
