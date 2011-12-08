#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <glib.h>
#include <math.h>
#include <semaphore.h>
#include <sqlite3.h>
#include <string.h>

/**
 * Compiles an SQL statement into byte-code.
 *
 * @param conn A sqlite database handle.
 * @param query An SQL statement, UTF-8 encoded.
 * @param res OUT: A sqlite statement handle.
 * @return The result of the operation, corresponding to the return codes of the sqlite3_prepare_v2 function call.
 */
static int db_prepare_statement(sqlite3 *conn, char *query, sqlite3_stmt **res) {
	int query_length = 0;
	int rc = SQLITE_ERROR; /* return code of sqlite operation */

	DEBUG(ENTRY);

	assert(conn != NULL);
	assert(query != NULL);
	assert(*res == NULL);

	DEBUG("Compiling query: %s", query);

	query_length = strlen(query);

	rc = sqlite3_prepare_v2(conn, query, query_length, res, NULL);

	/* handle result code */
	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" failed with result code %d: %s", query, rc, sqlite3_errmsg(conn));
		WARN("An error occured while communicating with the database");
	}

	DEBUG("Query compilation was %ssuccessful", rc == SQLITE_DONE ? "" : "un");
	DEBUG(EXIT);
	return rc;
} /* db_prepare_statement */

/**
 * Run the sqlite statement.
 *
 * @param res A sqlite statement handle.
 * @return The result of the operation, corresponding to the return codes of the sqlite3_step function call.
 */
static int db_step_statement(sqlite3 *conn, char *query, sqlite3_stmt *res) {
	int rc = SQLITE_ERROR; /* return code of sqlite operation */

	DEBUG(ENTRY);

	assert(res != NULL);

	DEBUG("Executing query...");

	rc = sqlite3_step(res);

	/* handle result code */
	if(rc != SQLITE_ROW && rc != SQLITE_DONE) {
		DEBUG("WARNING: Executing statement \"%s\" failed with result code %d: %s", query, rc, sqlite3_errmsg(conn));
		WARN("An error occured while communicating with the database");
	}

	DEBUG("Query step complete with result code: %d", rc);
	DEBUG(EXIT);
	return rc;
} /* db_step_statement */

/*
 * Execute the SQL statement.
 *
 * @param conn A sqlite database handle.
 * @param query An SQL statement, UTF-8 encoded.
 * @param res OUT: A sqlite statement handle.
 * @return The result of the operation, corresponding to the return codes of the sqlite3_prepare_v2 function call (if it fails), otherwise the error code that is returned corresponds to the return codes of the sqlite3_step function call.
 */
static int db_execute_statement(sqlite3 *conn, char *query, sqlite3_stmt **res) {
	int rc = SQLITE_ERROR;

	DEBUG(ENTRY);

	assert(conn != NULL);
	assert(query != NULL);
	assert(*res == NULL);

	DEBUG("Executing query: %s", query);

	rc = db_prepare_statement(conn, query, res);

	/* handle result code */
	if(rc == SQLITE_OK) {
		rc = db_step_statement(conn, query, *res);
	}

	DEBUG("Query execution complete with result code: %d", rc);
	DEBUG(EXIT);
	return rc;
} /* db_execute_statement */

/**
 * Finalize a sqlite statement.
 *
 * @param conn A sqlite database handle.
 * @param query An SQL statement, UTF-8 encoded.
 * @param res A sqlite statement handle.
 * @return The result of the operation, corresponding to the return codes of the sqlite3_finalize function call.
 */
static int db_finalize_statement(sqlite3 *conn, char *query, sqlite3_stmt *res) {
	int rc = SQLITE_ERROR;

	DEBUG(ENTRY);

	assert(conn != NULL);
	assert(query != NULL);
	assert(res != NULL);

	DEBUG("Finalizing statement.");

	rc = sqlite3_finalize(res);

	/* handle result code */
	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" failed with result code %d: %s", query, rc, sqlite3_errmsg(conn));
		WARN("An error occured when communicating with the database");
	}

	DEBUG("Statement finalization completed with result code: %d", rc);
	DEBUG(EXIT);
	return rc;
} /* db_finalize_statement */

