#include "tagfs_debug.h"

int num_digits(unsigned int num) {
	DEBUG(ENTRY);
	int count = 0;

	DEBUG("Counting number of digits in %u", num);

	do {
		num /= 10;
		count++;
	} while(num != 0);

	DEBUG("Number of digits in %u: %d", num, count);
	DEBUG(EXIT);
	return count;
} /* num_digits */


















//bool valid_path_to_tag(const char *path) {
//	bool valid = false;
//	char *file_query = NULL;
//	char **file_array = NULL;
//	int num_files = 0;
//
//	DEBUG(ENTRY);
//
//	assert(path != NULL);
//
//	DEBUG("Checking that %s is a valid path to a tag", path);
//
//	if(strcmp(path, "/") == 0) { valid = true; }
//	else {
//		file_query = db_query_files_
//		num_files = db_count_from_query(
//		num_files = db_files_from_query(path, &file_array);
//		free_char_ptr_array(&file_array, num_files);
//
//		if(!unique_tags_in_path(path) || num_files == 0) { valid = false; }
//		else {valid = true; }
//	}
//
//	DEBUG(D_FUNCTION_VALID_PATH_TO_TAG, D_LEVEL_DEBUG, "%s is %sa valid path to a tag", path, valid ? "" : "not a ");
//	DEBUG(EXIT);
//	return valid;
//} /* valid_path_to_tag */
