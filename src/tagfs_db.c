#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <string.h>

/**
 * Enable foreign key constraints on the database.
 **/
static void db_enable_foreign_keys(sqlite3 *conn) {
	DEBUG(ENTRY);
	char *err_msg = NULL; /* sqlite3 error message */
	const char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int rc = 0; /* return code of sqlite3 operation */

	//DEBUG("Enabling foreign keys");

	/* execute statement */
	assert(conn != NULL);
	rc = sqlite3_exec(conn, enable_foreign_keys, NULL, NULL, &err_msg);

	/* handle return code */
	if(rc != SQLITE_OK) {
		if(err_msg != NULL) {
			DEBUG("Error when enabling foreign keys: %s", err_msg);
			sqlite3_free(err_msg);
		}

		ERROR("There was an error when enabling foreign keys.");
	}
	else { DEBUG("Foreign keys enabled successfully"); }

	DEBUG(EXIT);
} /* db_enable_foreign_keys */

/**
 * Connect to the database.
 */
static sqlite3 *db_connect() {
	DEBUG(ENTRY);
	int rc = 0; /* return code of sqlite3 operation */
	sqlite3 *conn = NULL;

	//DEBUG("Connecting to database: %s", TAGFS_DATA->db_path);

	/* connect to the database */
	assert(TAGFS_DATA->db_path != NULL);
	rc = sqlite3_open_v2(TAGFS_DATA->db_path, &conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(conn != NULL);

	/* handle result code */
	if(rc != SQLITE_OK) { /* if no space on drive or file does not exist */
		DEBUG("ERROR: %s", sqlite3_errmsg(conn));
		ERROR("An error occurred when connecting to the database");
	}
	//else { DEBUG("Database connection successful"); }

	/* enable foreign keys */
	db_enable_foreign_keys(conn);

	return conn;
	DEBUG(EXIT);
} /* db_connect */

const char *db_get_file_location(int file_id) {
	DEBUG(ENTRY);
	sqlite3 *conn = NULL;

	assert(file_id != 0);

	conn = db_connect();













	char *path = NULL;

	if(file_id == 1) {
		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg") * sizeof(*path) + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg");
		DEBUG("file id is %d for path %s.\n", 1, path);
		DEBUG(EXIT);
		return path;
	}
	else if(file_id == 2) {
		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg") * sizeof(*path) + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg");
		DEBUG("file id is %d for path %s.\n", 2, path);
		DEBUG(EXIT);
		return path;
	}
	else if(file_id == 3) {
		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov") * sizeof(*path) + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov");
		DEBUG("file id is %d for path %s.\n", 3, path);
		DEBUG(EXIT);
		return path;
	}
	else if(file_id == 4) {
		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt") * sizeof(*path) + 1);
		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt");
		DEBUG("file id is %d for path %s.\n", 4, path);
		DEBUG(EXIT);
		return path;
	}
	else {
		DEBUG("%d is not recognized.", file_id);
		return path;
	}
















	db_disconnect(conn);
	DEBUG(EXIT);
}

//const char *db_get_file_location(int file_id) {
//	DEBUG(ENTRY);
//	char *path = NULL;
//
//	if(file_id == 1) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/How fast.ogg");
//		DEBUG("file id is %d for path %s.\n", 1, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else if(file_id == 2) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/Josh Woodward - Swansong.ogg");
//		DEBUG("file id is %d for path %s.\n", 2, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else if(file_id == 3) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/sample_iTunes.mov");
//		DEBUG("file id is %d for path %s.\n", 3, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else if(file_id == 4) {
//		path = malloc(strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt") * sizeof(*path) + 1);
//		snprintf(path, strlen("/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt") + 1, "%s", "/home/keith/Programming/FUSE/TagFS/src/Files/Moby Dick.txt");
//		DEBUG("file id is %d for path %s.\n", 4, path);
//		DEBUG(EXIT);
//		return path;
//	}
//	else {
//		DEBUG("%d is not recognized.", file_id);
//		return path;
//	}
//}
