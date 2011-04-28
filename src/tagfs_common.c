#include "tagfs_common.h"
#include "tagfs_debug.h"
#include "tagfs_db.h"

#include <assert.h>
#include <libgen.h>
#include <stdbool.h>
#include <string.h>

int num_digits(unsigned int num) {
	DEBUG(ENTRY);
	int count = 0;
	int origNum = num;

	DEBUG("Counting number of digits in %u", num);

	/* count number of digits */
	do {
		num /= 10;
		count++;
	} while(num != 0);

	DEBUG("Number of digits in %u: %d", origNum, count);
	DEBUG(EXIT);
	return count;
} /* num_digits */

const char *get_exec_dir(const char *exec_name) {
	const char *exec_dir = NULL;

	assert(exec_name != NULL);

	exec_dir = dirname(realpath(exec_name, NULL));

	return exec_dir;
} /* get_exec_dir */

char *file_name_from_id(int file_id) {
	DEBUG(ENTRY);
	char *file_location = NULL;
	char *file_name = NULL;
	char *tmp_file_name = NULL;
	int file_name_length = 0;
	int written = 0;

	/* get file location */
	file_location = db_get_file_location(file_id);

	/* get file name from location */
	tmp_file_name = basename(file_location);

	file_name_length = strlen(tmp_file_name);
	file_name = malloc(file_name_length * sizeof(*file_name) + 1);
	written = snprintf(file_name, file_name_length + 1, "%s", tmp_file_name);
	assert(written == file_name_length);

	assert(file_location != NULL);
	free(file_location);
	file_location = NULL;

	DEBUG(EXIT);
	return file_name;
} /* file_name_from_id */








static bool unique_tags_in_path(const char *path) {
//	bool unique = true;
	char **tag_array = NULL;
//	char **tags_checked = NULL;
//	int i = 0;
	int num_tokens = 0;

	DEBUG(ENTRY);

	assert(path != NULL);

	DEBUG("Checking uniqueness of tags in %s", path);
	num_tokens = path_to_array(path, &tag_array);
	return true;
//	DEBUG("Number of tokens: %d", num_tokens);
//	tags_checked = malloc(num_tokens * sizeof(*tags_checked));
//	assert(tags_checked != NULL);
//
//	for(i = 0; i < num_tokens; i++) {
//		assert(tag_array != NULL);
//		if(array_contains_string(tags_checked, tag_array[i], i)) {
//			unique = false;
//			break;
//		}
//
//		tags_checked[i] = malloc(strlen(tag_array[i]) + 1);
//		assert(tags_checked[i] != NULL);
//		strcpy(tags_checked[i], tag_array[i]);
//	}
//
//	free_char_ptr_array(&tag_array, num_tokens);
//	free_char_ptr_array(&tags_checked, i);
//
//	DEBUG("Tags in path are %sunique", unique ? "" : "not ");
//	DEBUG(EXIT);
//	return unique;
} /* unique_tags_in_path */

bool valid_path_to_folder(const char *path) {
	DEBUG(ENTRY);
	bool valid = false;
	int *file_array = NULL;
	int num_files = 0;

	assert(path != NULL);

	DEBUG("Checking that %s is a valid path to a tag", path);

	if(strcmp(path, "/") == 0) { valid = true; }
	else {
		/* count files at location */
		num_files = files_at_location(path, &file_array);
		free(file_array);

		/* if tags are not unique (to prevent repeated filtering of identical tags) or there are no files at the location */
		if(!unique_tags_in_path(path) || num_files == 0) { valid = false; }
		else {valid = true; }
	}

	DEBUG("%s is %sa valid path to a tag", path, valid ? "" : "not a ");
	DEBUG(EXIT);
	return valid;
} /* valid_path_to_folder */




















//int array_intersection(int a[], int a_size, int b[], int b_size, int **result) {
//	int i = 0;
//	int j = 0;
//	int num_results = 0;
//
//	if(a_size > b_size) {
//		*result = malloc(a_size * sizeof(*result));
//	}
//	else {
//		*result = malloc(b_size * sizeof(*result));
//	}
//
//	for(i = 0; i < a_size; i++) {
//		while(j < 6) {
//			if(a[i] < b[j]) { break; }
//			else if(a[i] == b[j]) { 
//				(*result)[num_results++] = a[i++];
//			}
//			j++;
//		}
//	}
//
//	*result = (int *)realloc(*result, num_results * sizeof(int));
//	return num_results;
//} /* array_intersection */








