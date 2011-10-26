#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <libgen.h>
#include <stdbool.h>
#include <string.h>

/**
 * Swaps two integers.
 *
 * @param a The first integer.
 * @param b The secont integer.
 */
static void swap(int *a, int *b) {
	int temp = *a;

	*a = *b;
	*b = temp;
} /* swap */

/**
 * Sifts the largest values to the bottom of the heap.
 *
 * @param array The array to be sifted.
 * @param start The beginning of the array segment to sift.
 * @param end The end of the array segment to sift.
 */
static void sift_down(int *array, int start, int end) {
	int root = start;
	int child = 0;
	int save_swap = 0;

	assert(array != NULL);
	assert(start >= 0);
	assert(end >= 0);
	assert(start <= end);

	while (root * 2 + 1 <= end) {
		child = root * 2 + 1;
		save_swap = root;

		if (array[save_swap] < array[child]) {
			save_swap = child;
		}

		if (child + 1 <= end && array[save_swap] < array[child + 1]) {
			save_swap = child + 1;
		}

		if (save_swap != root) {
			swap(&array[root], &array[save_swap]);
			root = save_swap;
		} else {
			return;
		}
	}
} /* sift_down */

/**
 * Puts the array into max-heap form.
 *
 * @param array The array to be heapified.
 * @param count The number of elements in the array.
 */
static void heapify(int *array, int count) {
	int start = count / 2 - 1;

	assert(array != NULL);
	assert(count > 0);

	while (start >= 0) {
		sift_down(array, start--, count - 1);
	}
} /* heapify */

/**
 * Sorts an array of integers using the heap sort algorithm.
 *
 * @param array The array of integers to be sorted.
 * @param count The number of elements in the array.
 */
void heap_sort(int *array, int count) {
	int end = 0;

//	DEBUG(ENTRY);

	assert(array != NULL);
	assert(count > 0);

	heapify(array, count);

	end = count - 1;

	while (end > 0) {
		swap(&array[end], &array[0]);
		sift_down(array, 0, --end);
	}

//	DEBUG(EXIT);
} /* heap_sort */

/**
 * Calculates if all of the elements in a path are unique.
 *
 * @param path The path to check.
 * @return True, if all path elements are unique. False, otherwise.
 */
static bool unique_tags_in_path(const char *path) {
	bool unique = true;
	char **tag_array = NULL;
	char **tags_checked = NULL;
	char *tag = NULL;
	int i = 0;
	int num_tokens = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	/* break path into tags and create array to hold checked tags */
	DEBUG("Checking uniqueness of tags in %s", path);
	num_tokens = path_to_array(path, &tag_array);
	DEBUG("Number of tokens: %d", num_tokens);
	tags_checked = malloc(num_tokens * sizeof(*tags_checked));
	assert(tags_checked != NULL);

	for(i = 0; i < num_tokens; i++) {
		assert(tag_array != NULL);
		tag = tag_array[i];

		if(array_contains_string((const char **)tags_checked, tag, i)) { /* if token is a duplicate */
			unique = false;
			break;
		}

		/* save checked tag */
		tags_checked[i] = malloc(strlen(tag) * sizeof(*tag) + 1);
		assert(tags_checked[i] != NULL);
		strcpy(tags_checked[i], tag_array[i]);
		assert(strcmp(tags_checked[i], tag_array[i]) == 0);
	}

	free_double_ptr((void ***)&tag_array, num_tokens);
	free_double_ptr((void ***)&tags_checked, i);

	DEBUG("Tags in path are %sunique", unique ? "" : "not ");
	DEBUG(EXIT);
	return unique;
} /* unique_tags_in_path */

int num_digits(unsigned int num) {
	int count = 0;
	int orig_num = num;

	DEBUG(ENTRY);
	DEBUG("Counting number of digits in %u", num);

	/* count number of digits */
	do {
		num /= 10;
		count++;
	} while(num != 0);

	DEBUG("Number of digits in %u: %d", orig_num, count);
	DEBUG(EXIT);
	return count;
} /* num_digits */

char *get_exec_dir(const char *exec_name) {
	char *exec_dir = NULL;

	assert(exec_name != NULL);

	exec_dir = dirname(realpath(exec_name, NULL));

	return exec_dir;
} /* get_exec_dir */

char *file_name_from_id(int file_id) {
	char *file_location = NULL;
	char *file_name = NULL;
	char *tmp_file_name = NULL;
	int file_name_length = 0;
	int written = 0; /* number of characters written by snprintf */

	DEBUG(ENTRY);
	DEBUG("Retrieving file name of file id %d", file_id);

	/* get file location */
	file_location = db_get_file_location(file_id);

	/* get file name from file location */
	tmp_file_name = basename(file_location);

	file_name_length = strlen(tmp_file_name);
	file_name = malloc(file_name_length * sizeof(*file_name) + 1);
	assert(file_name != NULL);
	written = snprintf(file_name, file_name_length + 1, "%s", tmp_file_name);
	assert(written == file_name_length);

	free_single_ptr((void **)&file_location);

	DEBUG("File name of file ID %d is %s", file_id, file_name);
	DEBUG(EXIT);
	return file_name;
} /* file_name_from_id */

