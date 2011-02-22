#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <string.h>

/**
 * Enable foreign key constraints on the database.
 **/
static void db_enable_foreign_keys() {
	DEBUG(ENTRY);
	char *err_msg = NULL; /* sqlite3 error message */
	const char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int rc = 0; /* return code of sqlite3 operations */

	DEBUG("Enabling foreign keys");

	/* execute statement */
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_exec(TAGFS_DATA->db_conn, enable_foreign_keys, NULL, NULL, &err_msg);

	/* handle return code */
	if(rc != SQLITE_OK) {
		DEBUG("Error when enabling foreign keys: %s", err_msg);
		ERROR("There was an error when enabling foreign keys.");
		sqlite3_free(err_msg);
	}
	else { DEBUG("Foreign keys enabled successfully"); }

	DEBUG(EXIT);
} /* db_enable_foreign_keys */

/*
 * Copies the results of a database query into a pre-existing table. This function assumes that the result set has only one column, or more specifically, it only copies the first column.
 *
 * @param src The statement handle for the compiled query.
 * @param dest The name of the table to copy the results to.
 **/
static void db_copy_result_set(sqlite3_stmt *src, const char *dest) {
	DEBUG(ENTRY);
	bool warn = false; /* whether or not a user visible warning should print */
	char *query = NULL;
	const char query_outline[] = "INSERT INTO (file_id) VALUES()";
	int file_id = 0;
	int i = 0;
	int length = 0; /* length of sqlite3 query */
	int rc = 0; /* return code of sqlite3 operations */
	int written = 0; /* characters written by snprintf */
	sqlite3_stmt *res = NULL;

	assert(src != NULL);
	assert(dest != NULL);
	assert(strcmp(dest, "") != 0);

	DEBUG("Copying result set to table \"%s\"", dest); 

	for(i = 0; sqlite3_step(src) == SQLITE_ROW; i++) {
		/* get file_id from database */
		file_id	= sqlite3_column_int(src, 0);
		DEBUG("Retrieving file id: %d", file_id);

		/* create query to insert file_id into database */
		length = strlen(query_outline) + strlen(dest) + num_digits(file_id); /* + 1 for closing parenthesis */
		assert(length > 0);
		query = malloc(length * sizeof(*query) + 1);
		assert(query != NULL);
		written = snprintf(query, length + 1, "INSERT INTO %s(file_id) VALUES(%d)", dest,  file_id);
		assert(written == length);
		DEBUG("Query to insert file ID \"%d\" into table \"%s\": %s", file_id, dest, query);

		/* compile prepared statement */
		assert(TAGFS_DATA->db_conn != NULL);
		rc = sqlite3_prepare_v2(TAGFS_DATA->db_conn, query, length, &res, NULL);

		if(rc != SQLITE_OK) {
			DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
			warn = true;
		}

		/* execute statement */
		rc = sqlite3_step(res);

		if(rc != SQLITE_DONE) {
			DEBUG("WARNING: Executing statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
			warn = true;
		}

		rc = sqlite3_finalize(res);		

		if(rc != SQLITE_OK) {
			DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
			warn = true;
		}

		assert(query != NULL);
		free(query);
		query = NULL;

		/* handle return code */
		if(warn == true) { WARN("An error occured when communicating with the database"); }
		else { DEBUG("Inserting file ID \"%d\" into table \"%s\" was a success", file_id, dest); }
	}

	DEBUG(EXIT);
} /* db_copy_result_set */

/**
 * Return a query which will select all files which contain the specified tag. The returned query must be free'd by the caller.
 *
 * @param tag The tag which will be used to construct the select query.
 * @return The query which can be used to select all files containing the specified tag.
 **/
static char *db_query_files_with_tag(const char *tag) {
	DEBUG(ENTRY);
	char *query = NULL;
	const char select_from_outline[] = "SELECT file_id FROM all_tables where tag_name == \"\"";
	int length = 0; /* length of sqlite3 query */
	int written = 0; /* characters written by snprintf */

	assert(tag != NULL);

	DEBUG("Building query to select all files with tag \"%s\"", tag);

	/* calculating length */
	length = strlen(select_from_outline) + strlen(tag);
	DEBUG("Length of query to select files with tag \"%s\" is %d", tag, length);

	/* building query */
	query = malloc(length * sizeof(*query) + 1);
	assert(query != NULL);
	written = snprintf(query, length + 1, "SELECT file_id FROM all_tables where tag_name == \"%s\"", tag);
	assert(written == length);

	DEBUG("Query to select files with tag \"%s\": %s", tag, query);
	DEBUG(EXIT);
	return query;
} /* db_query_files_with_tag */

/**
 * Return the number of rows returned from the specified query.
 *
 * @param query The query which will be run to count the results.
 * @return The number of rows returned by the query.
 */
static int db_count_from_query(const char *query) {
	DEBUG(ENTRY);
	bool warn = false; /* whether or not a user visible warning should print */
	char *count_query = NULL;
	const char select_count_outline[] = "SELECT COUNT(*) FROM ()";
	int count = 0; /* number of rows returned from the count query */
	int length = 0; /* length of the sqlite3 count query */
	int rc = 0; /* return code of sqlite3 operations */
	int written = 0; /* characters written by snprintf */
	sqlite3_stmt *res = NULL;

	assert(query != NULL);

	DEBUG("Calculating number of results returned from query: %s", query);

	/* calculate query length */
	length = strlen(query) + strlen(select_count_outline);
	DEBUG("Length of query to count the results: %d", length);

	/* build count query */
	count_query = malloc(length * sizeof(*query) + 1);
	assert(count_query != NULL);
	written = snprintf(count_query, length + 1, "SELECT COUNT(*) FROM (%s)", query);
	assert(written == length);

	/* compile prepared statement */	
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_prepare_v2(TAGFS_DATA->db_conn, count_query, length, &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", count_query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* execute statement */
	rc = sqlite3_step(res);

	if(rc != SQLITE_ROW) {
		DEBUG("WARNING: Executing statement \"%s\" of length %d FAILED with result code %d: %s", count_query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* get result of count */
	count = sqlite3_column_int(res, 0);

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", count_query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	assert(count_query != NULL);
	free(count_query);
	count_query = NULL;

	/* handle return code */
	if(warn == true) { WARN("An error occured when communicating with the database"); }
	else { DEBUG("Results returned from query: %d", count); }

	DEBUG(EXIT);
	return count;
} /* db_count_from_query */

/**
 * Loads into the specified table, all file IDs which have the specificed tag. If the table is not already empty, the ids will still be inserted.
 *
 * @param tag The tag to use when selecting files to insert.
 * @param table the table to insert the file IDs into.
 */
static void db_load_table(const char *tag, const char *table) {
	DEBUG(ENTRY);
	bool warn = false; /* whether or not a user visible warning should print */
	char *query = NULL;
	int count = 0; /* number of rows returned from the query */
	int length = 0; /* length of the sqlite3 query */
	int rc = 0; /* return code of sqlite3 operations */
	sqlite3_stmt *res = NULL;

	assert(tag != NULL);

	DEBUG("Loading results with tag \"%s\" into table \"%s\"", tag, table);

	/* get query and number of files from query */
	query = db_query_files_with_tag(tag);
	assert(query != NULL);
	DEBUG("Query to select files with tag \"%s\": %s", tag, query);
	length = strlen(query);
	DEBUG("Length of query to select files with tag \"%s\" is %d", tag, length);

	count = db_count_from_query(query);
	assert(count >= 0);
	DEBUG("Query returns %d files", count);

	/* compile prepared statement */
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_prepare_v2(TAGFS_DATA->db_conn, query, length, &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* insert results of query into table table */
	assert(strcmp(sqlite3_column_name(res, 0), "file_id") == 0);
	db_copy_result_set(res, table);

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	assert(query != NULL);
	free(query);
	query = NULL;

	if(warn == true) { WARN("An error occured when communicating with the database"); }
	else { DEBUG("Table \"%s\" succesfully loaded with files having tag \"%s\"", table, tag); }

	DEBUG(EXIT);
} /* db_load_table */

/**
 * Delete all rows from the specified table.
 *
 * @param table the name of the table to truncate.
 */
static void db_truncate_table(const char *table) {
	DEBUG(ENTRY);
	bool warn = false; /* whether or not a user visible warning should print */
	char *query = NULL;
	const char delete_from_outline[] = "DELETE FROM ";
	int length = 0; /* length of the sqlite3 query */
	int rc = 0; /* return code of sqlite3 operations */
	int written = 0; /* characters written by snprintf */
	sqlite3_stmt *res = NULL;

	assert(table != NULL);

	DEBUG("Truncating table \"%s\"", table); 

	/* calculate query length */
	length = strlen(delete_from_outline) + strlen(table);
	query = malloc(length * sizeof(*query) + 1);
	written = snprintf(query, length + 1, "DELETE FROM %s", table);
	assert(written == length);

	/* compile prepared statement */
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_prepare_v2(TAGFS_DATA->db_conn, query, length, &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* execute statement */
	rc = sqlite3_step(res);

	if(rc != SQLITE_DONE) {
		DEBUG("WARNING: Executing statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", query, length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	assert(query != NULL);
	free(query);
	query = NULL;

	if(warn == true) { WARN("An error occured when communicating with the database"); }
	else { DEBUG("Table \"%s\" succesfully truncated", table); }

	DEBUG(EXIT);
} /* db_truncate_table */

void db_connect() {
	DEBUG(ENTRY);
	int rc = 0; /* return code of sqlite3 operations */

	DEBUG("Connecting to database: %s", TAGFS_DATA->db_path);

	/* connect to the database */
	assert(TAGFS_DATA->db_path != NULL);
	assert(TAGFS_DATA->db_conn == NULL);
	rc = sqlite3_open_v2(TAGFS_DATA->db_path, &TAGFS_DATA->db_conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(TAGFS_DATA->db_conn != NULL);

	/* handle result code */
	if(rc != SQLITE_OK) { /* if no space on drive or file does not exist */
		DEBUG("ERROR: %s", sqlite3_errmsg(TAGFS_DATA->db_conn));
		ERROR("An error occurred when connecting to the database");
	}
	else { DEBUG("Database connection successful"); }

	/* enable foreign keys */
	db_enable_foreign_keys();

	DEBUG(EXIT);
} /* db_connect */

void db_disconnect() {
	DEBUG(ENTRY);
	int rc = 0; /* return code of sqlite3 operations */

	DEBUG("Disconnecting from the database");

	/* close database connection */
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_close(TAGFS_DATA->db_conn);

	/* handle result code */
	if(rc != SQLITE_OK) { /* has pending operations or prepared statements to be free'd */
		DEBUG("ERROR: %s", sqlite3_errmsg(TAGFS_DATA->db_conn));
		WARN("An error occured while disconnecting from the database");
	}
	else { DEBUG("Database disconnection successful"); }

	DEBUG(EXIT);
} /* db_disconnect */

void db_set_directory_contents(const char *path, const char *table) {
	DEBUG(ENTRY);
	int num_tags = 0; /* number of tags in the path */
	char **tag_array = NULL; /* array of path elements */
	int i = 0;

	assert(path != NULL);
	assert(table != NULL);

	DEBUG("Loading the contents of %s into table \"%s\"", path, table);

	/* turn path into array and get count */
	num_tags = path_to_array(path, &tag_array);
	DEBUG("%s has %d tags", path, num_tags);

	/* truncate the table */
	db_truncate_table(table);

	if(strcmp(path, "/") != 0) {
		/* load first tag into table */
		assert(tag_array != NULL);
		assert(tag_array[0] != NULL);
		db_load_table(tag_array[0], table);

		/* filter the rest out that do not have subsequent tags */
		for(i = 1; i < num_tags; i++) {
			db_filter_table(tag_array[i], table);
		}

		free_char_ptr_array(&tag_array, num_tags);
	}

	DEBUG(EXIT);
} /* db_set_directory_contents */

/* TODO: This approach may be too slow. May be better to use arrays, or copy to intermediate table and rename. */
void db_filter_table(const char *tag, const char *table) {
	DEBUG(ENTRY);
	const char *select_from = "SELECT * FROM directory_intermediate";
	bool warn = false; /* whether or not a user visible warning should print */
	char *file_query = NULL; /* sqlite3 query to select files with tag */
	char *filter_query = NULL; /* sqlite3 query to select matching files in file_query and table */
	const char filter_query_outline[] = "SELECT file_id FROM  WHERE file_id IN ()";
	int filter_query_length = 0;
	int rc = 0; /* return code of sqlite3 operations */
	int written = 0; /* characters written by snprintf */
	sqlite3_stmt *res = NULL;

	assert(tag != NULL);
	assert(table != NULL);

	DEBUG("Filtering rows in table \"%s\" that do not contain the tag \"%s\"", table, tag);

	/* build query to select potential files of interest (by tag) */
	file_query = db_query_files_with_tag(tag);
	DEBUG("Query to select files with tag \"%s\": %s", tag, file_query);

	/* select files of interest that appear in table */
	filter_query_length = strlen(filter_query_outline) + strlen(file_query) + strlen(table);
	filter_query = malloc(filter_query_length * sizeof(*filter_query) + 1);
	assert(filter_query != NULL);

	written = snprintf(filter_query, filter_query_length + 1, "SELECT file_id FROM %s WHERE file_id IN (%s)", table, file_query);
	assert(written == filter_query_length);
	DEBUG("Query to filter out files not containing tag \"%s\": %s", tag, filter_query);

	assert(file_query != NULL);
	free(file_query);
	file_query = NULL;

	/* compile prepared statement */
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_prepare_v2(TAGFS_DATA->db_conn, filter_query, filter_query_length, &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", filter_query, filter_query_length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* truncate intermediate table */
	db_truncate_table("directory_intermediate");

	/* insert results of query into intermediate table */
	assert(strcmp(sqlite3_column_name(res, 0), "file_id") == 0);
	db_copy_result_set(res, "directory_intermediate");

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", filter_query, filter_query_length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* truncate result table */
	db_truncate_table(table);

	/* compile prepared statement */
	rc = sqlite3_prepare_v2(TAGFS_DATA->db_conn, select_from, strlen(select_from), &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", filter_query, filter_query_length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* insert results of query into result table */
	assert(strcmp(sqlite3_column_name(res, 0), "file_id") == 0);
	db_copy_result_set(res, "directory_contents");

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", filter_query, filter_query_length, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
		warn = true;
	}

	/* truncate intermediate table */
	db_truncate_table("directory_intermediate");

	assert(filter_query != NULL);
	free(filter_query);
	filter_query = NULL;

	if(warn == true) { WARN("An error occured when communicating with the database"); }
	else { DEBUG("Table \"%s\" succesfully filtered with tag \"%s\"", table, tag); }

	DEBUG(EXIT);
} /* db_filter_table */












void db_get_tags_from_query(const char *path, char ***tag_array) {
	const char select_distinct[] = "SELECT DISTINCT tag_name FROM all_tables WHERE file_id IN (SELECT file_id FROM directory_contents)";
} /* db_get_tags_from_query */
