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
 * Converts a path into an array of tokens. The path is delimated using the "/" character and each token is inserted into an array. The number of elements in the array is returned and the pointer passed in by the user will be allocated for the array of elements. The caller is responsible for freeing both the array pointer, and the pointers of each token in the array. The function free_char_ptr_array can be used for this purpose.
 *
 * @param path a string representing a path in the file system.
 * @param array a pointer to an array of strings (char pointers).
 */
int path_to_array(const char *path, char ***array);

/**
 * Responsible for freeing the memory in an array of strings that have been alloc'd. Also frees the array pointer.
 *
 * @param array a pointer to the array of pointers to be free'd.
 * @param count the number of elements in the array to be free'd.
 */
void free_char_ptr_array(char ***array, int count);

/**
 * Counts the number of tags in a path. Uses the standand path delimiter "/" to calculate the total. Function uses strtok_r, but does not modify the path that is passed in as a parameter.
 *
 * @param path a string representing a path in the file system.
const int num_tags_in_path(const char* path);
 * @return the number of path elements delimited by the "/" character.
 */
int num_tags_in_path(const char* path);

/**
 * Return a query which will select all files which contain the specified tag. The returned query must be free'd by the caller.
 *
 * @param tag The tag which will be used to construct the select query.
 * @return The query which can be used to select all files containing the specified tag.
 **/
char *get_query_files_with_tag(const char *tag);

/**
 * Sets the table contents to all files which are in the given path (have the tags specified by the path) including all files which may have a stricter collection of tags.
 *
 * @param path The path to use when looking for matching files.
 * @param table The table to insert the results into.
 */
void set_directory_contents(const char *path, const char *table);

/**
 * Retrieves all tags at a location in the TagFS. The results are placed into tag_array and are queried from the specified table. This table should be populated prior to calling tags_from_query with a call to db_set_directory_contents(path, table) where table is the same as the table being queried for tags. tag_array must be free'd by the caller once it is no longer needed. free_char_ptr_array can be used for this purpose.
 *
 * @param path The path to use when looking for tags in the filesystem.
 * @param tag_array The array to fill with tags.
 * @param table The name of the table to use when looking for tags.
 * @return The number of tags in tag_array.
 */
int tags_from_query(const char *path, char ***tag_array, const char *table);

/**
 * Returns all files which have the tags as denoted in the path.
 *
 * @param path The path to use when looking for tags in the filesystem.
 * @param file_array The array to fill with files.
 * @param table The name of the table to use when looking for files.
 * @return The number of files in file_array.
 */
int files_from_query(const char *path, char ***file_array, const char *table);

/**
 * Checks whether or not a given path is valid. This is specifically done by checking that every component of the path is valid and that every component is unique. Essentially, this is done with calls to valid_tags_in_path and unique_path.
 *
 * @param path A string representing a path in the file system.
 * @return true, if the path is valid. false, if the path is not valid.
 */
bool valid_path_to_tag(const char *path);








int db_array_from_query(char *desired_column_name, const char *result_query, char ***result_array);

#endif
