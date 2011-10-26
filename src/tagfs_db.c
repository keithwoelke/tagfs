#include "tagfs_common.h"
#include "tagfs_db.h"
#include "tagfs_debug.h"

#include <assert.h>
#include <glib.h>
#include <math.h>
#include <sqlite3.h>
#include <string.h>

/**
 * Disconnect from the database.
 *
 * @param conn The sqlite3 database connection to close.
 */
static void db_disconnect(sqlite3 *conn) {
	DEBUG(ENTRY);
	int rc = 0; /* return code of sqlite3 operations */

	DEBUG("Disconnecting from the database");

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
 *
 * @param conn The sqlite3 database connection to enable foreign keys on.
 **/
static void db_enable_foreign_keys(sqlite3 *conn) {
	/* DEBUG(ENTRY); */
	char *err_msg = NULL; /* sqlite3 error message */
	const char enable_foreign_keys[] = "PRAGMA foreign_keys = ON";
	int rc = 0; /* return code of sqlite3 operation */

	/* DEBUG("Enabling foreign keys"); */

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
 *
 * @return The database connection handle.
 */
static sqlite3 *db_connect() {
	DEBUG(ENTRY);
	int rc = 0; /* return code of sqlite3 operation */
	sqlite3 *conn = NULL;

	DEBUG("Connecting to database: %s", TAGFS_DATA->db_path);

	/* connect to the database */
	assert(TAGFS_DATA->db_path != NULL);
	rc = sqlite3_open_v2(TAGFS_DATA->db_path, &conn, SQLITE_OPEN_READWRITE, NULL); /* TODO: set as 'SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE' and create the database if it does not exist already */
	assert(conn != NULL);

	/* handle result code */
	if(rc != SQLITE_OK) { /* if no space on drive or file does not exist */
		DEBUG("ERROR: %s", sqlite3_errmsg(conn));
		ERROR("An error occurred when connecting to the database");
	}
	else { DEBUG("Database connection successful"); }

	/* enable foreign keys */
	db_enable_foreign_keys(conn);

	DEBUG(EXIT);
	return conn;
} /* db_connect */

char *db_get_file_location(int file_id) {
	DEBUG(ENTRY);
	char *query = NULL;
	char *file_location = NULL;
	char *tmp_file_directory = NULL; /* holds text from the query so it can be copied to a new memory location */
	char *tmp_file_name = NULL; /* hold name of file until it can be copied to a new memory location */
	const char query_outline[] = "SELECT file_location, file_name FROM files WHERE file_id = ";
	int file_location_length = 0; /* length of the file location to return */
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
	written = snprintf(query, query_length + 1, "SELECT file_location, file_name FROM files WHERE file_id = %d", file_id);
	assert(written == query_length);

	/* connect to database */
	conn = db_connect();
	assert(conn != NULL);

	/* compile prepared statement */	
	assert(conn != NULL);
	rc = sqlite3_prepare_v2(conn, query, query_length, &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", query, query_length, rc, sqlite3_errmsg(conn));
		WARN("An error occured when communicating with the database");
	}

	/* execute statement */
	rc = sqlite3_step(res);

	if(rc != SQLITE_ROW) {
		DEBUG("WARNING: Executing statement \"%s\" of length %d FAILED with result code %d: %s", query, query_length, rc, sqlite3_errmsg(conn));
		WARN("An error occured when communicating with the database");
	}

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

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", query, query_length, rc, sqlite3_errmsg(conn));
		WARN("An error occured when communicating with the database");
	}

	db_disconnect(conn);

	free_single_ptr((void *)&query);

	DEBUG("File id %d corresponds to %s", file_id, file_location);
	DEBUG(EXIT);
	return file_location;
} /* db_get_file_location */







int db_files_from_tag_id(int tag_id, int **file_array) {}
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
} /* db_files_from_tag_id */








static int db_insert_query_results_into_hashtable(const char *query, sqlite3 *conn, sqlite3_stmt *res, GHashTable *table) {
	int rc = 0; /* return code of sqlite3 operation */
	unsigned long int_from_table = 0;

	DEBUG("Preparing to execute query: %s", query);
	rc = sqlite3_prepare_v2(conn, query, (int)strlen(query), &res, NULL);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Compiling statement \"%s\" of length %d FAILED with result code %d: %s", query, (int)strlen(query), rc, sqlite3_errmsg(conn));
		WARN("An error occured when communicating with the database");
	}

	rc = sqlite3_step(res);

	/* insert results into hashset */
	do {
		if(rc != SQLITE_ROW) {
			DEBUG("WARNING: Executing statement \"%s\" of length %d FAILED with result code %d: %s", query, (int)strlen(query), rc, sqlite3_errmsg(conn));
			WARN("An error occured when communicating with the database");
		}

		int_from_table = sqlite3_column_int(res, 0);
		g_hash_table_insert(table, (gpointer)int_from_table, (gpointer)int_from_table);

		rc = sqlite3_step(res);
	}
	while(rc == SQLITE_ROW);

	rc = sqlite3_finalize(res);

	if(rc != SQLITE_OK) {
		DEBUG("WARNING: Finalizing statement \"%s\" of length %d FAILED with result code %d: %s", query, (int)strlen(query), rc, sqlite3_errmsg(conn));
		WARN("An error occured when communicating with the database");
	}

	return rc;
} /* db_insert_query_results_into_hashtable */

int db_tags_from_files(const int *files, int num_files, int **tags) {
	GHashTable *table = NULL;
	GHashTableIter iter;
	char *file_id_str = NULL;
	char *query = NULL;
	const char or_outline[] = " OR file_id = ";
	const char query_outline[] = "SELECT DISTINCT tag_id FROM all_tables WHERE file_id = ";
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
	sqlite3_stmt *res = NULL;

	DEBUG(ENTRY);

	assert(num_files > 0);
	assert(files != NULL);

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

			db_insert_query_results_into_hashtable(query, conn, res, table);

			free_single_ptr((void *)&query);

			continue;
		}

		/* append file ID to query */
		file_id_str = calloc(num_digits_in_id + 1, sizeof(*file_id_str));
		snprintf(file_id_str, num_digits_in_id + 1, "%d", file_id);
		strncat(strncat(query, or_outline, strlen(or_outline)), file_id_str, strlen(file_id_str));
		free_single_ptr((void *)&file_id_str);
	}

	db_insert_query_results_into_hashtable(query, conn, res, table);

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
	char * foo = NULL;

	if(tag_id == 1) {
		foo = malloc(6 * sizeof(*foo));
		snprintf(foo, 6, "Video");
	} else if(tag_id == 2) {
		foo = malloc(6 * sizeof(*foo));
		snprintf(foo, 6, "Audio");
	} else if(tag_id == 3) {
		foo = malloc(4 * sizeof(*foo));
		snprintf(foo, 4, "ogg");
	} else if(tag_id == 4) {
		foo = malloc(4 * sizeof(*foo));
		snprintf(foo, 4, "mov");
	}

	return foo;
} /* db_tag_name_from_tag_id */

int db_tag_id_from_tag_name(const char *tag) {
	if(strcmp(tag, "Video") == 0)
		return 1;
	} else if(strcmp(tag, "Audio") == 0) {
		return 2;
	} else if(strcmp(tag, "ogg") == 0) {
		return 3;
	} else if(strcmp(tag, "mov") == 0) {
		return 4;
	} else if(strcmp(tag, "/") == 0) {
		return 0;
	}
} /* db_tag_id_from_tag_name */












//
//
//
//
//
//
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
