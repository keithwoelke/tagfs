#include "tagfs_common.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <string.h>

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

int path_to_array(const char *path, char ***array) {
	DEBUG(ENTRY);
	char *tmp_path = NULL;
	char *tok_ptr = NULL;
	char *token = NULL;
	int i = 0;
	int num_tokens = 0;

	assert(path != NULL);
	assert(array != NULL);
	assert(*array == NULL);

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

		assert(tmp_path);
		free(tmp_path);
		tmp_path = NULL;

		DEBUG("Returning array from %s with %d element(s).", path, num_tokens);
	}

	DEBUG(EXIT);
	return num_tokens;
} /* path_to_array */

void free_char_ptr_array(char ***array, int count) {
	DEBUG(ENTRY);
	int i = 0;

	DEBUG("Attempting to free array of %d pointer(s) plus the main pointer.", count);

	assert(array != NULL);
	assert(count >= 0);

	/* array is empty */
	if(*array == NULL) { DEBUG("Array is NULL. Nothing to free."); }
	/* array is not empty */
	else {
		assert(*array != NULL);

		/* free each element */
		for(i = 0; i < count; i++) {
			DEBUG("Freeing array[%d] = %s", i, (*array)[i]);
			free((*array)[i]);
			(*array)[i] = NULL;
		}

		DEBUG("Free'd %d element(s).", i);

		assert(*array != NULL);
		free(*array);
		*array = NULL;

		DEBUG("Free'd main array pointer.");
	}

	DEBUG(EXIT);
} /* free_char_ptr_array */

int num_tags_in_path(const char *path) {
	DEBUG(ENTRY);
	char *tmp_path = NULL;
	char *tok_ptr = NULL;
	char *token = NULL;
	int i = 0;

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

	assert(tmp_path != NULL);
	free(tmp_path);
	tmp_path = NULL;

	DEBUG("%d tags in %s", i, path);
	DEBUG(EXIT);
	return i;
} /* num_tags_in_path */
