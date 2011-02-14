#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <sqlite3.h>
#include <string.h>

/**
 * Enable foreign key constraints on the database.
 **/
static void db_enable_foreign_keys() {
	DEBUG(ENTRY);
	char *err_msg = NULL;
	const char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int rc = 0;

	/* execute statement */
	assert(TAGFS_DATA->db_conn != NULL);
	rc = sqlite3_exec(TAGFS_DATA->db_conn, enable_foreign_keys, NULL, NULL, &err_msg);

	/* handle return code */
	if(rc != SQLITE_OK) {
		DEBUG("ERROR: %s", err_msg);
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
	char *query = NULL;
	const char insert_into[] = "INSERT INTO \"directory_contents\"(file_id) VALUES(";
	int file_id = 0;
	int i = 0;
	int length = 0;
	int rc = 0;
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
		length = strlen(insert_into) + num_digits(file_id) + 1; /* + 1 for closing parenthesis */
		assert(length > 0);
		query = malloc(length * sizeof(*query) + 1);
		assert(query != NULL);
		snprintf(query, length + 1, "%s%d)", insert_into,  file_id);
		DEBUG("Query to insert file ID \"%d\" into table \"%s\": %s", file_id, dest, query);

		/* compile prepared statement */
		(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, query, strlen(query), &res, NULL);

		assert(query != NULL);
		free(query);
		query = NULL;

		/* execute statement */
		rc = sqlite3_step(res);
		(void)sqlite3_finalize(res);

		/* handle return code */
		if(rc == SQLITE_DONE) {	DEBUG("Inserting file ID \"%d\" into table \"%s\" was a success", file_id, dest); }
		else {
			DEBUG("WARNING: Inserting file ID \"%d\" into table \"%s\" FAILED with result code %d: %s", file_id, dest, rc, sqlite3_errmsg(TAGFS_DATA->db_conn));
			WARN("An error occured when communicating with the database");
		}
	}
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
	const char select_from[] = "SELECT file_id FROM all_tables where tag_name == \"";
	int length = 0;

	assert(tag != NULL);

	DEBUG("Building query to select all files with tag \"%s\"", tag);

	/* calculating length */
	length = strlen(select_from) + strlen(tag) + 1; /* + 1 for the closing quotation mark */
	DEBUG("Length of query to select files with tag \"%s\" is %d", tag, length);

	/* building query */
	query = malloc(length * sizeof(*query) + 1);
	assert(query != NULL);
	snprintf(query, length + 1, "%s%s\"", select_from, tag);

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
	char *count_query = NULL;
	const char select_count[] = "SELECT COUNT(*) FROM (";
	int count = 0;
	int length = 0;
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(query != NULL);

	DEBUG("Calculating number of results returned from query: %s", query);

	/* calculate query length */
	length = strlen(query) + strlen(select_count) + 1; /* + 1 for closing parenthesis */
	DEBUG("Length of query to count the results: %d", length);

	/* build count query */
	count_query = malloc(length * sizeof(*query) + 1);
	assert(count_query != NULL);
	snprintf(count_query, length + 1, "%s%s)", select_count, query);

	/* compile prepared statement */	
	(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, count_query, strlen(count_query), &res, NULL);

	assert(count_query != NULL);
	free(count_query);
	count_query = NULL;

	/* execute statement */
	(void)sqlite3_step(res);

	/* get result of count */
	count = sqlite3_column_int(res, 0);

	(void)sqlite3_finalize(res);

	DEBUG("Results returned from query: %d", count);
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
	char *query = NULL;
	int count = 0;
	sqlite3_stmt *res = NULL;

	assert(tag != NULL);

	/* get number of files in directory */
	query = db_query_files_with_tag(tag);
	assert(query != NULL);
	DEBUG("Query to select files with tag \"%s\": %s", tag, query);

	count = db_count_from_query(query);
	assert(count >= 0);
	DEBUG("Query returns %d files", count);

	/* compile prepared statement */
	assert(TAGFS_DATA->db_conn != NULL);
	(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, query, strlen(query), &res, NULL);

	assert(query != NULL);
	free(query);
	query = NULL;

	/* insert results of query into table table */
	assert(strcmp(sqlite3_column_name(res, 0), "file_id") == 0);
	db_copy_result_set(res, table);

	(void)sqlite3_finalize(res);

	DEBUG(EXIT);
} /* db_load_table */

/**
 * Delete all rows from the specified table.
 *
 * @param table the name of the table to truncate.
 */
