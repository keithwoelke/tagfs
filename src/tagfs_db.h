/**
 * Handles interaction with the sqlite3 database.
 *
 * @file tagfs_db.h
 * @author Keith Woelke
 * @date 07/25/2010
 */

#ifndef TAGFS_DB_H
#define TAGFS_DB_H

/**
 * Returns a physical file system location corresponding to a location in the 
 * TagFS.
 *
 * @param file_id The file ID for which to return the full physical path.
 * @return The path to the physical location in the file system corresponding 
 * to the file ID in the TagFS.
 */
char *db_get_file_location(int file_id);

/**
 * Retrieve a set of unique tags associated with a collection of file IDs
 *
 * @param files Array of file IDs.
 * @param num_files Number of file IDs in the array.
 * @param folders The collection of tags associated with the file IDs.
 * @return The number of unique tags associated with the file IDs.
 */
int db_tags_from_files(int *files, int num_files, int **folders);

/**
 * Return the number of rows returned from the specified query.
 *
 * @param query The query which will be run to count the results.
 * @return The number of rows returned by the query.
 */
int db_count_from_query(char *query);

/**
 * Converts a tag ID into its corresponding tag name.
 *
 * @param tag_id The tag ID to match to its corresponding tag name.
 * @return The tag name corresponding to the specified tag ID.
 */
char *db_tag_name_from_tag_id(int tag_id);

/**
 * Returns an integer array representing a column in the database where the results correspond to those returned from a query.
 *
 * @param desired_column_name The name of the column to pull results from.
 * @param result_query The query to run against the database.
 * @param result_array The array with which to store results from the column returned from result_query matching the name desired_column_name.
 * @return The number of results returned from the query.
 */
int db_int_array_from_query(char *desired_column_name, char *result_query, int **result_array);

/**
 * Returns an array of all tags.
 *
 * @param tags A collection of all available tags.
 * @return The number of tags returned.
 */
int db_get_all_tags(int **tags);

/**
 * Returns an array of all files.
 *
 * @param tags A collection of all available files.
 * @return The number of files returned.
 */
int db_get_all_files(int **files);

/**
 * Returns files associated with a tag ID.
 *
 * @param tag_id The tag ID to use when checking for files.
 * @param file_array OUT: The files which match the tag ID.
 * @return The number of files matching the tag ID.
 */
int db_files_from_tag_id(int tag_id, int **file_array);

/**
 * Retrieve the tag ID from a tag name.
 *
 * @param tag_name The tag to match to its corresponding tag ID.
 * @return The tag ID which matches the tag name.
 */
int db_tag_id_from_tag_name(char *tag_name);

/**
 * Delete a file from the TagFS by its corresponding file ID.
 *
 * @param file_id The ID of the file to delete.
 */
void db_delete_file(int file_id);

/**
 * Delete a tag from the TagFS by its corresponding tag ID.
 *
 * @param tag_id The ID of the tag to delete.
 */
void db_delete_tag(int tag_id);

/**
 * Deletes all tags from the database which are not associated with files.
 */
void db_delete_empty_tags();

/**
 * Adds an existing tag to a file.
 *
 * @param tag_id The ID of the tag to add to the file.
 * @param file_id The ID of the file to add the tag to.
 */
void db_add_tag_to_file(int tag_id, int file_id);

/**
 * Removes a tag from a file.
 *
 * @param tag_id The ID of the tag to add to the file.
 * @param file_id The ID of the file to add the tag to.
 */
void db_remove_tag_from_file(int tag_id, int file_id);

/**
 * Remove a file from the filesystem.
 *
 * @param file_id The ID of the file to be removed from the filesystem.
 */
void db_remove_file(int file_id);

#endif
