#include "tagfs_common.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

static int db_enable_foreign_keys(sqlite3 *conn) {
	char *error_msg = NULL;
	char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int error = 0;

	DEBUG(ENTRY);

	assert(conn != NULL);

	error = sqlite3_exec(conn, enable_foreign_keys, NULL, NULL, &error_msg);

	if(error != SQLITE_OK) {
		DEBUG("There was an error when enabling foreign keys: %s", error_msg);
		sqlite3_free(error_msg);
	}
	else {
		DEBUG("Foreign keys enabled successfully");
	}

	DEBUG("db_enable_foreign_keys");
	return error;
} /* db_enable_foreign_keys */

static sqlite3* db_connect() {
	int error = 0;
	sqlite3 *conn = NULL;
	char *dbName = "/home/keith/Programming/FUSE/tagfs/src/tagfs.sl3";

	DEBUG("db_connect");

	assert(dbName != NULL);

	DEBUG("Connecting to database: %s", dbName);

	error = sqlite3_open_v2(dbName, &conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(conn != NULL);

	if(error != SQLITE_OK) { /* if no space on drive or file does not exist */
		DEBUG("An error occurred when connecting to the database: %s", sqlite3_errmsg(conn));
	}

	DEBUG("Database connection successful");
	(void)db_enable_foreign_keys(conn);

	DEBUG("db_connect");
	return conn;
} /* db_connect */

static int db_disconnect(sqlite3 *conn) {
	int error = 0;

	DEBUG("db_disconnect");

	assert(conn != NULL);

	error = sqlite3_close(conn);

	if(error != SQLITE_OK) /* has pending operations or prepared statements to be free'd */ {
		DEBUG("An error occured while disconnecting from the database: %s", sqlite3_errmsg(conn));
	}
	else {
		DEBUG("Database disconnection successful");
	}

	DEBUG("db_disconnect");
	return error;
} /* db_disconnect */

void db_delete_table() {
	char *delete_query = NULL;
	char query[] = "DROP TABLE \"thread_\")";
	int delete_query_length = 0;
	int rc = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	unsigned int tid = pthread_self();

	DEBUG(ENTRY);
	DEBUG("Deleting table: %u", tid);

	delete_query_length = strlen(query) + num_digits(tid) + 1;
	DEBUG("Delete query length is %d", delete_query_length);
	delete_query = malloc(delete_query_length * sizeof(*delete_query));
	assert(delete_query != NULL);
	snprintf(delete_query, delete_query_length, "DROP TABLE \"thread_%u\"", tid);
	DEBUG("Delete query: %s", delete_query);

	conn = db_connect();
	(void)sqlite3_prepare_v2(conn, delete_query, strlen(delete_query), &res, NULL);
	assert(delete_query != NULL);
	free(delete_query);
	delete_query = NULL;

	rc = sqlite3_step(res);

	(void)sqlite3_finalize(res);
	db_disconnect(conn);

	if(rc == SQLITE_DONE) {
		DEBUG("Table deletion was a SUCCESS");
	}
	else {
		ERROR("Table deletion FAILED with result code %d", rc);
	}

	DEBUG(EXIT);
} /* db_delete_table */

void db_create_table() {
	char *create_query = NULL;
	char query[] = "CREATE TABLE \"thread_\" (\"file_id\" INTEGER PRIMARY KEY NOT NULL)";
	int create_query_length = 0;
	int rc = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	const char *tail = NULL;
	unsigned int tid = pthread_self();

	DEBUG(ENTRY);
	DEBUG("Creating table: thread_%u", tid);

	create_query_length = strlen(query) + num_digits(tid) + 1;
	DEBUG("Create query length is %d", create_query_length);
	create_query = malloc(create_query_length * sizeof(*create_query));
	assert(create_query != NULL);
	snprintf(create_query, create_query_length, "CREATE TABLE \"thread_%u\" (\"file_id\" INTEGER PRIMARY KEY NOT NULL)", tid);
	DEBUG("Create query: %s", create_query);

	conn = db_connect();
	sqlite3_extended_result_codes(conn, 1);
	rc = sqlite3_prepare_v2(conn, create_query, strlen(create_query), &res, &tail);
	DEBUG("FOO: %d", rc);
	DEBUG("BAR: %s", sqlite3_errmsg(conn));
	assert(create_query != NULL);
	free(create_query);
	create_query = NULL;

	rc = sqlite3_step(res);

	(void)sqlite3_finalize(res);
	db_disconnect(conn);

	if(rc == SQLITE_DONE) {
		DEBUG("Table creation was a SUCCESS");
	}
	else {
		ERROR("Table creation FAILED with result code %d", rc);
	}

	DEBUG(EXIT);
} /* db_create_table */
