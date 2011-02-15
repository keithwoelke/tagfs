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
 * Sets the table contents to all files which are in the given path (have the tags specified by the path) including all files which may have a stricter collection of tags.
 *
 * @param path The path to use when looking for matching files.
 * @param table The table to insert the results into.
 */
void db_set_directory_contents(const char *path, const char *table);

/**
 * Removes all files from a table which do not have the specified tag.
 *
 * @param tag The tag to use when filtering the specified table.
 * @param table The table to filter.
 */
void db_filter_table(const char *tag, const char *table);

#endif
