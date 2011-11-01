#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <glib.h>
#include <math.h>
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

	DEBUG("Query compilation was %ssuccessful", rc == SQLITE_OK ? "" : "un");
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
	if(rc != SQLITE_ROW) {
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
} /* db_step_statement */

/**
 * Insert the results of the specified query into the specified table. Only the results of the first column are entered into the table, and the values are assumed to be integers.
 *
 * @param conn A sqlite database handle.
 * @param query An SQL statement, UTF-8 encoded.
 * @return The result of the operation, corresponding to the return codes of the sqlite3_finalize function call.
 */
static int db_insert_query_results_into_hashtable(sqlite3 *conn, char *query, GHashTable *table) {
	int rc = SQLITE_ERROR; /* return code of sqlite3 operation */
	sqlite3_stmt *res = NULL;
	unsigned long int_from_table = 0;

	DEBUG(ENTRY);

	assert(query != NULL);
	assert(conn != NULL);
	assert(table != NULL);

	DEBUG("Inserting into table results from query: %s", query);

	rc = db_execute_statement(conn, query, &res);

	/* insert results into hashset */
	do {
		int_from_table = sqlite3_column_int(res, 0);
		g_hash_table_insert(table, (gpointer)int_from_table, (gpointer)int_from_table);

		rc = db_step_statement(conn, query, res);
	} while(rc == SQLITE_ROW);

	rc = db_finalize_statement(conn, query, res);

	DEBUG("Results entered into table from query: %s", query);
	DEBUG(EXIT);
	return rc;
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
	rc = sqlite3_open_v2(TAGFS_DATA->db_path, &conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
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







char *db_tag_name_from_tag_id(int tag_id) {
	char *query = NULL;
	const unsigned char *tag_name = NULL;
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
	query_outline_length = strlen(query_outline) + num_digits_in_id;
	query_length = query_outline_length + num_digits_in_id;
	query = malloc(query_outline_length * sizeof(*query) + 1);
	written = snprintf(query, query_outline_length + 1, "%s%d", query_outline, tag_id);
	assert(written == query_outline_length + num_digits_in_id);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	db_execute_statement(conn, query, &res);

	/* get name corresponding to tag_id */
	tag_name = sqlite3_column_text(res, 0); 

	db_disconnect(conn);

	DEBUG("Tag ID %d corresponds to tag %s", tag_id, tag_name);
	DEBUG(EXIT);
	return tag_name;
} /* db_tag_name_from_tag_id */

























int db_files_from_tag_id(int tag_id, int **file_array) {
	DEBUG(ENTRY);

	if(tag_id == 2) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 2;

		DEBUG(EXIT);
		return 1;
	} else if(tag_id == 4) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 3;

		DEBUG(EXIT);
		return 1;
	} else if(tag_id == 3) {
		(*file_array) = malloc(2 * sizeof(**file_array));
		(*file_array)[0] = 1;
		(*file_array)[1] = 2;

		DEBUG(EXIT);
		return 2;
	} else if(tag_id == 1) {
		(*file_array) = malloc(2 * sizeof(**file_array));
		(*file_array)[0] = 1;
		(*file_array)[1] = 3;

		DEBUG(EXIT);
		return 2;
	} else if(tag_id == 0) {
		(*file_array) = malloc(1 * sizeof(**file_array));
		(*file_array)[0] = 4;

		DEBUG(EXIT);
		return 1;
	}

	WARN("Tag ID is %d.", tag_id);
	DEBUG(EXIT);
	return 0;
} /* db_files_from_tag_id */

int db_tag_id_from_tag_name(char *tag) {
	DEBUG(ENTRY);

	if(strcmp(tag, "Video") == 0) {
		DEBUG(EXIT);
		return 1;
	} else if(strcmp(tag, "Audio") == 0) {
		DEBUG(EXIT);
		return 2;
	} else if(strcmp(tag, "ogg") == 0) {
		DEBUG(EXIT);
		return 3;
	} else if(strcmp(tag, "mov") == 0) {
		DEBUG(EXIT);
		return 4;
	} else if(strcmp(tag, "/") == 0) {
		DEBUG(EXIT);
		return 0;
	}

	DEBUG(EXIT);
	return -1;
} /* db_tag_id_from_tag_name */

int db_get_all_tags(int **folders) {
	int *array = NULL;

	DEBUG(ENTRY);

	array = malloc(4 * sizeof(*array));
	array[0] = 1;
	array[1] = 2;
	array[2] = 3;
	array[3] = 4;

	*folders = array;
	DEBUG(EXIT);
	return 4;
}

//char *db_tag_name_from_tag_id(int tag_id) {
//	char * foo = NULL;
//
//	DEBUG(ENTRY);
//
//	assert(tag_id > 0);
//
//	if(tag_id == 1) {
//		foo = malloc(6 * sizeof(*foo));
//		snprintf(foo, 6, "Video");
//	} else if(tag_id == 2) {
//		foo = malloc(6 * sizeof(*foo));
//		snprintf(foo, 6, "Audio");
//	} else if(tag_id == 3) {
//		foo = malloc(4 * sizeof(*foo));
//		snprintf(foo, 4, "ogg");
//	} else if(tag_id == 4) {
//		foo = malloc(4 * sizeof(*foo));
//		snprintf(foo, 4, "mov");
//	}
//
//	DEBUG(EXIT);
//	return foo;
//} /* db_tag_name_from_tag_id */






int db_count_from_query(char *query) {
	DEBUG(ENTRY);
	bool warn = false; /* whether or not a user visible warning should print */
	char *count_query = NULL;
	char select_count_outline[] = "SELECT COUNT(*) FROM ()";
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

int db_array_from_query(char *desired_column_name, char *result_query, char ***result_array) {
	DEBUG(ENTRY);
	bool column_match = false;
	char *tail = NULL;
	unsigned char *result = NULL;
	int column_count = 0;
	int desired_column_index = 0;
	int i = 0;
	int num_results = 0;
	sqlite3_stmt *res = NULL;
	int foo = 0;


	assert(result_query != NULL);
	assert(result_array != NULL);
	assert(*result_array == NULL);


	num_results = db_count_from_query(result_query);

	if(num_results > 0) {
		*result_array = malloc(num_results * sizeof(**result_array));
		assert(*result_array != NULL);


		foo = sqlite3_prepare_v2(TAGFS_DATA->db_conn, result_query, strlen(result_query), &res, &tail);
		column_count = sqlite3_column_count(res);

		for(desired_column_index = 0; desired_column_index < column_count; desired_column_index++) { /* find the requested column */
			if(strcmp(desired_column_name, sqlite3_column_name(res, desired_column_index)) == 0) { 
				DEBUG("COLUMN FOUND");
				column_match = true;
				break; 
			}
		}

		if(column_match == false) {
			DEBUG("COLUMN NOT FOUND");
		}
		for(i = 0; sqlite3_step(res) == SQLITE_ROW; i++) {
			result = sqlite3_column_text(res, desired_column_index); 
			(*result_array)[i] = malloc(result == NULL ? sizeof(NULL) : strlen((char *)result) * sizeof(*result) + 1);
			assert((*result_array)[i] != NULL);
			if(result != NULL) {
				strcpy((*result_array)[i], (char*)result);
				DEBUG("BAR: %s", (char*)result);
			}
			else {
				(*result_array)[i] = NULL;
			}

		}

		(void)sqlite3_finalize(res);
	}
	else {
	}

	DEBUG(EXIT);
	return num_results;
} /* db_array_from_query */