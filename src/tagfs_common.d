module tagfs_common;
import tagfs_db;
import tagfs_debug;

import std.c.stdlib;
import std.c.string;
import std.string;
import std.array;
import std.conv;
import std.path;
import std.algorithm;

extern(C):
char* get_file_directory(const char *path) {
	char **tag_array = null;
	char *new_path = null;
	int i = 0;
	int num_tokens = 0;

	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_ENTRY, "get_file_directory");

	assert(path !is null);

	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Retrieving path to file referenced by: %s", path);

	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "Path: %s", path);
	num_tokens = path_to_array(path, &tag_array);
	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "Number of tokens: %d", num_tokens);

	if(num_tokens < 2) {
		new_path = cast(char*) malloc(strlen("/") * (*new_path).sizeof + 1);
		assert(new_path !is null);
		strcpy(new_path, "/");
		assert(strcmp(new_path, "/") == 0);
	}
	else {
		DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "Last token (tag_array[%d]): %s", num_tokens - 1, tag_array[num_tokens - 1]);
		new_path = cast(char*) calloc(strlen(path) - strlen(tag_array[num_tokens - 1]), (*new_path).sizeof); /* No + 1 for the \0 is necessary because it cancels with the - 1 for the last / */
		assert(new_path !is null);

		for(i = 0; i < num_tokens - 1; i++) {
			assert(tag_array[i] !is null);
			strcat(strcat(new_path, "/"), tag_array[i]);
		}
	}

	free_char_ptr_array(&tag_array, num_tokens);

	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_DEBUG, "File directory of %s: %s", path, new_path);
	DEBUG(D_FUNCTION_GET_FILE_DIRECTORY, D_LEVEL_EXIT, "get_file_directory");
	return new_path;
} /* get_file_directory */

void free_char_ptr_array(/*@null@*/ const(char *)**array, int count) {
	int i = 0;

	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_ENTRY, "free_char_ptr_array");
	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Attempting to free array of %d pointer(s) plus the main pointer.", count);

	assert(array !is null);
	assert(count >= 0);

	if(*array is null) {
		DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Array is null. Nothing to free.");
	}
	else {
		assert(*array !is null);
		free(cast(void*)*array);
		*array = null;

		DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_DEBUG, "Free'd main array pointer.");
	}

	DEBUG(D_FUNCTION_FREE_CHAR_PTR_ARRAY, D_LEVEL_EXIT, "free_char_ptr_array");
} /* free_char_ptr_array */

int path_to_array(const char *path, char ***array) {
	auto dpath = to!string(path);
	auto result = dpath.split(sep);
	if(result.front == "")
		result.popFront();

	*array = cast(char**) malloc(result.length * (**array).sizeof);
	assert(*array !is null);

	foreach(i, a; result)
		(*array)[i] = cast(char*)(toStringz(a));
	return result.length;
}

int num_tags_in_path(const char *path) {
	auto dpath = to!string(path);
	auto result = dpath.split(sep);
	if(result.front == "")
		result.popFront();

	return result.length;
}

bool array_contains_string(/*@out@*/ char **array, char *strData, int count) {
	bool contains = false;
	int i = 0;

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_ENTRY, "array_contains_strData");

	assert(strData !is null);

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_DEBUG, "Checking if array contains \"%s\"", strData);

	assert(array !is null);
	assert(count >= 0);

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "Array contents:");
	
	debug_indent();
	for(i = 0; i < count; i++) {
		DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "array[%d] = %s", i, array[i]);
	}
	debug_deindent();

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "Array comparison:");

	debug_indent();
	if(array !is null) {
		for(i = 0; i < count; i++) {
			if(array[i] !is null && strcmp(array[i], strData) == 0) { 
				DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "array[%d] = %s = %s", i, array[i], strData);
				contains = true;
				break;
			}
			else {
				DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_FOLDER_CONTENTS, "array[%d] = %s != %s", i, array[i], strData);
			}
		}
	}
	debug_deindent();

	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_DEBUG, "%s \"%s\", %s", toStringz(contains ? "Array contains" : "Array does not contain"), strData, toStringz(contains ? "true" : "false"));
	DEBUG(D_FUNCTION_ARRAY_CONTAINS_STRING, D_LEVEL_EXIT, "array_contains_string");
	return contains;
} /* array_contains_strData */

bool valid_path_to_file(const char *file_path) {
	bool valid = false;
	char **file_array = null;
	char **tag_array = null;
	char *file = null;
	char *dir_path = null;
	int num_files_in_dir = 0;
	int num_tokens = 0;

	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_ENTRY, "valid_path_to_file");

	assert(file_path !is null);

	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "Checking that %s is a valid path to a file.", file_path);

	dir_path = get_file_directory(file_path);
	assert(dir_path !is null);
	num_files_in_dir = db_files_from_restricted_query(dir_path, &file_array);

	assert(dir_path !is null);
	free(cast(void *)dir_path);
	dir_path = null;

	num_tokens = path_to_array(file_path, &tag_array);
	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "Number of tokens: %d", num_tokens);

	if(num_tokens > 0) {
		assert(tag_array !is null);
		file = tag_array[num_tokens - 1];
		assert(file !is null);
		DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "File: %s", file);

		if(num_files_in_dir != 0 && array_contains_string(file_array, file, num_files_in_dir)) { valid = true; }
	}

	free_char_ptr_array(&file_array, num_files_in_dir);

	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_DEBUG, "%s is %svalid path to a file", file_path, toStringz(valid ? "a " : "not a "));
	DEBUG(D_FUNCTION_VALID_PATH_TO_FILE, D_LEVEL_EXIT, "valid_path_to_file");

	free_char_ptr_array(&tag_array, num_tokens);

	return valid;
} /* valid_path_to_file */

static bool unique_tags_in_path(const char *path) {
	auto dpath = to!string(path);
	auto result = dpath.split(sep);
	if(findAdjacent(sort(result)).empty)
		return true;
	return false;
}

bool valid_path_to_tag(const char *path) {
	bool valid = false;
	char **file_array = null;
	int num_files = 0;

	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_ENTRY, "valid_path_to_tag");

	assert(path !is null);

	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_DEBUG, "Checking that %s is a valid path to a tag", path);

	if(strcmp(path, "/") == 0) { valid = true; }
	else {
		num_files = db_files_from_query(path, &file_array);

		if(!unique_tags_in_path(path) || num_files == 0) { valid = false; }
		else {valid = true; }

		free_char_ptr_array(&file_array, num_files);
	}


	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_DEBUG, "%s is %sa valid path to a tag", path, toStringz(valid ? "" : "not a "));
	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_EXIT, "valid_path_to_tag");
	return valid;
} /* valid_path_to_tag */

int num_digits(int num) {
	int count = 0;

	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_ENTRY, "num_digits");
	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_DEBUG, "Counting number of digits in %d", num);

	do {
		num /= 10;
		count++;
	} while(num != 0);

	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_DEBUG, "Number of digits: %d", count);
	DEBUG(D_FUNCTION_NUM_DIGITS, D_LEVEL_EXIT, "num_digits");
	return count;
} /* num_digits */
