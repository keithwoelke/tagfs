#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <sqlite3.h>
#include <stdbool.h>
#include <string.h>

static int db_disconnect(sqlite3 *conn) {
	int error = 0;

	assert(conn != NULL);

	error = sqlite3_close(conn);

	if(error != SQLITE_OK) /* has pending operations or prepared statements to be free'd */ {
		ERROR("A problem occured while disconnecting from the database: %s", sqlite3_errmsg(conn));
	}

	return error;
} /* db_disconnect */

static int db_enable_foreign_keys(sqlite3 *conn) {
	char *error_msg = NULL;
	char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int error = 0;

	assert(conn != NULL);

	error = sqlite3_exec(conn, enable_foreign_keys, NULL, NULL, &error_msg);

	if(error != SQLITE_OK) {
		ERROR("A problem occured while configuring the database: %s", error_msg);
		sqlite3_free(error_msg);
	}

	return error;
} /* db_enable_foreign_keys */

static sqlite3 *db_connect() {
	int error = 0;
	sqlite3 *conn = NULL;

	error = sqlite3_open_v2(TAGFS_DATA->dbpath, &conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(conn != NULL);

	if(error != SQLITE_OK) { /* if no space on drive or file does not exist */
		ERROR("An error occurred when connecting to the database: %s", sqlite3_errmsg(conn));
	}
	
	(void)db_enable_foreign_keys(conn);

	return conn;
} /* db_connect */

static int db_count_from_query(const char *query) {
	char *count_query = NULL;
	char select_count[] = "SELECT COUNT(*) FROM (";
	const char *tail = NULL;
	int count = 0;
	int count_query_length = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	assert(query != NULL);

	DEBUG("Calculating results returned from query: %s", query);

	count_query_length = strlen(query) + strlen(select_count) + 1; /* 2 for closing parenthesis and null terminating character */
	assert(count_query_length > 0);
	DEBUG("Query is %d character(s) in length.", count_query_length);
	count_query = calloc(count_query_length, sizeof(*count_query));
	assert(count_query != NULL);

	(void)snprintf(count_query, count_query_length + 1, "SELECT COUNT(*) FROM (%s)", count_query);

	conn = db_connect();
	assert(conn != NULL);

	(void)sqlite3_prepare_v2(conn, count_query, strlen(count_query), &res, &tail);

	assert(count_query != NULL);
	free(count_query);
	count_query = NULL;

	(void)sqlite3_step(res);
	count = sqlite3_column_int(res, 0);
	assert(count > 0);

	(void)sqlite3_finalize(res);
	(void)db_disconnect(conn);

	DEBUG("%d results returned from query.", count);

	return count;
} /* db_count_from_query */

static int db_array_from_query(char *desired_column_name, const char *result_query, char ***result_array) {
	bool column_match = false;
	const char *tail = NULL;
	const unsigned char *result = NULL;
	int column_count = 0;
	int desired_column_index = 0;
	int i = 0;
	int num_results = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;

	assert(result_query != NULL);
	assert(result_array != NULL);
	assert(*result_array == NULL);

	DEBUG("Generating array from query: %s", result_query);
	DEBUG("Requested column: %s", desired_column_name);

	num_results = db_count_from_query(result_query);

	if(num_results > 0) {
		*result_array = malloc(num_results * sizeof(**result_array));
		assert(*result_array != NULL);

		DEBUG("Allocating array of %d elements", num_results);

		conn = db_connect();
		assert(conn != NULL);

		(void)sqlite3_prepare_v2(conn, result_query, strlen(result_query), &res, &tail);
		column_count = sqlite3_column_count(res);
		assert(column_count > 0);
		DEBUG("Query returns %d column(s)", column_count);

		for(desired_column_index = 0; desired_column_index < column_count; desired_column_index++) { /* find the requested column */
			if(strcmp(desired_column_name, sqlite3_column_name(res, desired_column_index)) == 0) { 
				DEBUG("Located matching column for %s at index %d", desired_column_name, desired_column_index);
				column_match = true;
				break; 
			}
		}

		if(column_match == false) {
			DEBUG("Database column was NOT matched successfully");
		}
		DEBUG("Query results: ");

		for(i = 0; sqlite3_step(res) == SQLITE_ROW; i++) {
			result = sqlite3_column_text(res, desired_column_index); 
			(*result_array)[i] = malloc(result == NULL ? sizeof(NULL) : strlen((const char *)result) * sizeof(*result) + 1);
			assert((*result_array)[i] != NULL);

			if(result != NULL) {
				(void)strcpy((*result_array)[i], (char*)result);
			}
			else {
				(*result_array)[i] = NULL;
			}

			DEBUG("  result_array[%d] = %s, at address %p", i, (*result_array)[i], (*result_array)[i]);
		}

		(void)sqlite3_finalize(res);
		(void)db_disconnect(conn);
	}
	else {
		DEBUG("Null array being returned");
	}

	DEBUG("Query returns %d result(s)", num_results);

	return num_results;
} /* db_array_from_query */

static char* db_query_files_from_tag(const char *tag) {
	char *query = NULL;
	const char select_from[] = "SELECT file_id FROM all_tables where tag_name == \"";
	int query_length = 0;

	assert(tag != NULL);

	query_length = strlen(select_from) + strlen(tag) + 1; /* 1 for closing " */
	assert(query_length > 0);
	DEBUG("Query to select files based on tag is %d characters long.", query_length);
	query = malloc(query_length + 1);
	assert(query != NULL);

	(void)snprintf(query, query_length + 1, "%s%s\"", select_from, tag);

	DEBUG("Query to select files based on tag \"%s\": %s", tag, query);

	return query;
} /* db_query_files_from_tag */

void db_create_table() {
	char *create_query = NULL;
	char query[] = "CREATE TABLE \"thread_\" (\"file_id\" INTEGER PRIMARY KEY NOT NULL)";
	int create_query_length = 0;
	int rc = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	unsigned int tid = pthread_self();

	DEBUG("Creating table thread_%u", tid);

	create_query_length = strlen(query) + num_digits(tid);
	assert(create_query_length > 0);
	DEBUG("Create query length is %d", create_query_length);
	create_query = malloc(create_query_length * sizeof(*create_query) + 1);
	assert(create_query != NULL);
	(void)snprintf(create_query, create_query_length + 1, "CREATE TABLE \"thread_%u\" (\"file_id\" INTEGER PRIMARY KEY NOT NULL)", tid);
	DEBUG("Create query: %s", create_query);

	conn = db_connect();
	assert(conn != NULL);
	(void)sqlite3_prepare_v2(conn, create_query, strlen(create_query), &res, NULL);
	assert(create_query != NULL);
	free(create_query);
	create_query = NULL;

	rc = sqlite3_step(res);

	(void)sqlite3_finalize(res);
	(void)db_disconnect(conn);

	if(rc == SQLITE_DONE) {
		DEBUG("Table \"thread_%u\" created successfully.", tid);
	}
	else {
		ERROR("Table creation FAILED for \"thread_%u\" with result code %d", tid, rc);
	}
} /* db_create_table */

void db_delete_table() {
	char *delete_query = NULL;
	char query[] = "DROP TABLE \"thread_\")";
	int delete_query_length = 0;
	int rc = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	unsigned int tid = pthread_self();

	DEBUG("Deleting table \"thread_%u\".", tid);

	delete_query_length = strlen(query) + num_digits(tid);
	assert(delete_query_length > 0);
	DEBUG("Delete query length is %d.", delete_query_length);
	delete_query = malloc(delete_query_length * sizeof(*delete_query) + 1);
	assert(delete_query != NULL);
	(void)snprintf(delete_query, delete_query_length + 1, "DROP TABLE \"thread_%u\"", tid);
	DEBUG("Delete query: %s", delete_query);

	conn = db_connect();
	assert(conn != NULL);
	(void)sqlite3_prepare_v2(conn, delete_query, strlen(delete_query), &res, NULL);
	assert(delete_query != NULL);
	free(delete_query);
	delete_query = NULL;

	rc = sqlite3_step(res);

	(void)sqlite3_finalize(res);
	db_disconnect(conn);

	if(rc == SQLITE_DONE) {
		DEBUG("Deletion of table \"thread_%u\" was a success.", tid);
	}
	else {
		ERROR("Deletion of table \"thread_%u\" failed with result code %d", tid, rc);
	}
} /* db_delete_table */

static void db_load_table_by_tag(const char *tag) {
DEBUG("foo");
printf("test");
	char *file_query = NULL;
	char *insert_query = NULL;
	const char query[] = "INSERT INTO \"thread_\"(file_name) VALUES()";
	int file_id = 0;
	int i = 0;
	int insert_query_length = 0;
	int rc = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *file_res = NULL;
	sqlite3_stmt *insert_res = NULL;
	unsigned int tid = pthread_self();

	assert(tag != NULL);

	DEBUG("Loading files into table \"thread_%u\".", tid);

	file_query = db_query_files_from_tag(tag);
	assert(file_query != NULL);
	DEBUG("Query to select files with tag \"%s\": %s", tag, file_query);

	conn = db_connect();
	assert(conn != NULL);

	(void)sqlite3_prepare_v2(conn, file_query, strlen(file_query), &file_res, NULL);
	assert(file_query != NULL);
	free(file_query);
	file_query = NULL;

	for(i = 0; sqlite3_step(file_res) == SQLITE_ROW; i++) {
		file_id	= sqlite3_column_int(file_res, 0);
		DEBUG("Retrieving file matching tag \"%s\" with id %d.", tag, file_id);
		insert_query_length = strlen(query) + num_digits(tid) + num_digits(file_id), sizeof(*query);
		insert_query = malloc(sizeof(*insert_query) * insert_query_length + 1);
		assert(insert_query != NULL);

		(void)snprintf(insert_query, insert_query_length + 1, "INSERT INTO \"thread_%u\"(file_id) VALUES(%d)", tid,  file_id);
		DEBUG("Query to load file ID \"%d\" into table \"thread_%u\": %s", file_id, tid, insert_query);
		(void)sqlite3_prepare_v2(conn, insert_query, strlen(insert_query), &insert_res, NULL);

		assert(insert_query != NULL);
		free(insert_query);
		insert_query = NULL;

		rc = sqlite3_step(insert_res);
		sqlite3_finalize(insert_res);

		if(rc == SQLITE_DONE) {
			DEBUG("Insertion of file ID \"%d\" was a success.", file_id);
		}
		else {
			WARN("Insertion of file ID \"%d\" failed with result code %d", file_id, rc);
		}
	}

	sqlite3_finalize(file_res);
	db_disconnect(conn);
} /* db_load_table_by_tag */

static void db_filter_table(const char *tag) {
	char **file_array = NULL;
	char *file_query = NULL;
	char *filter_query = NULL;
	char *insert_query = NULL;
	char filter_query_outline[] = "SELECT file_id FROM thread_ WHERE file_id IN ()";
	char insert_query_outline[] = "INSERT INTO thread_(file_id) VALUES(\"\")";
	int file_query_length = 0;
	int i = 0;
	int insert_query_length = 0;
	int num_files = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	unsigned int tid = pthread_self();

	assert(tag != NULL);

	DEBUG("Filtering rows in \"thread_%u\" that do not contain the tag \"%s\"", tid, tag);

	filter_query = db_query_files_from_tag(tag);
	assert(filter_query != NULL);
	DEBUG("Query to select files with tag \"%s\" from table \"thread_%u\": %s", tag, tid, filter_query);

	file_query_length = strlen(filter_query_outline) + strlen(filter_query) + num_digits(tid);
	assert(file_query_length > 0);
	file_query = malloc(file_query_length * sizeof(*file_query) + 1);
	assert(file_query != NULL);
	(void)snprintf(file_query, file_query_length + 1, "SELECT file_id FROM thread_%u WHERE file_id IN (%s)", tid, filter_query);
	DEBUG("Query to select files from table \"thread_%u\" after those with tag \"%s\" are filtered out: %s", tid, tag, file_query);
	num_files = db_array_from_query("file_id", file_query, &file_array);
	assert(num_files > 0);

	db_truncate_table();

	conn = db_connect();

	for(i = 0; i < num_files; i++) {
		insert_query_length = strlen(insert_query_outline) + num_digits(tid) + strlen(file_array[i]);
		assert(insert_query_length > 0);
		insert_query = malloc(insert_query_length * sizeof(*insert_query) + 1);
		assert(insert_query != NULL);
		(void)snprintf(insert_query, insert_query_length + 1, "INSERT INTO thread_%u(file_id) VALUES(\"%s\")", tid, file_array[i]);
		DEBUG("Query to insert file \"%s\" into table \"thread_%u\" after filtering files with tag \"%s\": %s", file_array[i], tid, tag, insert_query);

		(void)sqlite3_prepare_v2(conn, insert_query, strlen(insert_query), &res, NULL);
		sqlite3_step(res);

		free(insert_query);
		sqlite3_finalize(res);
	}

	assert(filter_query != NULL);
	free(filter_query);
	filter_query = NULL;

	assert(file_query != NULL);
	free(file_query);
	file_query = NULL;

	free_char_ptr_array(&file_array, num_files);
	db_disconnect(conn);
} /* db_filter_table */

void db_load_table_by_path(const char *path) {
	int num_tags = 0;
	char **tag_array = NULL;
	int i = 0;

	num_tags = path_to_array(path, &tag_array);

DEBUG("bar");
	db_load_table_by_tag(tag_array[0]);

	for(i = 1; i < num_tags; i++) {
		db_filter_table(tag_array[i]);
	}

	free_char_ptr_array(&tag_array, num_tags);
} /* db_load_table_by_path */

void db_truncate_table() {
	char *delete_query = NULL;
	char query[] = "DELETE FROM thread_";
	int delete_query_length = 0;
	sqlite3 *conn = NULL;
	sqlite3_stmt *res = NULL;
	unsigned int tid = pthread_self();

	delete_query_length = strlen(query) + num_digits(tid);
	assert(delete_query_length > 0);

	delete_query = malloc(delete_query_length * sizeof(*delete_query) + 1);
	snprintf(delete_query, delete_query_length + 1, "DELETE FROM thread_%u", tid);

	conn = db_connect();
	assert(conn != NULL);

	(void)sqlite3_prepare_v2(conn, delete_query, strlen(delete_query), &res, NULL);
	sqlite3_step(res);

	sqlite3_finalize(res);

	assert(delete_query != NULL);
	free(delete_query);
	delete_query = NULL;

	db_disconnect(conn);
} /* truncate_table */










//static const char* db_build_file_query(const char *path) {
//	char **tag_array = NULL;
//	char *file_query = NULL;
//	char select_from_where[] = "SELECT * FROM all_tables JOIN file_tag_count USING(file_name) WHERE ";
//	char zero_tag_query[] = "SELECT files.file_id, tag_id, file_location, file_name FROM files LEFT JOIN file_has_tag ON files.file_id == file_has_tag.file_id";
//	int file_query_length = 0;
//	int i = 0;
//	int num_tags = 0;
//
//	DEBUG(D_FUNCTION_DB_BUILD_FILE_QUERY, D_LEVEL_ENTRY, "db_build_file_query");
//
//	assert(path != NULL);
//
//	DEBUG(D_FUNCTION_DB_BUILD_FILE_QUERY, D_LEVEL_DEBUG, "Building query to select files in %s", path);
//
//	if(strcmp(path, "/") == 0) {
//		DEBUG(D_FUNCTION_DB_BUILD_FILE_QUERY, D_LEVEL_DEBUG, "Using default query", path);
//		file_query = malloc(strlen(zero_tag_query) + 1);
//		assert(file_query != NULL);
//
//		strcpy(file_query, zero_tag_query);
//	}
//	else {
//		num_tags = path_to_array(path, &tag_array);
//		assert(tag_array != NULL);
//
//		DEBUG(D_FUNCTION_DB_BUILD_FILE_QUERY, D_LEVEL_DEBUG, "Number of tags in path: %d", num_tags);
//
//		file_query_length = strlen(select_from_where) + (num_tags * (strlen("tag_name == '") + strlen("' "))) + ((num_tags - 1) * strlen("OR ")) + strlen(path) - num_tags + 1;
//		assert(file_query_length > 0);
//		file_query = calloc(file_query_length, sizeof(*file_query));
//		assert(file_query != NULL);
//
//		strcat(file_query, select_from_where);
//
//		for(; i < num_tags; i++) {
//			strcat(strcat(strcat(file_query, "tag_name == '"), tag_array[i]), "'");
//			if(num_tags - 1 != i) { strcat(file_query, " OR "); }
//		}
//
//		free_char_ptr_array(&tag_array, num_tags);
//	}
//
//	DEBUG(D_FUNCTION_DB_BUILD_FILE_QUERY, D_LEVEL_DEBUG, "File query: %s", file_query);
//	DEBUG(D_FUNCTION_DB_BUILD_FILE_QUERY, D_LEVEL_EXIT, "db_build_file_query");
//	return file_query;
//} /* db_build_file_query */
//
//static const char* db_build_restricted_file_query(const char *path) {
//	char *num_tags_str = NULL;
//	char *restricted_file_query = NULL;
//	char group_by_having[] = " GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == ";
//	char where[] = " WHERE tag_id IS NULL";
//	const char *file_query = NULL;
//	int num_tags = 0;
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_ENTRY, "db_build_restricted_file_query");
//
//	assert(path != NULL);
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_DEBUG, "Building restricted file query from path: %s", path);
//
//	file_query = db_build_file_query(path);
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_DEBUG, "Base file query: %s", file_query);
//
//	if(strcmp(path, "/") == 0) {
//		restricted_file_query = calloc(strlen(file_query) + strlen(where) + 1, sizeof(*restricted_file_query));
//		assert(restricted_file_query != NULL);
//		strcat(strcat(restricted_file_query, file_query), where);
//		DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_DEBUG, "Using default restriction (no tags)");
//	}
//	else {
//		num_tags = num_tags_in_path(path);
//		DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_DEBUG, "Number of tags in path: %d", num_tags);
//		num_tags_str = malloc(num_digits(num_tags) * sizeof(*num_tags_str) + 1);
//		(void)snprintf(num_tags_str, num_digits(num_tags) + 1, "%d", num_tags);
//	
//		restricted_file_query = calloc(strlen(file_query) + strlen(group_by_having) + strlen(num_tags_str) + 1, sizeof(*restricted_file_query));
//		assert(restricted_file_query != NULL);
//		strcat(strcat(strcat(restricted_file_query, file_query), group_by_having), num_tags_str);
//
//		assert(num_tags_str != NULL);
//		free(num_tags_str);
//		num_tags_str = NULL;
//	}
//
//	assert(file_query != NULL);
//	free((void *)file_query);
//	file_query = NULL;
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_DEBUG, "Restricted file query: %s", restricted_file_query);
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY, D_LEVEL_EXIT, "db_build_restricted_file_query");
//	return restricted_file_query;
//} /* db_build_restricted_file_query */
//
//static const char* db_build_tag_query(const char *path) {
//	char *tag_query = NULL;
//	char select_tag_name[] = "SELECT DISTINCT tag_name FROM all_tables WHERE file_name IN (SELECT file_name FROM (";
//	const char *file_query = NULL;
//
//	DEBUG(D_FUNCTION_DB_BUILD_TAG_QUERY, D_LEVEL_ENTRY, "db_build_tag_query");
//
//	assert(path != NULL);
//
//	DEBUG(D_FUNCTION_DB_BUILD_TAG_QUERY, D_LEVEL_DEBUG, "Building a tag query for path: %s", path);
//
//	file_query = db_build_file_query(path);
//	assert(file_query != NULL);
//
//	DEBUG(D_FUNCTION_DB_BUILD_TAG_QUERY, D_LEVEL_DEBUG, "File query: %s", file_query);
//
//	tag_query = calloc(strlen(select_tag_name) + strlen(file_query) + 3, sizeof(*tag_query)); /* 3 is for closing parentheses and null terminating character */
//	assert(tag_query != NULL);
//
//	strcat(strcat(strcat(tag_query, select_tag_name), file_query), "))");
//
//	assert(file_query != NULL);
//	free((void *)file_query);
//	file_query = NULL;
//
//	DEBUG(D_FUNCTION_DB_BUILD_TAG_QUERY, D_LEVEL_DEBUG, "Tag query: %s", tag_query);
//	DEBUG(D_FUNCTION_DB_BUILD_TAG_QUERY, D_LEVEL_EXIT, "db_build_tag_query");
//	return tag_query;
//} /* db_build_tag_query */
//
//static const char* db_build_restricted_tag_query(const char *path) {
//	char **tag_array = NULL;
//	char *restricted_tag_query = NULL;
//	char and_not[] = " AND tag_name != '";
//	const char *tag_query = NULL;
//	int i = 0;
//	int num_tags = 0;
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY, D_LEVEL_ENTRY, "db_build_restricted_tag_query");
//
//	assert(path != NULL);
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY, D_LEVEL_DEBUG, "Building restricted tag query for path: %s", path);
//
//	num_tags = path_to_array(path, &tag_array);
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY, D_LEVEL_DEBUG, "Number of tags in path: %d", num_tags);
//
//	tag_query = db_build_tag_query(path);
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY, D_LEVEL_DEBUG, "Tag query: %s", tag_query);
//	restricted_tag_query = calloc(strlen(tag_query) + (num_tags * (strlen(and_not) + 1) + strlen(path) - num_tags + 1), sizeof(*restricted_tag_query));
//	assert(restricted_tag_query != NULL);
//	strcat(restricted_tag_query, tag_query);
//
//	assert(tag_query != NULL);
//	free((void *)tag_query);
//	tag_query = NULL;
//
//	for(; i < num_tags; i++) {
//		strcat(strcat(strcat(restricted_tag_query, and_not), tag_array[i]), "'");
//	}
//
//	free_char_ptr_array(&tag_array, num_tags);
//
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY, D_LEVEL_DEBUG, "Restricted tag query: %s", restricted_tag_query);
//	DEBUG(D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY, D_LEVEL_EXIT, "db_build_restricted_tag_query");
//	return restricted_tag_query;
//} /* db_build_restricted_tag_query */
//
//int db_files_from_restricted_query(const char *path, /*@out@*/ char ***file_array) {
//	const char *file_query = NULL;
//	int num_files = 0;
//
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_ENTRY, "db_files_from_restricted_query");
//
//	assert(path != NULL);
//	assert(file_array != NULL);
//	assert(*file_array == NULL);
//	
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_DEBUG, "Creating array containing files at path: %s", path);
//
//	file_query = db_build_restricted_file_query(path);
//	assert(file_query != NULL);
//
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_DEBUG, "File query: %s", file_query);
//
//	num_files = db_array_from_query("file_name", file_query, file_array);
//
//	assert(file_query != NULL);
//	free((void *)file_query);
//	file_query = NULL;
//
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_DEBUG, "Number of files returned from query: %d", num_files);
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_EXIT, "db_files_from_restricted_query");
//	return num_files;
//} /* db_files_from_restricted_query */
//
//int db_files_from_query(const char *path, /*@out@*/ char ***file_array) {
//	const char *file_query = NULL;
//	int num_files = 0;
//
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_ENTRY, "db_files_from_query");
//
//	assert(path != NULL);
//	assert(file_array != NULL);
//	assert(*file_array == NULL);
//	
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_DEBUG, "Creating array containing files at path: %s", path);
//
//	file_query = db_build_file_query(path);
//	assert(file_query != NULL);
//
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_DEBUG, "File query: %s", file_query);
//
//	num_files = db_array_from_query("file_name", file_query, file_array);
//
//	assert(file_query != NULL);
//	free((void *)file_query);
//	file_query = NULL;
//
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_DEBUG, "Number of files returned from query: %d", num_files);
//	DEBUG(D_FUNCTION_DB_FILES_FROM_QUERY, D_LEVEL_EXIT, "db_files_from_query");
//	return num_files;
//} /* db_files_from_query */
//
//int db_tags_from_query(const char *path, /*@out@*/ char ***tag_array) {
//	const char *tag_query = NULL;
//	int num_tags = 0;
//
//	DEBUG(D_FUNCTION_DB_TAGS_FROM_QUERY, D_LEVEL_ENTRY, "db_tags_from_query");
//
//	assert(path != NULL);
//	assert(tag_array != NULL);
//	assert(*tag_array == NULL);
//
//	DEBUG(D_FUNCTION_DB_TAGS_FROM_QUERY, D_LEVEL_DEBUG, "Building restricted tag query for path: %s", path);
//
//	tag_query = db_build_restricted_tag_query(path);
//	assert(tag_query != NULL);
//
//	DEBUG(D_FUNCTION_DB_TAGS_FROM_QUERY, D_LEVEL_DEBUG, "Restricted tag query: %s", tag_query);
//
//	num_tags = db_array_from_query("tag_name", tag_query, tag_array);
//
//	assert(tag_query != NULL);
//	free((void *)tag_query);
//	tag_query = NULL;
//
//	DEBUG(D_FUNCTION_DB_TAGS_FROM_QUERY, D_LEVEL_DEBUG, "Number of tags in path: %d", num_tags);
//	DEBUG(D_FUNCTION_DB_TAGS_FROM_QUERY, D_LEVEL_EXIT, "db_tags_from_query");
//	return num_tags;
//} /* db_tags_from_query */
//
//static int db_get_file_id(const char *file_path) {
//	char **file_id_array = NULL;
//	char **file_name_array = NULL;
//	char **tag_array = NULL;
//	char *file = NULL;
//	const char *dir_path = NULL;
//	const char *file_query = NULL;
//	int file_id = 0;
//	int file_id_array_count = 0;
//	int file_name_array_count = 0;
//	int i = 0;
//	int num_tokens = 0;
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_ENTRY, "db_get_file_id");
//
//	assert(file_path != NULL);
//
//	dir_path = get_file_directory(file_path);
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "File to locate is in the directory: %s", dir_path);
//	file_query = db_build_restricted_file_query(dir_path);
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "File query: %s", file_query);
//
//	num_tokens = path_to_array(file_path, &tag_array);
//	file = tag_array[num_tokens - 1];
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "File name is: %s", file);
//
//	file_name_array_count = db_array_from_query("file_name", file_query, &file_name_array);
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "Files returned from query: %d", file_name_array_count);
//
//	file_id_array_count = db_array_from_query("file_id", file_query, &file_id_array);
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "File IDs returned from query: %d", file_id_array_count);
//
//	if(file_name_array_count != file_id_array_count) {
//		DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_WARNING, "Number of file names do not match number of file IDs returned from same query. Aborting!");
//		return 0;
//	}
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_FOLDER_CONTENTS, "Query results:");
//	debug_indent();
//	for(i = 0; i < file_id_array_count; i++) {
//		DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_FOLDER_CONTENTS, "File ID: %s; File name: %s", file_id_array[i], file_name_array[i]);
//	}
//	debug_deindent();
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_FOLDER_CONTENTS, "Checking query results:");
//	debug_indent();
//	for(i = 0; i < file_id_array_count; i++) {
//		if(strcmp(file, file_name_array[i]) == 0) { 
//			DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "File ID: %s; File name: %s == %s", file_id_array[i], file_name_array[i], file);
//			break;
//		}
//		else {
//			DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_FOLDER_CONTENTS, "File ID: %s; File name: %s != %s", file_id_array[i], file_name_array[i], file);
//		}
//	}
//	debug_deindent();
//
//	assert(strcmp(file_name_array[i], file) == 0);
//
//	free_char_ptr_array(&tag_array, num_tokens);
//	free_char_ptr_array(&file_name_array, file_name_array_count);
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_DEBUG, "File located at file_id_array: %s", file_id_array[i]);
//
//	file_id = atoi(file_id_array[i]);
//	
//	free_char_ptr_array(&file_id_array, file_id_array_count);
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_ID, D_LEVEL_EXIT, "db_get_file_id");
//
//	return file_id;
//} /* db_get_file_id */
//
//void db_delete_file(const char *path) {
//	char *delete_query = NULL;
//	char *file_id_str = NULL;
//	char delete_from[] = "DELETE FROM files WHERE file_id = ";
//	const char *tail = NULL;
//	int delete_query_length = 0;
//	int file_id = 0;
//	int rc = 0;
//	sqlite3 *conn = NULL;
//	sqlite3_stmt *res = NULL;
//
//	DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_ENTRY, "db_delete_file");
//
//	assert(path != NULL);
//
//	DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_DEBUG, "Deleting file: %s", path);
//
//	file_id = db_get_file_id(path);
//	DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_DEBUG, "File ID to delete: %d", file_id);
//
//	conn = db_connect();
//	assert(conn != NULL);
//
//	delete_query_length = strlen(delete_from) + num_digits(file_id) + 1;
//	delete_query = calloc(delete_query_length, sizeof(*delete_query));
//	assert(delete_query != NULL);
//
//	file_id_str = malloc(num_digits(file_id) * sizeof(*file_id_str) + 1);
//	(void)snprintf(file_id_str, num_digits(file_id) + 1, "%d", file_id);
//	strcat(strcat(delete_query, delete_from), file_id_str);
//	free(file_id_str);
//
//	DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_DEBUG, "Delete query: %s", delete_query);
//
//	(void)sqlite3_prepare_v2(conn, delete_query, strlen(delete_query), &res, &tail);
//	free(delete_query);
//
//	rc = sqlite3_step(res);
//	db_disconnect(conn);
//
//	if(rc == SQLITE_MISUSE) {
//		DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_WARNING, "SQLite library used incorrectly");
//	}
//	else if(rc == SQLITE_CONSTRAINT) {
//		DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_WARNING, "SQLite constraint violation");
//	}
//
//	DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_DEBUG, "%s deleted successfully", path);
//	DEBUG(D_FUNCTION_DB_DELETE_FILE, D_LEVEL_EXIT, "db_delete_file");
//} /* db_delete_file */
//
//const char* get_file_location(const char *path) {
//	char *file_id_str = NULL;
//	char *file_location = NULL;
//	char *file_name = NULL;
//	char *file_path = NULL;
//	char *query = NULL;
//	char select_from[] = "SELECT file_location, file_name FROM files WHERE file_id = ";
//	int file_id = 0;
//	int num_digits_id = 0;
//	sqlite3 *conn = NULL;
//	sqlite3_stmt *res = NULL;
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_ENTRY, "db_get_file_location");
//
//	assert(path != NULL);
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_DEBUG, "Retrieving real file location for %s", path);
//
//	file_id = db_get_file_id(path);
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_DEBUG, "File ID: %d", file_id);
//
//	query = calloc(strlen(select_from) + num_digits(file_id) + 1, sizeof(*query));
//	assert(query != NULL);
//	num_digits_id = num_digits(file_id);
//	file_id_str = malloc((num_digits_id + 1) * sizeof(*file_id_str));
//	assert(file_id_str != NULL);
//	snprintf(file_id_str, num_digits_id + 1, "%d", file_id);
//	strcat(strcat(query, select_from), file_id_str);
//	free(file_id_str);
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_DEBUG, "File location query: %s", query);
//
//	conn = db_connect();
//
//	(void)sqlite3_prepare_v2(conn, query, strlen(query), &res, NULL);
//	free(query);
//	(void)sqlite3_step(res);
//
//	file_location = (char*)sqlite3_column_text(res, 0);
//	file_name = (char*)sqlite3_column_text(res, 1);
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_DEBUG, "File location: %s", file_location);
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_DEBUG, "File name: %s", file_name);
//
//	file_path = calloc(strlen(file_location) + strlen(file_name) + 1, sizeof(*file_path));
//	strcat(file_path, file_location);
//	if(strcmp(file_path, "/") != 0) { strcat(file_path, "/"); }
//	strcat(file_path, file_name);
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_DEBUG, "File location: %s", file_path);
//
//	DEBUG(D_FUNCTION_DB_GET_FILE_LOCATION, D_LEVEL_EXIT, "db_get_file_location");
//
//	return file_path;
//} /* get_file_location */
