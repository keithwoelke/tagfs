#include "tagfs_common.h"
#include "tagfs_debug.h"
#include "tagfs_db.h"

#include <libgen.h>

const char *get_exec_dir(const char *exec_name) {
	const char *exec_dir = NULL;

	exec_dir = dirname(realpath(exec_name, NULL));

	return exec_dir;
} /* get_exec_dir */

void heapSort(int a[], int count) {
	heapify(a, count);

	int end = count-1;
	while (end > 0) {
		swap(&a[end], &a[0]);
		siftDown(a, 0, end-1);
		end = end - 1;
	}
}

void heapify(int a[], int count) {
	int start = count / 2 - 1;

	while (start >= 0) {
		siftDown(a, start, count-1);
		start = start - 1;
	}
}

void siftDown(int a[], int start, int end) {
	int root = start;

	while (root * 2 + 1 <= end) {
		int child = root * 2 + 1;
		int swapI = root;
		if (a[swapI] < a[child])
			swapI = child;
		if (child < end && a[swapI] < a[child+1])
			swapI = child + 1;
		if (swapI != root) {
			swap(&a[root], &a[swapI]);
			root = swapI;
		}
		else
			return;
	}
}

void swap(int *a, int *b) {
	int temp = 0;

	temp = *a;
	*a = *b;
	*b = temp;
}

int array_intersection(int a[], int a_size, int b[], int b_size, int **result) {
	int i = 0;
	int j = 0;
	int num_results = 0;

	if(a_size > b_size) {
		*result = malloc(a_size * sizeof(*result));
	}
	else {
		*result = malloc(b_size * sizeof(*result));
	}

	for(i = 0; i < a_size; i++) {
		while(j < 6) {
			if(a[i] < b[j]) { break; }
			else if(a[i] == b[j]) { 
				(*result)[num_results++] = a[i++];
			}
			j++;
		}
	}

	*result = (int *)realloc(*result, num_results * sizeof(int));
	return num_results;
} /* array_intersection */

bool valid_path_to_tag(const char *path) {
	return false;
}

bool valid_path_to_file(const char *path) {
	return false;
}
