#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <glib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Given a path and a collection of files at that location, calculate which tag is used the most among all files.
 *
 * @param path A directory path.
 * @param files An array of file IDs.
 * @param num_files The number of files.
 * @param exclude_tags An array of tag IDs to exclude from consideration.
 * @param exclude_tags_count The number tags to exclude.
 */
static int most_popular_tag_on_files_at_location(const char *path, int *files, int num_files, int **exclude_tags, int *exclude_tags_count) {
	GHashTable *table = NULL;
	GHashTableIter iter;
	gpointer key = NULL;
	gpointer largest_key = NULL;
	gpointer largest_value = NULL;
	gpointer value = NULL;
	int *tags_on_file = NULL;
	int file_id = 0;
	int i = 0;
	int j = 0;
	int num_tags = 0;
	long unsigned int tag_id = 0;

	DEBUG(ENTRY);

	table = g_hash_table_new(NULL, NULL);

	for(i = 0; i < num_files; i++) {
		file_id = files[i];
		num_tags = tags_from_file(file_id, &tags_on_file);

		for(j = 0; j < num_tags; j++) {
			tag_id = tags_on_file[j];

			if(g_hash_table_lookup(table, (gpointer)tag_id) == NULL) {
				g_hash_table_insert(table, (gpointer)tag_id, (gpointer)1);
			} else {
				value = g_hash_table_lookup(table, (gpointer)tag_id);
				g_hash_table_insert(table, (gpointer)tag_id, (gpointer)value++);
			}
		}

		free_single_ptr((void **)&tags_on_file);
	}

	remove_path_from_hash_table(path, &table);

	if(*exclude_tags == NULL) {
		*exclude_tags = malloc(sizeof(**exclude_tags) * g_hash_table_size(table));
	}

	for(i = 0; i < *exclude_tags_count; i++) {
		tag_id = (*exclude_tags)[i];
		g_hash_table_remove(table, (gpointer)tag_id);
	}

	g_hash_table_iter_init(&iter, table);
	while(g_hash_table_iter_next(&iter, &key, &value)) {
		if(value > largest_value) {
			largest_key = key;
			largest_value = value;
		}
	}

	g_hash_table_destroy(table);

	if(largest_key != 0) { /* add tag to list of exclusions */
		(*exclude_tags)[(*exclude_tags_count)++] = GPOINTER_TO_INT(largest_key);
	}

	DEBUG(EXIT);
	return GPOINTER_TO_INT(largest_key);
} /* most_popular_tag_on_files_at_location */

/**
 * Given a hash table, remove all matching path elements (tags).
 *
 * @param path A directory path.
 * @param table The hash table from which to remove all matching path elements (tags).
 */
static int remove_path_from_hash_table(const char *path, GHashTable **table) {
	char **path_array = NULL;
	int i = 0;
	int num_tags_in_path = 0;
	long unsigned int tag_id = 0;

	DEBUG(ENTRY);

	num_tags_in_path = path_to_array(path, &path_array);

	for(i = 0; i < num_tags_in_path; i++) {
		tag_id = db_tag_id_from_tag_name(tag_name);

		g_hash_table_remove(*table, (gpointer)tag_id);
	}

	if(num_tags_in_path > 0) {
		free_double_ptr((void ***)&path_array, num_tags_in_path);
	}

	DEBUG(EXIT);
	return g_hash_table_size(*table);
} /* remove_path_from_hash_table */

/**
 * Swaps two integers.
 *
 * @param a The first integer.
 * @param b The second integer.
 */
static void swap(int *a, int *b) {
	int temp = *a;

	DEBUG(ENTRY);

	assert(a != NULL);
	assert(b != NULL);

	*a = *b;
	*b = temp;

	DEBUG(EXIT);
} /* swap */

/**
 * Sifts the largest values to the bottom of the heap.
 *
 * @param array The array to be sifted.
 * @param start The beginning of the array segment to sift.
 * @param end The end of the array segment to sift.
 */
static void sift_down(int *array, int start, int end) {
	int child = 0;
	int root = start;
	int save_swap = 0;

	DEBUG(ENTRY);

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

	DEBUG(EXIT);
} /* sift_down */

/**
 * Puts the array into max-heap form.
 *
 * @param array The array to be heapified.
 * @param count The number of elements in the array.
 */
static void heapify(int *array, int count) {
	int start = count / 2 - 1;

	DEBUG(ENTRY);

	assert(array != NULL);
	assert(count > 0);

	while (start >= 0) {
		sift_down(array, start--, count - 1);
	}

	DEBUG(EXIT);
} /* heapify */

/**
 * Sorts an array of integers using the heap sort algorithm.
 *
 * @param array The array of integers to be sorted.
 * @param count The number of elements in the array.
 */
