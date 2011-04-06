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
 * Returns a physical file system location corresponding to a location in the TagFS.
 *
 * @param path The path to the file in the TagFS.
 * @return The physical location in the file system corresponding to the file in the TagFS.
 */
const char *db_get_file_location(int file_id);

#endif

