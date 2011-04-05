/**
 * General functions that are neither tagfs callbacks nor database functions.
 *
 * @file tagfs_common.h
 * @author Keith Woelke
 * @date 30/07/2010
 */

#ifndef TAGFS_COMMON_H
#define TAGFS_COMMON_H

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

#endif