bool valid_path_to_folder(const char *path) {
	bool valid = false;
	int *file_array = NULL;
	int num_files = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	DEBUG("Checking that %s is a valid path to a folder", path);

	if(strcmp(path, "/") == 0) { valid = true; }
	else {
		/* count files at location */
		num_files = files_at_location(path, &file_array);
		free_single_ptr((void **)&file_array);

		/* if tags are not unique (to prevent repeated filtering of identical tags) or there are no files at the location */
		if(!unique_tags_in_path(path) || num_files == 0) { valid = false; }
		else {valid = true; }
	}

	DEBUG("%s is %sa valid path to a folder", path, valid ? "" : "not a ");
	DEBUG(EXIT);
	return valid;
} /* valid_path_to_folder */

int path_to_array(const char *path, char ***array) {
	char *tmp_path = NULL;
	char *tok_ptr = NULL;
	char *token = NULL;
	int i = 0;
	int num_tokens = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	*array = NULL;

	DEBUG("Converting %s to array", path);

	if(strcmp(path, "/") != 0) {
		/* calculate the number of tags in the path */
		num_tokens = num_tags_in_path(path);
		DEBUG("Number of tokens in path: %d", num_tokens);

		/* make a copy of the path that can be modified */
		tmp_path = malloc(strlen(path) * sizeof(*tmp_path) + 1);
		assert(tmp_path != NULL);

		*array = malloc(num_tokens * sizeof(**array));
		assert(*array != NULL);

		strcpy(tmp_path, path);
		assert(strcmp(path, tmp_path) == 0);

		/* copy tags from path into an array */
		token = strtok_r(tmp_path, "/", &tok_ptr);
		DEBUG("Array contents:");
		for(i = 0; token != NULL; i++) {
			(*array)[i] = malloc(strlen(token) * sizeof((**array)[i]) + 1);
			assert((*array)[i] != NULL);
			strncpy((*array)[i], token, strlen(token) + 1);
			assert(strcmp((*array)[i], token) == 0);
			DEBUG("array[%d] = %s, at address %p.", i, (*array)[i], (*array)[i]);
			token = strtok_r(NULL, "/", &tok_ptr);
		}

		free_single_ptr((void **)&tmp_path);

		DEBUG("Returning array from %s with %d element(s).", path, num_tokens);
	}

	DEBUG(EXIT);
	return num_tokens;
} /* path_to_array */

int num_tags_in_path(const char *path) {
	char *tmp_path = NULL;
	char *tok_ptr = NULL;
	char *token = NULL;
	int i = 0;

	DEBUG(ENTRY);
	
	assert(path != NULL);

	DEBUG("Calculating number of tags in %s", path);

	/* make copy of path for tokenization */
	tmp_path = malloc(strlen(path) * sizeof(*tmp_path) + 1);
	assert(tmp_path != NULL);

	strcpy(tmp_path, path);
	assert(strcmp(path, tmp_path) == 0);
	token = strtok_r(tmp_path, "/", &tok_ptr);

	/* count tokens */
	for(i = 0; token != NULL; i++) { token = strtok_r(NULL, "/", &tok_ptr); }

	free_single_ptr((void **)&tmp_path);

	DEBUG("%d tags in %s", i, path);
	DEBUG(EXIT);
	return i;
} /* num_tags_in_path */

bool array_contains_string(const char **array, const char *string, int count) {
	bool contains = false;
	int i = 0;

	DEBUG(ENTRY);

	assert(string != NULL);
	assert(count >= 0);

	DEBUG("Checking if array contains \"%s\"", string);

	if(array != NULL) {
		/* check each array element and see if it matches string */
		for(i = 0; i < count; i++) {
			if(array[i] != NULL && strcmp(array[i], string) == 0) { 
				contains = true;
				break;
			}
		}
	}

	DEBUG("%s \"%s\"", contains ? "Array contains" : "Array does not contain", string);
	DEBUG(EXIT);
	return contains;
} /* array_contains_string */

void free_single_ptr(void **ptr) {
	free(*ptr);
	*ptr = NULL;
} /* free_single_ptr */

void free_double_ptr(void ***array, int count) {
	int i = 0;

	DEBUG(ENTRY);

	assert(array != NULL);
	assert(count >= 0);

	DEBUG("Attempting to free array of %d pointer(s) plus the main pointer.", count);

	/* array is empty */
	if(*array == NULL) { DEBUG("Array is NULL. Nothing to free."); }
	/* array is not empty */
	else {
		assert(*array != NULL);

		/* free each element */
		for(i = 0; i < count; i++) {
			free_single_ptr((void **)&(*array)[i]);
		}

		DEBUG("Free'd %d element(s).", i);

		free_single_ptr((void **)array);

		DEBUG("Free'd main array pointer.");
	}

	DEBUG(EXIT);
} /* free_double_ptr */

