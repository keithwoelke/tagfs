/**
 * Handles interaction with the sqlite3 database.
 * @file tagfs_db.h
 * @author Keith Woelke
 * @date 07/25/2010
 */

#ifndef TAGFS_DB_H
#define TAGFS_DB_H

/**
 * Creates a table with the name "thread_<thread_id>".
 *
 * @return The SQL result code of the table create operation.
 */
void db_create_table();

/**
 * Deletes the table with the name "thread_<thread_id>".
 *
 * @return The SQL result code of the table drop operation.
 */
void db_delete_table();







//#include <sqlite3.h>
//#include <stdbool.h>
//
//extern char *db_location;
//
///**
// * Retrieves an array of files that reside at a location in the file system. The array that is passed in must be free'd to avoid memory leaks. This can be easily done using the free_char_ptr_array function.
// * @param path a string representing a path in the file system.
// * @param file_array an array of the files that reside at the specified path.
// * @return the number of files in the array
// */
//int db_files_from_restricted_query(const char *path, /*@out@*/ char ***file_array);
//
//int db_files_from_query(const char *path, /*@out@*/ char ***file_array);
//
///**
// * Retrieves an array of tags that reside at a location in the file system. The array that is passed in must be free'd to avoid memory leaks. This can be easily done using the free_char_ptr_array function.
// * @param path a string representing a path in the file system.
// * @param tag_array an array of the tags that reside at the specified path.
// * @return the number of tags in the array
// */
//int db_tags_from_query(const char *path, /*@out@*/ char ***tag_array);
//
//void db_delete_file(const char *path);
//
//const char* get_file_location(const char *path);

#endif