/**
 * Insert the results of the specified query into the specified table. Only the results of the first column are entered into the table, and the values are assumed to be integers.
 *
 * @param conn A sqlite database handle.
 * @param query An SQL statement, UTF-8 encoded.
 */
static void db_insert_query_results_into_hashtable(sqlite3 *conn, char *query, GHashTable *table) {
	int rc = SQLITE_ERROR;
	sqlite3_stmt *res = NULL;
	unsigned long int_from_table = 0;

	DEBUG(ENTRY);

	assert(query != NULL);
	assert(conn != NULL);
	assert(table != NULL);

	DEBUG("Inserting into table results from query: %s", query);

	db_execute_statement(conn, query, &res);

	/* insert results into hashset */
	do {
		int_from_table = sqlite3_column_int(res, 0);
		g_hash_table_insert(table, (gpointer)int_from_table, (gpointer)int_from_table);

		rc = db_step_statement(conn, query, res);
	} while(rc == SQLITE_ROW);

	db_finalize_statement(conn, query, res);

	DEBUG("Results entered into table from query: %s", query);
	DEBUG(EXIT);
} /* db_insert_query_results_into_hashtable */

/**
 * Disconnect from the database.
 *
 * @param conn The sqlite3 database connection to close.
 */
static void db_disconnect(sqlite3 *conn) {
	int rc = 0; /* return code of sqlite3 operations */

	DEBUG(ENTRY);

	assert(conn != NULL);

	DEBUG("Disconnecting from the database");

	/* close database connection */
	rc = sqlite3_close(conn);

	/* handle result code */
	if(rc != SQLITE_OK) { /* has pending operations or prepared statements to be free'd */
		DEBUG("WARNING: Disconnecting from database failed with result code %d: %s", rc, sqlite3_errmsg(conn));
		WARN("An error occured while disconnecting from the database");
	}
	else { DEBUG("Database disconnection successful"); }

	DEBUG(EXIT);
} /* db_disconnect */

/**
 * Enable foreign key constraints on the database.
 *
 * @param conn The sqlite3 database connection to enable foreign keys on.
 **/
static void db_enable_foreign_keys(sqlite3 *conn) {
	char *err_msg = NULL; /* sqlite3 error message */
	char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int rc = 0; /* return code of sqlite3 operation */

	DEBUG(ENTRY);

	assert(conn != NULL);

	DEBUG("Enabling foreign keys");

	/* execute statement */
	rc = sqlite3_exec(conn, enable_foreign_keys, NULL, NULL, &err_msg);

	/* handle return code */
	if(rc != SQLITE_OK) {
		if(err_msg != NULL) {
			DEBUG("ERROR: Enabling foreign keys failed with result code %d: %s", rc, sqlite3_errmsg(conn));
			sqlite3_free(err_msg);
		}

		ERROR("There was an error when enabling foreign keys.");
	}
	else { DEBUG("Foreign keys enabled successfully"); }

	DEBUG(EXIT);
} /* db_enable_foreign_keys */

/**
 * Connect to the database.
 *
 * @return The database connection handle.
 */
