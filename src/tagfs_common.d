#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

const char* get_file_directory(const char *path) {
	char **tag_array = NULL;
	char *new_path = NULL;
	int i = 0;
	int num_tokens = 0;

	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_ENTRY, "get_file_directory");

	assert(path != NULL);

	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Retrieving path to file referenced by: %s", path);

	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "Path: %s", path);
	num_tokens = path_to_array(path, &tag_array);
	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "Number of tokens: %d", num_tokens);

	if(num_tokens < 2) {
		new_path = malloc(strlen("/") * sizeof(*new_path) + 1);
		assert(new_path != NULL);
		strcpy(new_path, "/");
		assert(strcmp(new_path, "/") == 0);
	}
	else {
		DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "Last token (tag_array[%d]): %s", num_tokens - 1, tag_array[num_tokens - 1]);
		new_path = calloc(strlen(path) - strlen(tag_array[num_tokens - 1]), sizeof(*new_path)); /* No + 1 for the \0 is necessary because it cancels with the - 1 for the last / */
		assert(new_path != NULL);

		for(i = 0; i < num_tokens - 1; i++) {
			assert(tag_array[i] != NULL);
			strcat(strcat(new_path, "/"), tag_array[i]);
		}
	}

	free_char_ptr_array(&tag_array, num_tokens);

	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "File directory of %s: %s", path, new_path);
	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_EXIT, "get_file_directory");
	return new_path;
} /* get_file_directory */

void free_char_ptr_array(/*@null@*/ char ***array, int count) {
	int i = 0;

	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_ENTRY, "free_char_ptr_array");
	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Attempting to free array of %d pointer(s) plus the main pointer.", count);

	assert(array != NULL);
	assert(count >= 0);

	if(*array == NULL) {
		DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Array is NULL. Nothing to free.");
	}
	else {
		assert(*array != NULL);
		for(i = 0; i < count; i++) {
			DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_FOLDER_CONTENTS, "Freeing array[%d] = %s", i, (*array)[i]);
			free((*array)[i]);
			(*array)[i] = NULL;
		}
		DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Free'd %d element(s).", i);

		assert(*array != NULL);
		free(*array);
		*array = NULL;

		DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Free'd main array pointer.");
	}

	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_EXIT, "free_char_ptr_array");
} /* free_char_ptr_array */

int path_to_array(const char *path, char ***array) {
	char *tmp_path = NULL;
	char *tok_ptr = NULL;
	char *token = NULL;
	int i = 0;
	int num_tokens = 0;

	assert(path != NULL);
	assert(array != NULL);
	assert(*array == NULL);

	if(strcmp(path, "/") != 0) {
		DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_ENTRY, "path_to_array");
		DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_DEBUG, "Converting %s to an array", path);

		num_tokens = num_tags_in_path(path);
		DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_DEBUG, "Number of tokens: %d", num_tokens);

		tmp_path = malloc(strlen(path) * sizeof(*tmp_path) + 1);
		assert(tmp_path);

		*array = malloc(num_tokens * sizeof(**array));
		assert(*array != NULL);

		strcpy(tmp_path, path);
		assert(strcmp(path, tmp_path) == 0);
		token = strtok_r(tmp_path, "/", &tok_ptr);

		DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_DEBUG, "Array contents:");

		sem_wait(&debug_sem);
		debug_indent_level++;
		sem_post(&debug_sem);
		for(i = 0; token != NULL; i++) {
			(*array)[i] = malloc(strlen(token) * sizeof(*(*array)[i]) + 1);
			assert((*array)[i] != NULL);
			strcpy((*array)[i], token);
			assert(strcmp((*array)[i], token) == 0);
			DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_DEBUG, "array[%d] = %s, at address %p.", i, (*array)[i], (*array)[i]);
			token = strtok_r(NULL, "/", &tok_ptr);
		}
		sem_wait(&debug_sem);
		debug_indent_level--;
		sem_post(&debug_sem);

		assert(tmp_path);
		free(tmp_path);
		tmp_path = NULL;

		DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_DEBUG, "Returning array from path with %d element(s).", num_tokens);
		DEBUG(D_FUNCTION_PATH_TO_ARRAY, D_LEVEL_EXIT, "path_to_array");
	}

	return num_tokens;
} /* path_to_array */

int num_tags_in_path(const char *path) {
	char *tmp_path = NULL;
	char *tok_ptr = NULL;
	char *token = NULL;
	int i = 0;

	DEBUG(D_FUNCTION_NUM_TAGS_IN_PATH, D_LEVEL_ENTRY, "num_tags_in_path");

	assert(path != NULL);

	DEBUG(D_FUNCTION_NUM_TAGS_IN_PATH, D_LEVEL_DEBUG, "Calculating number of tags in %s", path);

	tmp_path = malloc(strlen(path) * sizeof(*tmp_path) + 1);
	assert(tmp_path != NULL);

	strcpy(tmp_path, path);
	assert(strcmp(path, tmp_path) == 0);
	token = strtok_r(tmp_path, "/", &tok_ptr);

	for(i = 0; token != NULL; i++) { token = strtok_r(NULL, "/", &tok_ptr); }
	
	assert(tmp_path != NULL);
	free(tmp_path);
	tmp_path = NULL;

	DEBUG(D_FUNCTION_NUM_TAGS_IN_PATH, D_LEVEL_DEBUG, "Number of tags in %s: %d", path, i);
	DEBUG(D_FUNCTION_NUM_TAGS_IN_PATH, D_LEVEL_EXIT, "num_tags_in_path");
	return i;
} /* num_tags_in_path */

