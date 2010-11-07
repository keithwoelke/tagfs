module tagfs_db;
/**
 * Handles interaction with the sqlite3 database.
 * @file tagfs_db.h
 * @author Keith Woelke
 * @date 25/07/2010
 */

//#ifndef TAGFS_DB_H
//#define TAGFS_DB_H
//
//#include <sqlite3.h>
//#include <stdbool.h>
//
//#define DB_LOCATION "/home/he/devel/src/byproject/keith/tagfs.sl3"

extern(C) :

/**
 * Retrieves an array of files that reside at a location in the file system. The array that is passed in must be free'd to avoid memory leaks. This can be easily done using the free_char_ptr_array function.
 * @param path a string representing a path in the file system.
 * @param file_array an array of the files that reside at the specified path.
 * @return the number of files in the array
 */
int db_files_from_query(const char *path, const(char *)**file_array);

/**
 * Retrieves an array of tags that reside at a location in the file system. The array that is passed in must be free'd to avoid memory leaks. This can be easily done using the free_char_ptr_array function.
 * @param path a string representing a path in the file system.
 * @param tag_array an array of the tags that reside at the specified path.
 * @return the number of tags in the array
 */
int db_tags_from_query(const char *path, const(char *)**tag_array);

void db_delete_file(const char *path);

//#endif