static sqlite3 *db_connect() {
	int rc = 0; /* return code of sqlite3 operation */
	sqlite3 *conn = NULL;

	DEBUG(ENTRY);
	DEBUG("Connecting to database: %s", TAGFS_DATA->db_path);

	/* connect to the database */
	assert(TAGFS_DATA->db_path != NULL);
	rc = sqlite3_open_v2(/*TAGFS_DATA->db_path*/"/home/keith/Programming/FUSE/TagFS/src/tagfs.sl3", &conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(conn != NULL);

	/* handle result code */
	if(rc != SQLITE_OK) { /* if no space on drive or file does not exist */
		DEBUG("ERROR: Connecting to the database failed with result code %d: %s", rc, sqlite3_errmsg(conn));
		ERROR("An error occurred when connecting to the database");
	}
	else { DEBUG("Database connection successful"); }

	db_enable_foreign_keys(conn);

	DEBUG(EXIT);
	return conn;
} /* db_connect */

char *db_get_file_location(int file_id) {
	char *file_location = NULL;
	char *query = NULL;
	char *tmp_file_directory = NULL; /* holds text from the query so it can be copied to a new memory location */
	char *tmp_file_name = NULL; /* hold name of file until it can be copied to a new memory location */
	char query_outline[] = "SELECT file_location, file_name FROM files WHERE file_id = ";
	int file_location_length = 0; /* length of the file location to return */
	int query_length = 0;
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(file_id > 0);

	DEBUG("Retrieving physical location for file with ID %d", file_id);

	/* prepare query */
	query_length = strlen(query_outline) + num_digits(file_id);
	query = malloc(query_length * sizeof(*query) + 1);
	written = snprintf(query, query_length + 1, "SELECT file_location, file_name FROM files WHERE file_id = %d", file_id);
	assert(written == query_length);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	db_execute_statement(conn, query, &res);

	/* get file location and name */
	tmp_file_directory = (char *)sqlite3_column_text(res, 0);
	assert(tmp_file_directory != NULL);
	tmp_file_name = (char *)sqlite3_column_text(res, 1);
	assert(tmp_file_name != NULL);

	/* build full file path from name and directory */
	file_location_length = strlen(tmp_file_directory) + strlen(tmp_file_name) + 1;
	file_location = malloc(file_location_length * sizeof(*file_location) + 1); 
	written = snprintf((char *)file_location, file_location_length + 1, "%s/%s", tmp_file_directory, tmp_file_name);
	assert(written == file_location_length);

	db_finalize_statement(conn, query, res);
	db_disconnect(conn);

	free_single_ptr((void *)&query);

	DEBUG("File id %d corresponds to %s", file_id, file_location);
	DEBUG(EXIT);
	return file_location;
} /* db_get_file_location */

int db_tags_from_files(int *files, int num_files, int **tags) {
	GHashTable *table = NULL;
	GHashTableIter iter;
	char *file_id_str = NULL;
	char *query = NULL;
	char or_outline[] = " OR file_id = ";
	char query_outline[] = "SELECT DISTINCT tag_id FROM all_tables WHERE file_id = ";
	const int QUERY_LENGTH_CAP = 1024;
	gpointer key = NULL;
	gpointer value = NULL;
	int file_id = 0;
	int hash_table_size = 0;
	int i = 0;
	int initial_query_length = 0;
	int num_digits_in_id = 0;
	int query_outline_length = 0;
	int sql_length_avail = 0;
	int sql_max_length = 0;
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;

	DEBUG(ENTRY);

	assert(files != NULL);
	assert(num_files > 0);
	assert(*tags == NULL);

	DEBUG("Retrieving tags from %d files", num_files);

	/* prepare query/table */
	query_outline_length = strlen(query_outline);
	table = g_hash_table_new(NULL, NULL); 
	assert(table != NULL);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	/* calculate maximum query length */
	sql_max_length = sqlite3_limit(conn, SQLITE_LIMIT_SQL_LENGTH, -1);
	assert(sql_max_length > query_outline_length);
	DEBUG("Database reports maximum query length of %d", sql_max_length);
	sql_max_length = sql_max_length < QUERY_LENGTH_CAP ? sql_max_length : QUERY_LENGTH_CAP;
	assert(sql_max_length > query_outline_length);
	DEBUG("Maximum query length has been set to %d", sql_max_length);

	/* for each file id */
	for(i = 0; i < num_files; i++) {
		num_digits_in_id = num_digits(files[i]);
		file_id = files[i];

		/* start new query */
		if(sql_length_avail <= 0) {
			DEBUG("Starting new query");
			sql_length_avail = sql_max_length - query_outline_length;
			assert(sql_length_avail > 0);

			query = calloc(sql_max_length + 1, sizeof(*query));
			assert(query != NULL);

			initial_query_length = query_outline_length + num_digits_in_id;
			written = snprintf(query, initial_query_length + 1, "%s%d", query_outline, file_id);
			assert(written == query_outline_length + num_digits_in_id);

			sql_length_avail -= num_digits_in_id;

			continue;
		}

		/* test if query can contain next file id */
		sql_length_avail -= strlen(or_outline) + num_digits(files[i]);

		/* if the query is at maximum length */
		if(sql_length_avail <= 0) {
			DEBUG("Query has reached maximum length...");
			i--; /* leave it for next iteration */

			db_insert_query_results_into_hashtable(conn, query, table);

			free_single_ptr((void *)&query);

			continue;
		}

		/* append file ID to query */
		file_id_str = calloc(num_digits_in_id + 1, sizeof(*file_id_str));
		snprintf(file_id_str, num_digits_in_id + 1, "%d", file_id);
		strncat(strncat(query, or_outline, strlen(or_outline)), file_id_str, strlen(file_id_str));
		free_single_ptr((void *)&file_id_str);
	}

	db_insert_query_results_into_hashtable(conn, query, table);

	free_single_ptr((void *)&query);
	db_disconnect(conn);

	/* copy results of set to array */
	hash_table_size = g_hash_table_size(table);
	*tags = malloc(hash_table_size * sizeof(**tags));

	g_hash_table_iter_init(&iter, table);

	i = 0;
	while(g_hash_table_iter_next(&iter, &key, &value)) {
		(*tags)[i++] = (unsigned long)value;
	}

	g_hash_table_destroy(table);

	DEBUG("Returning array of %d elements", hash_table_size);
	DEBUG(EXIT);
	return hash_table_size;
} /* db_tags_from_files */

int db_count_from_query(char *query) {
	char *count_query = NULL;
	char select_count_outline[] = "SELECT COUNT(*) FROM ()";
	int count = 0; /* number of rows returned from the count query */
	int length = 0; /* length of the sqlite3 count query */
	int written = 0; /* characters written by snprintf */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(query != NULL);

	DEBUG("Calculating number of results returned from query: %s", query);

	/* calculate query length */
	length = strlen(query) + strlen(select_count_outline);

	/* build count query */
	count_query = malloc(length * sizeof(*count_query) + 1);
	assert(count_query != NULL);
	written = snprintf(count_query, length + 1, "SELECT COUNT(*) FROM (%s)", query);
	assert(written == length);
	DEBUG("Complete query: %s", count_query);

	conn = db_connect();
	assert(conn != NULL);

	/* compile prepared statement */	
	db_execute_statement(conn, count_query, &res);

	/* get result of count */
	count = sqlite3_column_int(res, 0);

	db_finalize_statement(conn, count_query, res);
	db_disconnect(conn);
	free_single_ptr((void **)&count_query);

	DEBUG("Query returns a count of %d", count);
	DEBUG(EXIT);
	return count;
} /* db_count_from_query */

char *db_tag_name_from_tag_id(int tag_id) {
	char *query = NULL;
	char *tag_name = NULL;
	char query_outline[] = "SELECT tag_name FROM tags WHERE tag_id = ";
	int num_digits_in_id = 0;
	int query_length = 0;
	int query_outline_length = 0;
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(tag_id > 0);

	DEBUG("Retrieving tag name for tag ID %d", tag_id);

	/* prepare query */
	num_digits_in_id = num_digits(tag_id);
	query_outline_length = strlen(query_outline);
	query_length = query_outline_length + num_digits_in_id;
	query = malloc(query_length * sizeof(*query) + 1);
	assert(query != NULL);
	written = snprintf(query, query_length + 1, "%s%d", query_outline, tag_id);
	assert(written == query_length);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	db_execute_statement(conn, query, &res);

	/* get name corresponding to tag_id */
	tag_name = strdup((char *)sqlite3_column_text(res, 0));

	db_finalize_statement(conn, query, res);
	db_disconnect(conn);
	free_single_ptr((void **)&query);

	DEBUG("Tag ID %d corresponds to tag %s", tag_id, tag_name);
	DEBUG(EXIT);
	return tag_name;
} /* db_tag_name_from_tag_id */

int db_int_array_from_query(char *desired_column_name, char *result_query, int **result_array) {
	bool column_match = false;
	int result = 0;
	int column_count = 0;
	int desired_column_index = 0;
	int i = 0;
	int num_results = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(desired_column_name != NULL);
	assert(result_query != NULL);
	assert(*result_array == NULL);

	DEBUG("Creating array from the %s column of the query: %s", desired_column_name, result_query);

	num_results = db_count_from_query(result_query);

	if(num_results > 0) {
		*result_array = malloc(num_results * sizeof(**result_array));
		assert(*result_array != NULL);

		conn = db_connect();
		assert(conn != NULL);

		db_prepare_statement(conn, result_query, &res);
		column_count = sqlite3_column_count(res);

		for(desired_column_index = 0; desired_column_index < column_count; desired_column_index++) { /* find the requested column */
			if(strcmp(desired_column_name, sqlite3_column_name(res, desired_column_index)) == 0) { 
				DEBUG("Matching column found (%s)", desired_column_name);
				column_match = true;
				break; 
			}
		}

		if(column_match == false) {
			DEBUG("Matching column not found (%s)", desired_column_name);
			ERROR("Unexpected input received from database.");
		}

		for(i = 0; db_step_statement(conn, result_query, res) == SQLITE_ROW; i++) {
			result = sqlite3_column_int(res, desired_column_index); 
			(*result_array)[i] = result;
		}

		db_finalize_statement(conn, result_query, res);
		db_disconnect(conn);
	}

	DEBUG(EXIT);
	return num_results;
} /* db_int_array_from_query */

int db_get_all_tags(int **tags) {
	char query[] = "SELECT tag_id FROM tags";
	int count = 0;
	sqlite3 *conn = NULL;

	DEBUG(ENTRY);

	assert(*tags == NULL);

	DEBUG("Retrieving all tags");

	conn = db_connect();
	assert(conn != NULL);

	count = db_int_array_from_query("tag_id", query, tags);
	db_disconnect(conn);

	DEBUG("Returning a list of %d tags", count);
	DEBUG(EXIT);
	return count;
} /* db_get_all_tags */

int db_files_from_tag_id(int tag_id, int **files) {
	char query_outline[] = "SELECT file_id FROM all_tables WHERE tag_id = ";
	int query_outline_length = 0;
	char *query = NULL;
	int count = 0;
	int query_length = 0;
	int written = 0; /* number of characters written */
	int num_digits_in_id = 0;

	DEBUG(ENTRY);

	assert(*files == NULL);

	DEBUG("Retrieving files from tag ID %d", tag_id);

	if(tag_id > 0) {
		/* prepare query */
		num_digits_in_id = num_digits(tag_id);
		query_outline_length = strlen(query_outline);
		query_length = query_outline_length + num_digits_in_id;
		query = malloc(query_length * sizeof(*query) + 1);
		assert(query != NULL);
		written = snprintf(query, query_length + 1, "%s%d", query_outline, tag_id);
		assert(written == query_length);

		count = db_int_array_from_query("file_id", query, files);
		free_single_ptr((void **)&query);
	} else {
		count = db_int_array_from_query("file_id", "SELECT * FROM files WHERE file_id NOT IN (SELECT file_id FROM file_has_tag)", files);
	}

	DEBUG("Returning a list of %d files", count);
	DEBUG(EXIT);
	return count;
} /* db_files_from_tag_id */

int db_tag_id_from_tag_name(char *tag_name) {
	char *query = NULL;
	int tag_id = 0;
	char query_outline[] = "SELECT tag_id FROM tags WHERE tag_name = \"\"";
	int query_length = 0;
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(tag_name != NULL);

	DEBUG("Retrieving tag name for tag ID %s", tag_name);

	if(strcmp(tag_name, "/") == 0) {
		tag_id = 0;
	} else {
		/* prepare query */
		query_length = strlen(query_outline) + strlen(tag_name);
		query = malloc(query_length * sizeof(*query) + 1);
		assert(query != NULL);
		written = snprintf(query, query_length + 1, "SELECT tag_id FROM tags WHERE tag_name = \"%s\"", tag_name);
		assert(written == query_length);

		/* connect to database */
		conn = db_connect();
		assert(conn != NULL);

		db_execute_statement(conn, query, &res);

		/* get name corresponding to tag_id */
		tag_id = sqlite3_column_int(res, 0);

		if(tag_id == 0) {
			tag_id = -1;
		}

		db_finalize_statement(conn, query, res);
		db_disconnect(conn);
		free_single_ptr((void **)&query);
	}

	DEBUG("Tag %s corresponds to tag ID %d", tag_name, tag_id);
	DEBUG(EXIT);
	return tag_id;
} /* db_tag_id_from_tag_name */

void db_delete_file(int file_id) {
	char *query = NULL;
	char query_outline[] = "DELETE FROM files WHERE file_id = \"\"";
	int query_length = 0;
	int rc = SQLITE_ERROR; /* return code of sqlite operation */
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(file_id > 0);

	DEBUG("Deleting file with file ID %d", file_id);

	/* prepare query */
	query_length = strlen(query_outline) + num_digits(file_id);
	query = malloc(query_length * sizeof(*query) + 1);
	assert(query != NULL);
	written = snprintf(query, query_length + 1, "DELETE FROM files WHERE file_id = \"%d\"", file_id);
	assert(written == query_length);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	rc = db_execute_statement(conn, query, &res);

	db_finalize_statement(conn, query, res);
	db_disconnect(conn);
	free_single_ptr((void **)&query);

	DEBUG("File with ID %d was %sdeleted successfully.", file_id, rc == 0 ? "" : "not ")
	DEBUG(EXIT);
} /* db_delete_file */

void db_delete_tag(int tag_id) {
	char *query = NULL;
	char query_outline[] = "DELETE FROM tags WHERE tag_id = \"\"";
	int query_length = 0;
	int rc = SQLITE_ERROR; /* return code of sqlite operation */
	int written = 0; /* number of characters written */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(tag_id > 0);

	DEBUG("Deleting tag with tag ID %d", tag_id);

	/* prepare query */
	query_length = strlen(query_outline) + num_digits(tag_id);
	query = malloc(query_length * sizeof(*query) + 1);
	assert(query != NULL);
	written = snprintf(query, query_length + 1, "DELETE FROM tags WHERE tag_id = \"%d\"", tag_id);
	assert(written == query_length);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	rc = db_execute_statement(conn, query, &res);

	db_finalize_statement(conn, query, res);
	db_disconnect(conn);
	free_single_ptr((void **)&query);

	DEBUG("File with ID %d was %sdeleted successfully.", tag_id, rc == 0 ? "" : "not ")
	DEBUG(EXIT);
} /* db_delete_tag */

void db_delete_empty_tags() {
	int rc = SQLITE_ERROR; /* return code of sqlite operation */
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	char query[] = "DELETE FROM tags WHERE tag_name NOT IN (SELECT DISTINCT tag_name FROM all_tables)";

	DEBUG(ENTRY);
	DEBUG("Preparing to purge all empty tags from the database.");

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	rc = db_execute_statement(conn, query, &res);

	db_finalize_statement(conn, query, res);
	db_disconnect(conn);

	DEBUG("Purging empty tags was %ssuccessful", rc == SQLITE_DONE ? "" : "not");
	DEBUG(EXIT);
} /* db_delete_empty_tags */
