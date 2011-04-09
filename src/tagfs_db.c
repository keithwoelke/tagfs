#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <sqlite3.h>
#include <string.h>

static void db_disconnect(sqlite3 *conn) {
	DEBUG(ENTRY);
	int rc = 0; /* return code of sqlite3 operations */

	//DEBUG("Disconnecting from the database");

	/* close database connection */
	assert(conn != NULL);
	rc = sqlite3_close(conn);

	/* handle result code */
	if(rc != SQLITE_OK) { /* has pending operations or prepared statements to be free'd */
		DEBUG("WARNING: %s", sqlite3_errmsg(conn));
		WARN("An error occured while disconnecting from the database");
	}
	else { DEBUG("Database disconnection successful"); }

	DEBUG(EXIT);
} /* db_disconnect */

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
	bool warn = false; /* whether or not a connection error should be displayed for the user */
	char *query = NULL;
	const char *file_location = NULL;
	const char query_outline[] = "SELECT file_location WHERE file_id == ";
	int query_length = 0;
	int rc = 0; /* return code of sqlite3 operation */
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	assert(file_id != 0);

	DEBUG("Retrieving physical location for file with ID %d", file_id);

	/* prepare query */
	query_length = strlen(query_outline) + num_digits(file_id);
	query = malloc(query_length * sizeof(*query) + 1);
	written = snprintf(query, query_length + 1, "SELECT file_location WHERE file_id == %d", file_id);
	assert(written == query_length);

	conn = db_connect();
	assert(conn != NULL);

	/* compile prepared statement */	
	assert(conn != NULL);
	rc = sqlite3_prepare_v2(conn, query, query_length, &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", query, query_length, rc, sqlite3_errmsg(conn));
		warn = true;
	}

	/* execute statement */
	rc = sqlite3_step(res);

	if(rc != SQLITE_ROW) {
		DEBUG("WARNING: Executing statement \"%s\" of length %d FAILED with result code %d: %s", query, query_length, rc, sqlite3_errmsg(conn));
		warn = true;
	}

	/* get file_id */
	file_location = (const char *)sqlite3_column_text(res, 0);

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", query, query_length, rc, sqlite3_errmsg(conn));
		warn = true;
	}

	db_disconnect(conn);

	assert(query != NULL);
	free(query);
	query = NULL;

	/* handle return code */
	if(warn == true) { WARN("An error occured when communicating with the database"); }
	else { DEBUG("File id %d corresponds to %s", file_id, file_location); }

	DEBUG(EXIT);
	return file_location;





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