int folders_at_location(const char *path, int *files, int num_files, int **folders) {
	int num_folders = 0;

	DEBUG(ENTRY);

	assert(path != NULL);
	assert(files != NULL);
	assert(num_files > 0);

	/* show all tags at root level */
	if(strcmp(path, "/") == 0) {
		num_folders = db_get_all_tags(folders);
		assert(num_folders >= 0);
		DEBUG("There are %d folders in the root directory.", num_folders);
	}
	/* get all tags on file array */
	else {
		num_folders = db_get_tags_from_files(files, num_files, folders);
		assert(num_folders >= 0);
		DEBUG("There are %d folders at %s", num_folders, path);
	}

	DEBUG(EXIT);
	return num_folders;
} /* folders_at_location */

int array_intersection(int *a, int a_size, int *b, int b_size, int **folders) {
	int folder_index = 0;
	int i = 0;
	int j = 0;
	int min_size = 0;

	DEBUG(ENTRY);

	assert(a != NULL);
	assert(b != NULL);
	assert(a_size > 0);
	assert(b_size > 0);

	min_size = a_size > b_size ? a_size : b_size;
	*folders = malloc(min_size * sizeof(**folders));
	assert(*folders != NULL);

	while(i < a_size) {
		while(j < b_size) {
			if(a[i] == b[j]) {
				(*folders)[folder_index++] = a[i];
				i++;
				j++;
				break;
			} else if(a[i] > b[j]) {
				j++;
				break;
			} else {
				i++;
				break;
			}
		}
	}

	DEBUG(EXIT);
	return folder_index;
} /* array_intersection */








 








char *tag_name_from_id(int tag_id) {
	char * foo = NULL;

	if(tag_id == 1) {
		foo = malloc(6 * sizeof(*foo));
		snprintf(foo, 6, "Video");
	} else if(tag_id == 2) {
		foo = malloc(6 * sizeof(*foo));
		snprintf(foo, 6, "Audio");
	} else if(tag_id == 3) {
		foo = malloc(4 * sizeof(*foo));
		snprintf(foo, 4, "ogg");
	} else if(tag_id == 4) {
		foo = malloc(4 * sizeof(*foo));
		snprintf(foo, 4, "mov");
	}

	return foo;
} /* file_name_from_id */

bool valid_path_to_file(const char *path) {
	DEBUG(ENTRY);

	if(strcmp(path, "/Audio/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/Audio/ogg/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/mov/Video/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/mov/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/ogg/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/ogg/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/ogg/Video/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/ogg/Audio/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/Video/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/Video/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/Video/ogg/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/Video/mov/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	} else if(strcmp(path, "/Moby Dick.txt") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else {
		return false;
	}
}

int file_id_from_path(const char *path) {
	DEBUG(ENTRY);

	if(strcmp(path, "/Audio/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	} else if(strcmp(path, "/Audio/ogg/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	} else if(strcmp(path, "/mov/sample_iTunes.mov") == 0)
	{
		return 3;
	} else if(strcmp(path, "/mov/Video/sample_iTunes.mov") == 0)
	{
		return 3;
	} else if(strcmp(path, "/ogg/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	} else if(strcmp(path, "/ogg/How fast.ogg") == 0)
	{
		return 1;
	} else if(strcmp(path, "/ogg/Audio/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	} else if(strcmp(path, "/ogg/Video/How fast.ogg") == 0)
	{
		return 1;
	} else if(strcmp(path, "/Video/How fast.ogg") == 0)
	{
		return 1;
	} else if(strcmp(path, "/Video/ogg/How fast.ogg") == 0)
	{
		return 1;
	} else if(strcmp(path, "/Video/sample_iTunes.mov") == 0)
	{
		return 3;
	} else if(strcmp(path, "/Video/mov/sample_iTunes.mov") == 0)
	{
		return 3;
	} else if(strcmp(path, "/Moby Dick.txt") == 0)
	{
		return 4;
	} else {
		DEBUG("%s is not recognized.", path);
	}

	DEBUG(EXIT);
	return 0;
};

int files_at_location(const char *path, int **file_array) {
	DEBUG(ENTRY);

	if(strcmp(path, "/Audio") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 2;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/Audio/ogg") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 2;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/mov") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 3;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/mov/Video") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 3;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/ogg") == 0) {
		(*file_array) = malloc(2 * sizeof(**file_array));
		(*file_array)[0] = 1;
		(*file_array)[1] = 2;

		DEBUG(EXIT);
		return 2;
	}
	else if(strcmp(path, "/ogg/Video") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 1;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/ogg/Audio") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 2;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/Video") == 0) {
		(*file_array) = malloc(2 * sizeof(**file_array));
		(*file_array)[0] = 1;
		(*file_array)[1] = 3;

		DEBUG(EXIT);
		return 2;
	}
	else if(strcmp(path, "/Video/ogg") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 1;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/Video/mov") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 3;

		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/") == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 4;

		DEBUG(EXIT);
		return 1;
	}

	DEBUG(EXIT);
	return 0;
}