bool array_contains_string(/*@out@*/ char **array, char *string, int count) {
	bool contains = false;
	int i = 0;

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_ENTRY, "array_contains_string");

	assert(string != NULL);

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_DEBUG, "Checking if array contains \"%s\"", string);

	assert(array != NULL);
	assert(count >= 0);

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "Array contents:");
	
	sem_wait(&debug_sem);
	debug_indent_level++;
	sem_post(&debug_sem);
	for(i = 0; i < count; i++) {
		DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "array[%d] = %s", i, array[i]);
	}
	sem_wait(&debug_sem);
	debug_indent_level--;
	sem_post(&debug_sem);

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "Array comparison:");

	sem_wait(&debug_sem);
	debug_indent_level++;
	sem_post(&debug_sem);
	if(array != NULL) {
		for(i = 0; i < count; i++) {
			if(array[i] != NULL && strcmp(array[i], string) == 0) { 
				DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "array[%d] = %s = %s", i, array[i], string);
				contains = true;
				break;
			}
			else {
				DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "array[%d] = %s != %s", i, array[i], string);
			}
		}
	}
	sem_wait(&debug_sem);
	debug_indent_level--;
	sem_post(&debug_sem);

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_DEBUG, "%s \"%s\"", contains ? "Array contains" : "Array does not contain", string, contains ? "true" : "false");
	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_EXIT, "array_contains_string");
	return contains;
} /* array_contains_string */

bool valid_path_to_file(const char *file_path) {
	bool valid = false;
	char **file_array = NULL;
	char **tag_array = NULL;
	char *file = NULL;
	const char *dir_path = NULL;
	int num_files_in_dir = 0;
	int num_tokens = 0;

	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_ENTRY, "valid_path_to_file");

	assert(file_path != NULL);

	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "Checking that %s is a valid path to a file.", file_path);

	dir_path = get_file_directory(file_path);
	assert(dir_path != NULL);
	num_files_in_dir = db_files_from_query(dir_path, &file_array);

	assert(dir_path != NULL);
	free((void *)dir_path);
	dir_path = NULL;

	num_tokens = path_to_array(file_path, &tag_array);
	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "Number of tokens: %d", num_tokens);

	if(num_tokens > 0) {
		assert(tag_array != NULL);
		file = tag_array[num_tokens - 1];
		assert(file != NULL);
		DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "File: %s", file);

		if(num_files_in_dir != 0 && array_contains_string(file_array, file, num_files_in_dir)) { valid = true; }
	}

	free_char_ptr_array(&file_array, num_files_in_dir);

	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "%s is %svalid path to a file", file_path, valid ? "a " : "not a ");
	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_EXIT, "valid_path_to_file");

	free_char_ptr_array(&tag_array, num_tokens);

	return valid;
} /* valid_path_to_file */

static bool unique_tags_in_path(const char *path) {
	bool unique = true;
	char **tag_array = NULL;
	char **tags_checked = NULL;
	int i = 0;
	int num_tokens = 0;

	DEBUG(D_FUNCTION_UNIQUE_TAGS_IN_PATH, D_LEVEL_ENTRY, "unique_tags_in_path");

	assert(path != NULL);

	DEBUG(D_FUNCTION_UNIQUE_TAGS_IN_PATH, D_LEVEL_DEBUG, "Checking uniqueness of tags in %s", path);
	num_tokens = path_to_array(path, &tag_array);
	DEBUG(D_FUNCTION_UNIQUE_TAGS_IN_PATH, D_LEVEL_DEBUG, "Number of tokens: %d", num_tokens);
	tags_checked = malloc(num_tokens * sizeof(*tags_checked));
	assert(tags_checked != NULL);

	for(i = 0; i < num_tokens; i++) {
		assert(tag_array != NULL);
		if(array_contains_string(tags_checked, tag_array[i], i)) {
			unique = false;
			break;
		}

		tags_checked[i] = malloc(strlen(tag_array[i]) + 1);
		assert(tags_checked[i] != NULL);
		strcpy(tags_checked[i], tag_array[i]);
	}

	free_char_ptr_array(&tag_array, num_tokens);
	free_char_ptr_array(&tags_checked, i);

	DEBUG(D_FUNCTION_UNIQUE_TAGS_IN_PATH, D_LEVEL_DEBUG, "Tags in path are %sunique", unique ? "" : "not ");
	DEBUG(D_FUNCTION_UNIQUE_TAGS_IN_PATH, D_LEVEL_EXIT, "unique_tags_in_path");
	return unique;
} /* unique_tags_in_path */

bool valid_path_to_tag(const char *path) {
	bool valid = false;

	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_ENTRY, "valid_path_to_tag");

	assert(path != NULL);

	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_DEBUG, "Checking that %s is a valid path to a tag", path);

	if(strcmp(path, "/") == 0) { valid = true; }
	else {
		if(!unique_tags_in_path(path)) { valid = false; }
		else {valid = true; }
	}

	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_DEBUG, "%s is %sa valid path to a tag", path, valid ? "" : "not a ");
	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_EXIT, "valid_path_to_tag");
	return valid;
} /* valid_path_to_tag */

int num_digits(int num) {
	int count = 1;

	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_ENTRY, "num_digits");
	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_DEBUG, "Counting number of digits in %d", num);

	while(num != 0) {
		num /= 10;
		count++;
	}

	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_DEBUG, "Number of digits: %d", count);
	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_EXIT, "num_digits");
	return count;
} /* num_digits */
