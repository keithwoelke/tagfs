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
 * Return the number of rows returned from the specified query.
 *
 * @param query The query which will be run to count the results.
 * @return The number of rows returned by the query.
 */
int db_count_from_query(const char *query);

/**
 * Connect to the database.
 */
void db_connect();

/**
 * Disconnect from the database.
 */
void db_disconnect();

/**
 * Removes all files from a table which do not have the specified tag.
 *
 * @param tag The tag to use when filtering the specified table.
 * @param table The table to filter.
 */
void db_filter_table(const char *tag, const char *table);

/**
 * Loads into the specified table, all file IDs which have the specificed tag. If the table is not already empty, the ids will still be inserted.
 *
 * @param tag The tag to use when selecting files to insert.
 * @param table the table to insert the file IDs into.
 */
void db_load_table(const char *tag, const char *table);

/**
 * Delete all rows from the specified table.
 *
 * @param table the name of the table to truncate.
 */
void db_truncate_table(const char *table);

/**
 * Returns an array representing a column in the database.
 *
 * @param desired_column_name The name of the column to pull results from.
 * @param result_query The query to run.
 * @param result_array The array with which to store results from the column returned from result_query matching the name desired_column_name.
 * @return The number of results returned from the query.
 */
int db_array_from_query(char *desired_column_name, const char *result_query, char ***result_array);

/**
 * Returns a physical file system location corresponding to a location in the TagFS.
 *
 * @param path The path to the file in the TagFS.
 * @return The physical location in the file system corresponding to the file in the TagFS.
 */
char* db_get_file_location(const char *path);

#endif