bool valid_path_to_file(const char *path) {
	DEBUG(ENTRY);

	if(strcmp(path, "/Audio/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Audio/ogg/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/mov/Video/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/mov/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg/Video/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg/Audio/Josh Woodward - Swansong.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video/ogg/How fast.ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video/mov/sample_iTunes.mov") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Moby Dick.txt") == 0) {
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
	}
	else if(strcmp(path, "/Audio/ogg/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	}
	else if(strcmp(path, "/mov/sample_iTunes.mov") == 0)
	{
		return 3;
	}
	else if(strcmp(path, "/mov/Video/sample_iTunes.mov") == 0)
	{
		return 3;
	}
	else if(strcmp(path, "/ogg/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	}
	else if(strcmp(path, "/ogg/How fast.ogg") == 0)
	{
		return 1;
	}
	else if(strcmp(path, "/ogg/Audio/Josh Woodward - Swansong.ogg") == 0)
	{
		return 2;
	}
	else if(strcmp(path, "/ogg/Video/How fast.ogg") == 0)
	{
		return 1;
	}
	else if(strcmp(path, "/Video/How fast.ogg") == 0)
	{
		return 1;
	}
	else if(strcmp(path, "/Video/ogg/How fast.ogg") == 0)
	{
		return 1;
	}
	else if(strcmp(path, "/Video/sample_iTunes.mov") == 0)
	{
		return 3;
	}
	else if(strcmp(path, "/Video/mov/sample_iTunes.mov") == 0)
	{
		return 3;
	}
	else if(strcmp(path, "/Moby Dick.txt") == 0)
	{
		return 4;
	}
	else {
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

int folders_at_location(const char *path, int *file_array, char ***folder_array) {
	DEBUG(ENTRY);

	if(strcmp(path, "/") == 0) {
		*folder_array = malloc(4 * sizeof(**folder_array));
		(*folder_array)[0] = malloc(5 * sizeof(*(*folder_array)[0]) + 1);
		(*folder_array)[1] = malloc(5 * sizeof(*(*folder_array)[1]) + 1);
		(*folder_array)[2] = malloc(3 * sizeof(*(*folder_array)[2]) + 1);
		(*folder_array)[3] = malloc(3 * sizeof(*(*folder_array)[3]) + 1);
		snprintf((*folder_array)[0], 6, "%s", "Audio");
		snprintf((*folder_array)[1], 6, "%s", "Video");
		snprintf((*folder_array)[2], 4, "%s", "mov");
		snprintf((*folder_array)[3], 4, "%s", "ogg");

		DEBUG("Returning folders for /.");
		DEBUG(EXIT);
		return 4;
	}
	else if(strcmp(path, "/mov") == 0) {
		*folder_array = malloc(1 * sizeof(**folder_array));
		(*folder_array)[0] = malloc(5 * sizeof(*(*folder_array)[0]) + 1);
		snprintf((*folder_array)[0], 6, "%s", "Video");

		DEBUG("Returning folders for /.");
		DEBUG(EXIT);
		return 1;
	}
	else if(strcmp(path, "/Video") == 0) {
		*folder_array = malloc(2 * sizeof(**folder_array));
		(*folder_array)[0] = malloc(3 * sizeof(*(*folder_array)[0]) + 1);
		(*folder_array)[1] = malloc(3 * sizeof(*(*folder_array)[0]) + 1);
		snprintf((*folder_array)[0], 4, "%s", "ogg");
		snprintf((*folder_array)[1], 4, "%s", "mov");

		DEBUG("Returning folders for /.");
		DEBUG(EXIT);
		return 2;
	}
	else if(strcmp(path, "/ogg") == 0) {
		*folder_array = malloc(2 * sizeof(**folder_array));
		(*folder_array)[0] = malloc(5 * sizeof(*(*folder_array)[0]) + 1);
		(*folder_array)[1] = malloc(5 * sizeof(*(*folder_array)[1]) + 1);
		snprintf((*folder_array)[0], 6, "%s", "Video");
		snprintf((*folder_array)[1], 6, "%s", "Audio");

		DEBUG("Returning folders for /.");
		DEBUG(EXIT);
		return 2;
	}
	else if(strcmp(path, "/Audio") == 0) {
		*folder_array = malloc(2 * sizeof(**folder_array));
		(*folder_array)[0] = malloc(3 * sizeof(*(*folder_array)[0]) + 1);
		(*folder_array)[1] = malloc(3 * sizeof(*(*folder_array)[1]) + 1);
		snprintf((*folder_array)[0], 4, "%s", "ogg");
		snprintf((*folder_array)[1], 4, "%s", "mov");

		DEBUG("Returning folders for /.");
		DEBUG(EXIT);
		return 2;
	}

	return 0;
}
