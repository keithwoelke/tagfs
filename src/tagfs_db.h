/**
 * Handles interaction with the sqlite3 database.
 * @file tagfs_db.h
 * @author Keith Woelke
 * @date 07/25/2010
 */

#ifndef TAGFS_DB_H
#define TAGFS_DB_H

/**
 * Creates a table with the name "thread_<thread id>".
 */
void db_create_table();

/**
 * Deletes a table with the name "thread_<thread id>".
 */
void db_delete_table();

#endif
