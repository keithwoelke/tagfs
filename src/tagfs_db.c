#include "tagfs_common.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

/**
 * Enable foreign key constraints on the database.
 */
static void db_enable_foreign_keys() {
	char *error_msg = NULL;
	char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int error = 0;

	DEBUG(ENTRY);

	assert(TAGFS_DATA->db_conn != NULL);

	error = sqlite3_exec(TAGFS_DATA->db_conn, enable_foreign_keys, NULL, NULL, &error_msg);

	if(error != SQLITE_OK) {
		ERROR("There was an error when enabling foreign keys: %s", error_msg);
		sqlite3_free(error_msg);
	}
	else {
		DEBUG("Foreign keys enabled successfully");
	}

	DEBUG(EXIT);
} /* db_enable_foreign_keys */

void db_connect() {
	int error = 0;

	DEBUG(ENTRY);
	DEBUG("Connecting to database: %s", TAGFS_DATA->db_path);

	error = sqlite3_open_v2(TAGFS_DATA->db_path, &TAGFS_DATA->db_conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(TAGFS_DATA->db_conn != NULL);

	if(error != SQLITE_OK) { /* if no space on drive or file does not exist */
		ERROR("An error occurred when connecting to the database: %s", sqlite3_errmsg(TAGFS_DATA->db_conn));
	}

	DEBUG("Database connection successful");
	db_enable_foreign_keys();

	DEBUG(EXIT);
} /* db_connect */

void db_disconnect() {
	int error = 0;

	DEBUG(ENTRY);

	assert(TAGFS_DATA->db_conn != NULL);

	error = sqlite3_close(TAGFS_DATA->db_conn);

	if(error != SQLITE_OK) /* has pending operations or prepared statements to be free'd */ {
		WARN("An error occured while disconnecting from the database: %s", sqlite3_errmsg(TAGFS_DATA->db_conn));
	}
	else {
		DEBUG("Database disconnection successful");
	}

	DEBUG(EXIT);
} /* db_disconnect */

void db_get_files_from_path(const char *path) {
	int num_tags = 0;
	char **tag_array = NULL;
	int i = 0;

	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_ENTRY, __FUNCTION__);
	num_tags = path_to_array(path, &tag_array);
	db_load_table(tag_array[0]);

	for(i = 1; i < num_tags; i++) {
		db_filter_table(tag_array[i]);
	}

	free_char_ptr_array(&tag_array, num_tags);
	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_EXIT, __FUNCTION__);
} /* db_get_files_from_path */
