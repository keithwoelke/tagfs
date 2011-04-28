/**
 * General functions that are neither tagfs callbacks nor database functions.
 *
 * @file tagfs_common.h
 * @author Keith Woelke
 * @date 30/07/2010
 */

#ifndef TAGFS_COMMON_H
#define TAGFS_COMMON_H

#include <stdbool.h>

/**
 * Counts the number if digits in an integer.
 *
 * @param num The number from which to count digits.
 * @return The number of digits in num.
 */
int num_digits(unsigned int num);

/**
 * Get the directory of where the TagFS is being executed from (assuming it is passed argv[0]). The returned path must be free'd by the caller.
 *
 * @param exec_name The path to the executable given from argv[0].
 * @return The path to the executable.
 */
const char *get_exec_dir(const char *exec_name);

/**
 * Retrieves the file name of the file associated with a file ID.
 *
 * @param file_id The ID of the file.
 * @return The name of the file corresponding to the file ID.
 */
char *file_name_from_id(int file_id);

/**
 * Checks whether or not a given path is valid. This is specifically done by checking that every component of the path is valid and that every component is unique. Essentially, this is done with calls to valid_tags_in_path and unique_path.
 *
 * @param path A string representing a path in the file system.
 * @return true, if the path is valid. false, if the path is not valid.
 */
bool valid_path_to_folder(const char *path);

/**
 * Converts a path into an array of tokens. The path is delimated using the "/" character and each token is inserted into an array. The number of elements in the array is returned and the pointer passed in by the user will be allocated for the array of elements. The caller is responsible for freeing both the array pointer, and the pointers of each token in the array. The function free_char_ptr_array can be used for this purpose.
 *
 * @param path a string representing a path in the file system.
 * @param array a pointer to an array of strings (char pointers).
 */
int path_to_array(const char *path, char ***array);

/**
 * Counts the number of tags in a path. Uses the standand path delimiter "/" to calculate the total. Function uses strtok_r, but does not modify the path that is passed in as a parameter.
 *
 * @param path a string representing a path in the file system.
const int num_tags_in_path(const char* path);
 * @return the number of path elements delimited by the "/" character.
 */
int num_tags_in_path(const char* path);

/**
 * Checks if a string is contained within an array. Expects a count parameter to be passed in representing the length of the array.
 * 
 * @param array the array the check
 * @param string the string to check for
 * @return true, if the string is in the array. false, if the string is not in the array.
 */
bool array_contains_string(char **array, char *string, int count);





































/**
 * Counts the number of tags in a path. Uses the standand path delimiter "/" to calculate the total. Function uses strtok, but does not modify the path that is passed in as a parameter.
 * @param path a string representing a path in the file system.
const int num_tags_in_path(const char* path);
 * @return the number of path elements delimited by the "/" character.
 */
int num_tags_in_path(const char* path);






//int array_intersection(int a[], int a_size, int b[], int b_size, int **result);


/**
 * Checks if a file is valid. If the files returned from a query contain the file that is being checked, then the file is assumed to be valid.
 * @param path a string representing a path in the file system
 * @return true, if the file is valid. false, if the file is not valid.
 */
bool valid_path_to_file(const char *path);

int file_id_from_path(const char *path);

int files_at_location(const char *path, int **file_array);

int folders_at_location(const char *path, int *file_array, char ***folder_array);

#endif
