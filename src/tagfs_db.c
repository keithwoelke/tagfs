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











static int db_count_from_query(const char *query) {
	char *count_query = NULL;
	char select_count[] = "SELECT COUNT(*) FROM (";
	const char *tail = NULL;
	int count = 0;
	int count_query_length = 0;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(query != NULL);

	DEBUG("Calculating count from query: %s", query);

	count_query_length = strlen(query) + strlen(select_count) + 2; /* 2 for closing parenthesis and null terminating character */
	DEBUG("Query length: %d characters", count_query_length);
	count_query = calloc(count_query_length, sizeof(*count_query));
	assert(count_query != NULL);

	strcat(strcat(strcat(count_query, "SELECT COUNT(*) FROM ("), query), ")");

	(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, count_query, strlen(count_query), &res, &tail);

	assert(count_query != NULL);
	free(count_query);
	count_query = NULL;

	(void)sqlite3_step(res);
	count = sqlite3_column_int(res, 0);

	(void)sqlite3_finalize(res);

	DEBUG("Results returned from query: %d", count);
	DEBUG(EXIT);
	return count;
} /* db_count_from_query */

static char *db_query_files_with_tag(const char *tag) {
	char *query = NULL;
	const char select_from[] = "SELECT file_id FROM all_tables where tag_name == \"";

	assert(tag != NULL);

	DEBUG(ENTRY);

	query = calloc(strlen(select_from) + strlen(tag) + 2, sizeof(*query));
	assert(query != NULL);

	strcat(strcat(strcat(query, select_from), tag), "\"");

	DEBUG("Files from tag query: %s", query);
	DEBUG(EXIT);

	return query;
} /* db_query_files_with_tag */

void db_load_table(const char *tag) {
	char *file_query = NULL;
	char *insert_query = NULL;
	const char query[] = "INSERT INTO \"directory_contents\"(file_id) VALUES(";
	int file_count;
	int file_id = 0;
	int i = 0;
	int insert_query_length = 0;
	int rc = 0;
	sqlite3_stmt *file_res = NULL;
	sqlite3_stmt *insert_res = NULL;

	DEBUG(ENTRY);

	assert(tag != NULL);

	file_query = db_query_files_with_tag(tag);
	assert(file_query != NULL);
	DEBUG("Query to select files with tag \"%s\": %s", tag, file_query);

	file_count = db_count_from_query(file_query);
	assert(file_count >= 0);
	DEBUG("Loading %d files into table \"directory_contents\"", file_count);

	assert(TAGFS_DATA->db_conn != NULL);
	(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, file_query, strlen(file_query), &file_res, NULL);

	assert(file_query != NULL);
	free(file_query);
	file_query = NULL;

	assert(strcmp(sqlite3_column_name(file_res, 0), "file_id") == 0);

	for(i = 0; sqlite3_step(file_res) == SQLITE_ROW; i++) {
		file_id	= sqlite3_column_int(file_res, 0);
		assert(file_id > 0);
		DEBUG("Retrieving file id: %d", file_id);

		insert_query_length = strlen(query) + num_digits(file_id) + 1; /* + 1 for closing parenthesis */
		insert_query = malloc(insert_query_length * sizeof(*insert_query) + 1);
		assert(insert_query != NULL);
		snprintf(insert_query, insert_query_length + 1, "%s%d)", query,  file_id);
		DEBUG("Query to insert file ID \"%d\" into table \"directory_contents\": %s", file_id, insert_query);

		(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, insert_query, strlen(insert_query), &insert_res, NULL);

		assert(insert_query != NULL);
		free(insert_query);
		insert_query = NULL;

		rc = sqlite3_step(insert_res);
		sqlite3_finalize(insert_res);

		if(rc == SQLITE_DONE) {
			DEBUG("Inserting file ID \"%d\" into table \"directory_contents\" was a success", file_id);
		}
		else {
			WARN("Inserting file ID \"%d\" into table \"directory_contents\" FAILED with result code %d: %s", file_id, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		}
	}

	sqlite3_finalize(file_res);

	DEBUG(EXIT);
} /* db_load_table */
