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













int db_array_from_query(char *desired_column_name, const char *result_query, /*@out@*/ char ***result_array);

#endif

