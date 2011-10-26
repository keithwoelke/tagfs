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
 * Get the directory of where the TagFS is being executed from (assuming it is 
 * passed argv[0]). The returned path must be free'd by the caller. Logging 
 * functions such as DEBUG, INFO, and WARN should not be called in this 
 * function.
 *
 * @param exec_name The path to the executable given from argv[0].
 * @return The path to the executable.
 */
char *get_exec_dir(const char *exec_name);

/**
 * Retrieves the file name of the file associated with a file ID. The returned
 * file name must be free'd by the caller.
 *
 * @param file_id The ID of the file.
 * @return The name of the file corresponding to the file ID.
 */
char *file_name_from_id(int file_id);

/**
 * Checks whether or not a given path is valid. This is specifically done by 
 * checking that every component of the path is valid and that every component 
 * is unique. Essentially, this is done with calls to valid_tags_in_path() and 
 * unique_path().
 *
 * @param path A string representing a path in the file system.
 * @return True, if the path is valid. False, if the path is not.
 */
bool valid_path_to_folder(const char *path);

/**
 * Converts a path into an array of tokens. The path is delimated using the "/" 
 * character and each token is inserted into an array. The number of elements in
 * the array is returned and the pointer passed in by the user will be allocated
 * for the array of elements. The caller is responsible for freeing both the 
 * array pointer, and the pointers of each token in the array. The function 
 * free_double_ptr() can be used for this purpose.
 *
 * @param path A string representing a path in the file system.
 * @param array A pointer to an array of strings (char pointers).
 * @return The number of elements in the path array.
 */
int path_to_array(const char *path, char ***array);

/**
 * Counts the number of tags in a path. Uses the standand path delimiter "/" to 
 * calculate the total. Function uses strtok_r, but does not modify the path 
 * that is passed in as a parameter.
 *
 * @param path A string representing a path in the file system.
 * @return The number of path elements delimited by the "/" character.
 */
int num_tags_in_path(const char *path);

/**
 * Checks if a string is contained within an array. Expects a count parameter 
 * to be passed in representing the length of the array.
 * 
 * @param array The array the check.
 * @param string The string to check the array for.
 * @return True, if the string is in the array. False, if the string is not.
 */
bool array_contains_string(const char **array, const char *string, int count);

/**
 * Responsible for freeing the memory of a single pointer. The main added 
 * benefit is that it will automatically assign the pointer to NULL once it is 
 * done.
 *
 * @param ptr The pointer to free.
 */
void free_single_ptr(void **ptr);

/**
 * Responsible for freeing the memory in an array of objects that have been
 * alloc'd. Also frees the array pointer.
 *
 * @param array A pointer to the array of pointers to be free'd.
 * @param count The number of elements in the array to be free'd.
 */
void free_double_ptr(void ***array, int count);

/**
 * Returns a list of the folders at the specified location in the filesystem. This based on the files which are at the same location.
 *
 * @param path The path in the filesystem.
 * @param files The files at the specified filesystem location.
 * @param num_files The number of files in the specified filesystem location.
 * @param folders The array of folders will be populated based on the tags on the specified files.
 * @return The number of folders at the specified location.
 */
int folders_at_location(const char *path, int *files, int num_files, int **folders);

/**
 * Counts the number of tags in a path. Uses the standand path delimiter "/" to calculate the total. Function uses strtok, but does not modify the path that is passed in as a parameter.
 *
 * @param path a string representing a path in the file system.
const int num_tags_in_path(const char* path);
 * @return The number of path elements delimited by the "/" character.
 */
int num_tags_in_path(const char *path);

/*
 * Finds the overlap between two arrays. Namely, it will return an array with the elements both arrays have in common. Both arrays are assumed to contain sorted array of unique values. Caller is responsible for freeing the memory for returned folder array.
 *
 * @param a The first array to compare.
 * @param a_size The size of the first array.
 * @param b The second array to compare.
 * @param b_size The size of the second array.
 * @param folders An array of items which exist in both arrays.
 * @return The number of items in the intersection.
 */
int array_intersection(int *a, int a_size, int *b, int b_size, int **intersection);





































/**
 * Checks if a file is valid. If the files returned from a query contain the file that is being checked, then the file is assumed to be valid.
 * @param path a string representing a path in the file system
 * @return true, if the file is valid. false, if the file is not valid.
 */
bool valid_path_to_file(const char *path);

int file_id_from_path(const char *path);

int files_at_location(const char *path, int **file_array);


char *tag_name_from_id(int file_id);

int filter_path_from_folders(const char *path, int **folders, int num_folders);

#endif
