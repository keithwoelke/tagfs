/**
 * General functions that are not tagfs callbacks nor database functions.
 * @file tagfs_common.h
 * @author Keith Woelke
 * @date 07/30/2010
 */

#ifndef TAGFS_COMMON_H
#define TAGFS_COMMON_H

/**
 * Modifies full_exec_path directly to remove everything following the final '/'.
 *
 * @param full_path The path to a file.
 * @return The modified character pointer. The pointer is modified directly and also returned for convenience (chaining) purposes.
 */
char *get_file_directory(char *file_path);

/**
 * Calculate the number of digits in an integer.
 *
 * @param num The integer to calculate the number of digits in.
 * @return The number of digits in the integer.
 */
int num_digits(unsigned int num);













///*
// * Identical to the standand printf, but outputs to stderr instead. Also forces the program to terminate execution.
// * @param format String that contains the text to be written to stderr. It can optionally contain embedded format tags that are substituted by the values specified in subsequent argument(s). In this respect, the function is identical to printf, but outputs to stderr instead.
// * @param additional Depending on the format string, the function may expect a sequence of additional arguments, each containing one value to be inserted instead of each %-tag specified in the format parameter, if any. There should be the same number of these arguments as the number of %-tags that expect a value.
// */
//void ERROR(char *error, ...);
//
///*
// * Identical to the standand printf, but outputs to stderr instead.
// * @param format String that contains the text to be written to stderr. It can optionally contain embedded format tags that are substituted by the values specified in subsequent argument(s). In this respect, the function is identical to printf, but outputs to stderr instead.
// * @param additional Depending on the format string, the function may expect a sequence of additional arguments, each containing one value to be inserted instead of each %-tag specified in the format parameter, if any. There should be the same number of these arguments as the number of %-tags that expect a value.
// */
//void WARNING(char *error, ...);
//
///**
// * Responsible for freeing the memory in an array of strings that have been alloc'd. Also frees the array pointer.
// * @param array a pointer to the array of pointers to be freed.
// * @param count the number of elements in the array to be freed.
// * @return On success, the total number of characters written. On failure, a negative number is returned.
// */
//void free_char_ptr_array(/*@null@*/ char ***array, int count);
//
///**
// * Converts a path into an array of tokens. The path is delimated using the "/" character and each token is inserted into an array. The number of elements in the array is returned and the pointer passed in by the user will be allocated for the array of elements. The caller is responsible for freeing both the array pointer, and the pointers of each token in the array. The function free_char_ptr_array can be used for this purpose.
// * @param path a string representing a path in the file system.
// * @param array a pointer to an array of strings (char pointers).
// * 
// */
//int path_to_array(const char *path, char ***array);
//
///**
// * Counts the number of tags in a path. Uses the standand path delimiter "/" to calculate the total. Function uses strtok, but does not modify the path that is passed in as a parameter.
// * @param path a string representing a path in the file system.
//const int num_tags_in_path(const char* path);
// * @return the number of path elements delimited by the "/" character.
// */
//int num_tags_in_path(const char* path);
//
///**
// * Checks if a string is contained within an array. Expects a count parameter to be passed in representing the length of the array.
// * @param array the array the check
// * @param string the string to check for
// * @return true, if the string is in the array. false, if the string is not in the array.
// */
//bool array_contains_string(/*@out@*/ char **array, char *string, int count);
//
///**
// * Checks if a file is valid. If the files returned from a query contain the file that is being checked, then the file is assumed to be valid.
// * @param path a string representing a path in the file system
// * @return true, if the file is valid. false, if the file is not valid.
// **/
//bool valid_path_to_file(const char *path);
//
///**
// * Checks whether or not a given path is valid. This is specifically done by checking that every component of the path is valid and that every component is unique. Essentially, this is done with calls to valid_tags_in_path and unique_path.
// * @param path a string representing a path in the file system.
// * @return true, if the path is valid. false, if the path is not valid.
// */
//bool valid_path_to_tag(const char *path);

#endif