static void db_truncate_table(const char *table) {
	DEBUG(ENTRY);
	char *query = NULL;
	const char delete_from[] = "DELETE FROM ";
	int length = 0;
	sqlite3_stmt *res = NULL;

	assert(table != NULL);

	DEBUG("Truncating table \"%s\"", table); 

	/* calculate query length */
	length = strlen(delete_from) + strlen(table);
	query = malloc(length * sizeof(*query) + 1);
	snprintf(query, length + 1, "DELETE FROM %s", table);

	/* compile prepared statement */
	(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, query, strlen(query), &res, NULL);
	
	assert(query != NULL);
	free(query);
	query = NULL;

	/* execute statement */
	sqlite3_step(res);

	(void)sqlite3_finalize(res);

	DEBUG(EXIT);
} /* db_truncate_table */

void db_connect() {
	DEBUG(ENTRY);
	int rc = 0;

	DEBUG("Connecting to database: %s", TAGFS_DATA->db_path);

	/* connect to the database */
	assert(TAGFS_DATA->db_path != NULL);
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
	int rc = 0;

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
	int num_tags = 0;
	char **tag_array = NULL;
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















static int db_array_from_query(char *desired_column_name, const char *result_query, /*@out@*/ char ***result_array) {
	bool column_match = false;
	const char *tail = NULL;
	const unsigned char *result = NULL;
	int column_count = 0;
	int desired_column_index = 0;
	int i = 0;
	int num_results = 0;
	sqlite3_stmt *res = NULL;


	assert(result_query != NULL);
	assert(result_array != NULL);
	assert(*result_array == NULL);


	num_results = db_count_from_query(result_query);

	if(num_results > 0) {
		*result_array = malloc(num_results * sizeof(**result_array));
		assert(*result_array != NULL);

		(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, result_query, strlen(result_query), &res, &tail);
		column_count = sqlite3_column_count(res);

		for(desired_column_index = 0; desired_column_index < column_count; desired_column_index++) { /* find the requested column */
			if(strcmp(desired_column_name, sqlite3_column_name(res, desired_column_index)) == 0) { 
				column_match = true;
				break; 
			}
		}

		if(column_match == false) {
		}
		for(i = 0; sqlite3_step(res) == SQLITE_ROW; i++) {
			result = sqlite3_column_text(res, desired_column_index); 
			(*result_array)[i] = malloc(result == NULL ? sizeof(NULL) : strlen((const char *)result) * sizeof(*result) + 1);
			assert((*result_array)[i] != NULL);
			if(result != NULL) {
				strcpy((*result_array)[i], (char*)result);
			}
			else {
				(*result_array)[i] = NULL;
			}

		}

		(void)sqlite3_finalize(res);
	}
	else {
	}

	return num_results;
} /* db_array_from_query */










void db_filter_table(const char *tag, const char *table) {
	char **file_array = NULL;
	char *file_query = NULL;
	char *filter_query = NULL;
	char filter_query_outline[] = "SELECT file_id FROM thread_ WHERE file_id IN ()";
	char *insert_query = NULL;
	char insert_query_outline[] = "INSERT INTO thread_(file_id) VALUES(\"\")";
	int insert_query_length = 0;
	sqlite3_stmt *res = NULL;
	int i = 0;
	int file_query_length = 0;
	int num_files = 0;
	unsigned int tid = 1337;

	assert(tag != NULL);

	DEBUG(ENTRY);
	DEBUG("Filtering rows in %u that do not contain the tag \"%s\"", tid, tag);

	filter_query = db_query_files_with_tag(tag);
	DEBUG("Filter query: %s", filter_query);

	file_query_length = strlen(filter_query_outline) + strlen(filter_query) + num_digits(tid) + 1;
	file_query = malloc(file_query_length * sizeof(*file_query));
	snprintf(file_query, file_query_length, "SELECT file_id FROM thread_%u WHERE file_id IN (%s)", tid, filter_query);
	DEBUG("File query: %s", file_query);
	num_files = db_array_from_query("file_id", file_query, &file_array);

	db_truncate_table(table);

	for(i = 0; i < num_files; i++) {
		insert_query_length = strlen(insert_query_outline) + num_digits(tid) + strlen(file_array[i]) + 1;
		insert_query = malloc(insert_query_length * sizeof(*insert_query));
		snprintf(insert_query, insert_query_length, "INSERT INTO thread_%u(file_id) VALUES(\"%s\")", tid, file_array[i]);
		DEBUG("Insert query: %s", insert_query);

		(void)sqlite3_prepare_v2(TAGFS_DATA->db_conn, insert_query, strlen(insert_query), &res, NULL);
		sqlite3_step(res);

		free(insert_query);
		sqlite3_finalize(res);
	}

	free(filter_query);
	free(file_query);
	free_char_ptr_array(&file_array, num_files);

	DEBUG(EXIT);
} /* db_filter_table */
