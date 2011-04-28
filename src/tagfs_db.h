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
 * @param file_id The file ID for which to return the full physical path.
 * @return The path to the physical location in the file system corresponding to the file ID the TagFS.
 */
char *db_get_file_location(int file_id);

#endif

