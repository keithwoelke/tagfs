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
 * Returns an array representing a column in the database.
 *
 * @param desired_column_name The name of the column to pull results from.
 * @param result_query The query to run.
 * @param result_array The array with which to store results from the column returned from result_query matching the name desired_column_name.
 * @return The number of results returned from the query.
 */
int db_array_from_query(char *desired_column_name, char *result_query, char ***result_array);

int db_files_from_tag_id(int tag_id, int **file_array);

int db_tag_id_from_tag_name(char *tag);

int db_get_all_tags(int **folders);

char *db_tag_name_from_tag_id(int tag_id);

#endif