static void heap_sort(int *array, int count) {
	int end = 0;

	DEBUG(ENTRY);

	assert(array != NULL);
	assert(count > 0);

	heapify(array, count);

	end = count - 1;

	while (end > 0) {
		swap(&array[end], &array[0]);
		sift_down(array, 0, --end);
	}

	DEBUG(EXIT);
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
		tags_checked[i] = strdup(tag_array[i]);
		assert(tags_checked[i] != NULL);
	}

	free_double_ptr((void ***)&tag_array, num_tokens);
	free_double_ptr((void ***)&tags_checked, i);

	DEBUG("Tags in path are %sunique", unique ? "" : "not ");
	DEBUG(EXIT);
	return unique;
	return true;
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
	char *real_path = NULL;

	assert(exec_name != NULL);

	real_path = realpath(exec_name, NULL);
	exec_dir = dirname(real_path);
	free_single_ptr((void **)&real_path);

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
	free_single_ptr((void **)&tmp_file_name);
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

		/* if tags are not unique (to prevent repeated filtering of identical tags) or there are no files at the location */
		if(!unique_tags_in_path(path) || num_files == 0) { valid = false; }
		else {
			valid = true;
			free_single_ptr((void **)&file_array);
		}
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
	assert(*array == NULL);

	DEBUG("Converting %s to array", path);

	if(strcmp(path, "/") != 0) {
		/* calculate the number of tags in the path */
		num_tokens = num_tags_in_path(path);
		DEBUG("Number of tokens in path: %d", num_tokens);

		*array = malloc(num_tokens * sizeof(**array));
		assert(*array != NULL);

		tmp_path = strdup(path);
		assert(tmp_path != NULL);

		/* copy tags from path into an array */
		token = strtok_r(tmp_path, "/", &tok_ptr);
		DEBUG("Array contents:");
		for(i = 0; token != NULL; i++) {
			(*array)[i] = strdup(token);
			assert((*array)[i] != 0);
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

	tmp_path = strdup(path);
	assert(tmp_path != 0);
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
	assert(*ptr != NULL);
	free(*ptr);
	*ptr = NULL;
} /* free_single_ptr */

void free_double_ptr(void ***array, int count) {
	int i = 0;

	DEBUG(ENTRY);

	assert(*array != NULL);
	assert(count >= 0);

	DEBUG("Attempting to free array of %d pointer(s) plus the main pointer.", count);

	assert(*array != NULL);

	/* free each element */
	for(i = 0; i < count; i++) {
		free_single_ptr((void **)&(*array)[i]);
	}

	DEBUG("Free'd %d element(s).", i);

	free_single_ptr((void **)array);

	DEBUG("Free'd main array pointer.");

	DEBUG(EXIT);
} /* free_double_ptr */

int smart_tags_from_files(const char *path, int *files, int num_files, int **tags) {
	GHashTable *table = NULL;
	GHashTableIter iter;
	gpointer key = NULL;
	gpointer value = NULL;
	int *file_array = NULL;
	int *files_with_tag = NULL;
	int exclude_tags_count = 0;
	int i = 0;
	int num_files_with_tag = 0;
	int popular_tag = -1;

	DEBUG(ENTRY);

	/* populate hash table with file IDs */
	table = g_hash_table_new(NULL, NULL);
	for(i = 0; i < num_files; i++) {
		g_hash_table_insert(table, GINT_TO_POINTER(files[i]), GINT_TO_POINTER(files[i]));
	}

	/* find most popular tags */
	while(popular_tag != 0) {
		num_files = g_hash_table_size(table);
		file_array = malloc(sizeof(*file_array) * num_files);

		i = 0;
		g_hash_table_iter_init(&iter, table); /* copy files from hash table to array */
		while(g_hash_table_iter_next(&iter, &key, &value)) {
				file_array[i++] = key;
		}

		popular_tag = most_popular_tag_on_files_at_location(path, file_array, num_files, tags, &exclude_tags_count);

		free_single_ptr((void *)&file_array);

		num_files_with_tag = db_files_from_tag_id(popular_tag, &files_with_tag);

		/* remove files with most popular tag from hash table */
		for(i = 0; i < num_files_with_tag; i++) {
			g_hash_table_remove(table, GINT_TO_POINTER(files_with_tag[i]));
		}

		free_single_ptr((void *)&files_with_tag);
	}

	g_hash_table_destroy(table);

	DEBUG(EXIT);
	return exclude_tags_count;
} /* smart_tags_from_files */

int folders_at_location(const char *path, int *files, int num_files, int **folders) {
	bool FAST_BROWSE = false;
	int *file_array = NULL;
	int num_folders = 0;

	DEBUG(ENTRY);

	assert(path != NULL);
	if(strcmp(path, "/") != 0) {  /* only root is allowed to have no files */
		assert(files != NULL);
		assert(num_files > 0);
	}
	assert(*folders == NULL);

	/* show all tags at root level */
	if(strcmp(path, "/") == 0) {
		if(FAST_BROWSE) {
			num_folders = db_get_all_tags(folders);
		} else {
			num_files = db_get_all_files(&file_array);
			num_folders = smart_tags_from_files(path, file_array, num_files, folders);
			free_single_ptr((void *)&file_array);
		}

		assert(num_folders >= 0);
		DEBUG("There are %d folders in the root directory.", num_folders);
	}
	/* get all tags on file array */
	else {
		if(FAST_BROWSE) {
			num_folders = db_tags_from_files(files, num_files, folders);
		} else {
			num_folders = smart_tags_from_files(path, files, num_files, folders);
		}

		assert(num_folders >= 0);
		DEBUG("There are %d folders at %s", num_folders, path);
	}

	DEBUG(EXIT);
	return num_folders;
} /* folders_at_location */

int array_intersection(int *a, int a_size, int *b, int b_size, int **intersection) {
	int i = 0;
	int intersection_index = 0;
	int j = 0;
	int min_size = 0;

	DEBUG(ENTRY);

	assert(a != NULL);
	assert(b != NULL);
	assert(a_size > 0);
	assert(b_size > 0);
	assert(*intersection == NULL);

	DEBUG("a length is %d. b length is %d.", a_size, b_size);
	min_size = a_size < b_size ? a_size : b_size;
	DEBUG("Intersection array length set to %d.", min_size);
	*intersection = malloc(min_size * sizeof(**intersection));
	assert(*intersection != NULL);

	while(i < a_size) {
		DEBUG("Outer array is on %d.", a[i]);
		if(j == b_size) {
			break;
		}

		while(j < b_size) {
			DEBUG("Inner array is on %d.", b[j]);
			if(a[i] == b[j]) {
				DEBUG("%d == %d", a[i], b[j]);
				(*intersection)[intersection_index++] = a[i];
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
	return intersection_index;
} /* array_intersection */

int files_at_location(const char *path, int **file_array) {
	char **tag_array = NULL;
	int *cur_files = NULL;
	int *intersection_files = NULL;
	int *prev_files = NULL;
	int i = 0;
	int num_cur_files = 0;
	int num_intersection_files = 0;
	int num_prev_files = 0;
	int num_tokens = 0;
	int tag_id = 0;

	DEBUG(ENTRY);

	assert(path != NULL);
	assert(*file_array == NULL);

	num_tokens = path_to_array(path, &tag_array);
	DEBUG("Path broken up into %d tokens for %s.", num_tokens, path);

	if(num_tokens == 0) { /* if root */
		DEBUG("Retrieving a list of files with no tags for root view.");
		/* get all untagged files */
		tag_id = db_tag_id_from_tag_name("/");
		assert(tag_id >= 0);
		num_prev_files = db_files_from_tag_id(tag_id, &prev_files);
	} else {
		DEBUG("Retrieving files for %s.", tag_array[i]);
		tag_id = db_tag_id_from_tag_name(tag_array[i]); /* get tag ID for first tag */

		if(tag_id >= 0) { /* if first tag is valid */
			DEBUG("Tag ID of %s is %d.", tag_array[i], tag_id);
			num_prev_files = db_files_from_tag_id(tag_id, &prev_files);

			if(num_prev_files == 0) { /* This shouldn't happen if database is purged properly after a delete */
				WARN("Tag ID %d has no files.", tag_id);
				WARN("Purging database of tag ID %d", tag_id);
				db_delete_tag(tag_id);
			} else if(num_prev_files > 0) {
				DEBUG("%d file(s) with %s tag.", num_prev_files, tag_array[i]);

				for(i = 1; i < num_tokens; i++) {
					tag_id = db_tag_id_from_tag_name(tag_array[i]); /* get files with tag */

					if(tag_id > 0) { /* if tag is valid */
						num_cur_files = db_files_from_tag_id(tag_id, &cur_files);

						if(num_prev_files > 0) {
							/* find intersection of both arrays */
							heap_sort(prev_files, num_prev_files);
							heap_sort(cur_files, num_cur_files);

							num_intersection_files = array_intersection(prev_files, num_prev_files, cur_files, num_cur_files, &intersection_files);

							free_single_ptr((void *)&cur_files);
							free_single_ptr((void *)&prev_files);

							/* assign result to prev_files array */
							prev_files = intersection_files;
							intersection_files = NULL;
							num_prev_files = num_intersection_files;
						}
					}
					else { /* path is not valid */
						num_prev_files = 0;
					}
				}
			}

		}

		free_double_ptr((void ***)&tag_array, num_tokens);
	}

	*file_array = prev_files;

	DEBUG(EXIT);

	return num_prev_files;
} /* files_at_location */

char *tag_name_from_tag_id(int tag_id) {
	char *tag_name = NULL;

	DEBUG(ENTRY);

	assert(tag_id > 0);

	DEBUG("Retrieving tag name from tag ID %d", tag_id);
	tag_name = db_tag_name_from_tag_id(tag_id);

	DEBUG("Tag ID %d has name of %s", tag_id, tag_name);
	DEBUG(EXIT);
	return tag_name;
} /* file_name_from_id */

char *dirname(const char *path) {
	char *tmp_path = NULL;
	int i = 0;
	int length = 0;

	assert(path != NULL);

	tmp_path = strdup(path);
	assert(tmp_path != NULL);

	length = strlen(tmp_path);

	for(i = length - 1; i >= 0; i--) {
		if(i == 0) {
			tmp_path[1] = '\0';
		}
		else if(tmp_path[i] == '/') {
			tmp_path[i] = '\0';
			break;
		}
	}

	return tmp_path;
} /* dirname */

char *basename(const char *path) {
	char *tmp_path = NULL;
	int i = 0;
	int j = 0;
	int length = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	DEBUG("Calculating the basename of %s", path);

	tmp_path = strdup(path);
	assert(tmp_path != NULL);

	length = strlen(tmp_path);

	for(i = length - 1; i >= 0; i--) {
		if(tmp_path[i] == '/') {
			i++; /* increment to start after the '/' character */

			for(; i <= length; i++) {
				tmp_path[j++] = tmp_path[i];		
			}
			break;
		}
	}

	DEBUG("Base name is %s", tmp_path);
	DEBUG(EXIT);
	return tmp_path;
} /* basename */

bool valid_path_to_file(const char *path) {
	bool valid = false;
	int file_id = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	DEBUG("Checking if %s is a valid path to a file", path);

	file_id = file_id_from_path(path);

	if(file_id > 0) {
		return true;
	}

	DEBUG("%s is %sa valid file.", path, valid ? "" : "not "); 
	DEBUG(EXIT);
	return valid;
} /* valid_path_to_file */

int file_id_from_path(const char *path) {
	bool valid = false;
	char *dirpath = NULL;
	char *file_found = NULL;
	char *filename = NULL;
	int *file_array = NULL;
	int file_count = 0;
	int file_id = 0;
	int i = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	DEBUG("Retrieving file ID from %s", path);

	dirpath = dirname(path);
	filename = basename(path);

	DEBUG("Retrieving files from %s", dirpath);
	file_count = files_at_location(dirpath, &file_array);
	DEBUG("%d files at %s", file_count, dirpath);
	free_single_ptr((void **)&dirpath);

	if(file_array != NULL) {
		for(i = 0; i < file_count; i++) {
			file_found = file_name_from_id(file_array[i]);

			if(strcmp(file_found, filename) == 0) {
				file_id = file_array[i];
				valid = true;
			}

			free_single_ptr((void **)&file_found);

			if(valid == true) {
				break;
			}
		}

		free_single_ptr((void **)&file_array);
	}

	free_single_ptr((void **)&filename);

	DEBUG("%s has file ID of %d.", path, file_id); 
	DEBUG(EXIT);
	return file_id;
} /* file_id_from_path */

char *get_file_location(int file_id) {
	char *file_location = NULL;

	DEBUG(ENTRY);

	assert(file_id > 0);

	file_location = db_get_file_location(file_id);

	DEBUG(EXIT);
	return file_location;
} /* get_file_location */

void delete_file(int file_id) {
	DEBUG(ENTRY);

	db_delete_file(file_id);
	db_delete_empty_tags();

	DEBUG(EXIT);
} /* delete_file */

void remove_tags(int file_id) {
	int *tags = NULL;
	int i = 0;
	int num_tags = 0;

	DEBUG(ENTRY);

	assert(file_id > 0);

	DEBUG("Removing tags from file with ID %d", file_id);

	num_tags = tags_from_file(file_id, &tags);
	DEBUG("File ID %d has %d tags to remove", file_id, num_tags);

	for(i = 0; i < num_tags; i++) {
		db_remove_tag_from_file(tags[i], file_id);
	}

	DEBUG(EXIT);
} /* remove_tags */

int tags_from_file(int file_id, int **tags) {
	int num_tags = 0;

	DEBUG(ENTRY);

	assert(file_id > 0);

	DEBUG("Retrieving tags from file ID %d", file_id);

	num_tags = db_tags_from_files(&file_id, 1, tags);

	DEBUG(EXIT);
	return num_tags;
} /* tags_from_files */
