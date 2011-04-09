#include "tagfs_common.h"
#include "tagfs_debug.h"
#include "tagfs_db.h"

#include <assert.h>
#include <libgen.h>
#include <stdbool.h>
#include <string.h>

const char *get_exec_dir(const char *exec_name) {
	const char *exec_dir = NULL;

	assert(exec_name != NULL);

	exec_dir = dirname(realpath(exec_name, NULL));

	return exec_dir;
} /* get_exec_dir */

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

bool valid_path_to_folder(const char *path) {
	DEBUG(ENTRY);

	if(strcmp(path, "/") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Audio") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/mov") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Audio/ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/mov/Video") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg/Video") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/ogg/Audio") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video/ogg") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else if(strcmp(path, "/Video/mov") == 0) {
		DEBUG(EXIT);
		return true;
	}
	else {
	DEBUG("FOO: %s\n", path);
		DEBUG(EXIT);
		return false;
	}
}

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

char *file_name_from_id(int id) {
	DEBUG(ENTRY);

	char *foo = NULL;

	if(id == 2) {
		foo = malloc(strlen("Josh Woodward - Swansong.ogg") * sizeof(foo) + 1);
		snprintf(foo, strlen("Josh Woodward - Swansong.ogg") + 1, "%s", "Josh Woodward - Swansong.ogg");
		return foo;
	}
	else if(id == 3) {
		foo = malloc(strlen("sample_iTunes.mov") * sizeof(foo) + 1);
		snprintf(foo, strlen("sample_iTunes.mov") + 1, "%s", "sample_iTunes.mov");
		return foo;
	}
	else if(id == 1) {
		foo = malloc(strlen("How fast.ogg") * sizeof(foo) + 1);
		snprintf(foo, strlen("How fast.ogg") + 1, "%s", "How fast.ogg");
		return foo;
	}
	else if(id == 4) {
		foo = malloc(strlen("Moby Dick.txt") * sizeof(foo) + 1);
		snprintf(foo, strlen("Moby Dick.txt") + 1, "%s", "Moby Dick.txt");
		return foo;
	}

	DEBUG(EXIT);
	return malloc(1);
}
