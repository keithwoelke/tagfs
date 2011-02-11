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
 * Loads into the "directory_contents" table, all file IDs which have the specificed tag.
 *
 * @param tag The tag to use when selecting files to insert.
 */
void db_load_table(const char *tag);

#endif
