#include "../src/tagfs_db.h"

#include <check.h>
#include <stdlib.h>

START_TEST(test_db_connect) {
	sqlite3 *conn = db_connect("../src/tagfs.sl3");
	bool valid_conn = conn ? true : false;

	db_disconnect(conn);
	
	fail_unless(valid_conn, "Database connection could not be established.");
}
END_TEST

START_TEST(test_db_disconnect) {
	int error = SQLITE_OK;
	sqlite3 *conn = db_connect("../src/tagfs.sl3");

	error = db_disconnect(conn);
	
	fail_unless(error == SQLITE_OK, "Database connection could not be terminated.");
}
END_TEST

START_TEST(test_enable_foreign_keys) {
	sqlite3 *conn = db_connect("../src/tagfs.sl3");

	int result = db_enable_foreign_keys(conn);

	db_disconnect(conn);
	
	fail_unless(result == SQLITE_OK, "Foreign keys could not be enabled.");
}
END_TEST

START_TEST(test_disable_foreign_keys) {
	sqlite3 *conn = db_connect("../src/tagfs.sl3");

	int result = db_disable_foreign_keys(conn);

	db_disconnect(conn);
	
	fail_unless(result == SQLITE_OK, "Foreign keys could not be disabled.");
}
END_TEST

START_TEST(test_db_files_from_query_zero_tags) {
	char **array = NULL;
	int count = db_files_from_query("/", &array);

	fail_unless(count == 1, "Query for files in \"/\" should return one file.");
}
END_TEST

START_TEST(test_db_files_from_query_one_tag) {
	char **array = NULL;
	int count = db_files_from_query("/foo", &array);

	fail_unless(count == 3, "Query for files in \"/foo\" should return three files.");
}
END_TEST

START_TEST(test_db_files_from_query_two_tags) {
	char **array = NULL;
	int count = db_files_from_query("/foo/bar", &array);

	fail_unless(count == 3, "Query for files in \"/foo/bar\" should return three files.");
}
END_TEST

START_TEST(test_db_build_file_query_zero_tags) {
	const char *query = db_build_file_query("/");

	fail_unless(strcmp(query, "SELECT file_name FROM files LEFT JOIN file_has_tag ON files.file_id == file_has_tag.file_id WHERE tag_id IS NULL") == 0, "Query for files in \"/\" should be SELECT file_name FROM files LEFT JOIN file_has_tag ON files.file_id == file_has_tag.file_id WHERE tag_id IS NULL");
}
END_TEST

START_TEST(test_db_build_file_query_one_tag) {
	const char *query = db_build_file_query("/foo");

	fail_unless(strcmp(query, "SELECT file_name \"File Name\" FROM all_tables JOIN file_tag_count USING(file_name) WHERE tag_name == 'foo' GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == 1") == 0, "SELECT file_name \"File Name\" FROM all_tables JOIN file_tag_count USING(file_name) WHERE tag_name == 'foo' GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == 1");
}
END_TEST

START_TEST(test_db_build_file_query_two_tags) {
	const char *query = db_build_file_query("/foo/bar");

	fail_unless(strcmp(query, "SELECT file_name \"File Name\" FROM all_tables JOIN file_tag_count USING(file_name) WHERE tag_name == 'foo' OR tag_name == 'bar' GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == 2") == 0, "SELECT file_name \"File Name\" FROM all_tables JOIN file_tag_count USING(file_name) WHERE tag_name == 'foo' OR tag_name == 'bar' GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == 2");
}
END_TEST

START_TEST(test_db_count_from_query_zero_tags) {
	int count = db_count_from_query("SELECT file_name FROM files LEFT JOIN file_has_tag ON files.file_id == file_has_tag.file_id WHERE tag_id IS NULL");

	fail_unless(count == 1, "Query should return one result.");
}
END_TEST

START_TEST(test_db_count_from_query_one_tag) {
	int count = db_count_from_query("SELECT file_name \"File Name\" FROM all_tables JOIN file_tag_count USING(file_name) WHERE tag_name == 'foo' GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == 1");

	fail_unless(count == 3, "Query should return three results.");
}
END_TEST

START_TEST(test_db_count_from_query_two_tags) {
	int count = db_count_from_query("SELECT file_name \"File Name\" FROM all_tables JOIN file_tag_count USING(file_name) WHERE tag_name == 'foo' OR tag_name == 'bar' GROUP BY file_name HAVING COUNT(file_name) == \"Total Tag Count\" AND \"Total Tag Count\" == 2");

	fail_unless(count == 3, "Query should return three results.");
}
END_TEST

int main() {
	int num_failed = 0;
	SRunner *sr = NULL;

	Suite *db = suite_create("DB");

	TCase *tc_db_connection = tcase_create("db_connection");
	TCase *tc_db_foreign_keys = tcase_create("db_foreign_keys");
	TCase *tc_files_from_query = tcase_create("db_files_from_query");
	TCase *tc_db_build_file_query = tcase_create("db_build_file_query");
	TCase *tc_db_count_from_query = tcase_create("db_count_from_query");

	/* db_connection */
	tcase_add_test(tc_db_connection, test_db_connect);
	tcase_add_test(tc_db_connection, test_db_disconnect);
	suite_add_tcase(db, tc_db_connection);

	/* db_foreign_keys */
	tcase_add_test(tc_db_foreign_keys, test_enable_foreign_keys);
	tcase_add_test(tc_db_foreign_keys, test_disable_foreign_keys);
	suite_add_tcase(db, tc_db_foreign_keys);

	/* db_files_from_query */
	tcase_add_test(tc_files_from_query, test_db_files_from_query_zero_tags);
	tcase_add_test(tc_files_from_query, test_db_files_from_query_one_tag);
	tcase_add_test(tc_files_from_query, test_db_files_from_query_two_tags);
	suite_add_tcase(db, tc_files_from_query);

	/* db_build_file_query */
	tcase_add_test(tc_db_build_file_query, test_db_build_file_query_zero_tags);
	tcase_add_test(tc_db_build_file_query, test_db_build_file_query_one_tag);
	tcase_add_test(tc_db_build_file_query, test_db_build_file_query_two_tags);
	suite_add_tcase(db, tc_db_build_file_query);

	/* db_count_from_query */
	tcase_add_test(tc_db_count_from_query, test_db_count_from_query_zero_tags);
	tcase_add_test(tc_db_count_from_query, test_db_count_from_query_one_tag);
	tcase_add_test(tc_db_count_from_query, test_db_count_from_query_two_tags);
	suite_add_tcase(db, tc_db_count_from_query);

	sr = srunner_create(db);
	srunner_run_all(sr, CK_NORMAL);
	num_failed += srunner_ntests_failed(sr);
	srunner_free(sr);

	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* main */
