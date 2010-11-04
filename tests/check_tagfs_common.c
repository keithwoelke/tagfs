#include "../src/tagfs_common.h"

#include <check.h>
#include <stdlib.h>

START_TEST(test_free_char_ptr_array_null) {
	char **array = NULL;
	free_char_ptr_array(&array, 0);

	fail_unless(array == NULL, "Array should be NULL after freeing.");
}
END_TEST

START_TEST(test_free_char_ptr_array_zero_elements) {
	char **array = malloc(0);
	free_char_ptr_array(&array, 0);

	fail_unless(array == NULL, "Array should be NULL after freeing.");
}
END_TEST

START_TEST(test_free_char_ptr_array_one_element) {
	char **array = malloc(1 * sizeof(*array));
	array[0] = malloc(4 * sizeof(*array[0]));
	strcpy(array[0], "foo");
	free_char_ptr_array(&array, 1);

	fail_unless(array == NULL, "Array should be NULL after freeing.");
}
END_TEST

START_TEST(test_free_char_ptr_array_two_elements) {
	char **array = malloc(2 * sizeof(*array));
	array[0] = malloc(4 * sizeof(*array[0]));
	array[1] = malloc(4 * sizeof(*array[1]));
	strcpy(array[0], "foo");
	strcpy(array[1], "bar");
	free_char_ptr_array(&array, 1);

	fail_unless(array == NULL, "Array should be NULL after freeing.");
}
END_TEST

START_TEST(test_path_to_array_zero_tags) {
	char path[2] = "/";
	char **array = NULL;
	int count = path_to_array(path, &array);

	fail_unless(array == NULL && count == 0, "array should be NULL and count should be 0.");
}
END_TEST

START_TEST(test_path_to_array_one_tag) {
	char path[5] = "/foo";
	char **array = NULL;
	int count = path_to_array(path, &array);

	fail_unless(strcmp(array[0], "foo") == 0 && count == 1, "array[0] should be equal to \"foo\" and count should be 1.");
}
END_TEST

START_TEST(test_path_to_array_two_tags) {
	char path[9] = "/foo/bar";
	char **array = NULL;
	int count = path_to_array(path, &array);

	fail_unless(strcmp(array[0], "foo") == 0 && strcmp(array[1], "bar") == 0 && count == 2, "array[0] should be equal to \"foo,\" array[1] should be equal to \"bar,\" and count should be 2.");
}
END_TEST

START_TEST(test_num_tags_in_path_zero_tags) {
	int num_tags = -1;

	num_tags = num_tags_in_path("/");
	
	fail_unless(num_tags == 0, "Path should return zero tags for \"/\"");
}
END_TEST

START_TEST(test_num_tags_in_path_one_tag) {
	int num_tags = -1;

	num_tags = num_tags_in_path("/foo");
	
	fail_unless(num_tags == 1, "Path should return one tag for \"/foo\"");
}
END_TEST

START_TEST(test_num_tags_in_path_two_tags) {
	int num_tags = -1;

	num_tags = num_tags_in_path("/foo/bar");
	
	fail_unless(num_tags == 2, "Path should return two tags for \"/foo/bar\"");
}
END_TEST

START_TEST(test_array_contains_string_match) {
	char *array[2] = {"foo", "bar"};

	fail_unless(array_contains_string(array, "foo", 2), "\"foo\" should be contained in the array.");
}
END_TEST

START_TEST(test_array_contains_string_no_match) {
	char *array[2] = {"foo", "bar"};

	fail_unless(!array_contains_string(array, "baz", 2), "\"baz\" should not be contained in the array.");
}
END_TEST

START_TEST(test_valid_path_to_file_match) {
	fail_unless(valid_path_to_file("/zouch"), "\"/zouch\" should be a valid file.");
}
END_TEST

START_TEST(test_valid_path_to_file_no_match) {
	fail_unless(valid_path_to_file("/foo"), "\"/foo\" should not be a valid file.");
}
END_TEST

START_TEST(test_unique_tags_in_path_zero_tags) {
	bool unique = unique_tags_in_path("/");

	fail_unless(unique, "Path should return true for \"/\"");
}
END_TEST

START_TEST(test_unique_tags_in_path_one_tag) {
	bool unique = unique_tags_in_path("/foo");

	fail_unless(unique, "Path should return true for \"/foo\"");
}
END_TEST

START_TEST(test_unique_tags_in_path_two_tags_unique) {
	bool unique = unique_tags_in_path("/foo/bar");

	fail_unless(unique, "Path should return true for \"/foo/bar\"");
}
END_TEST

START_TEST(test_unique_tags_in_path_two_tags_not_unique) {
	bool unique = unique_tags_in_path("/foo/foo");

	fail_unless(!unique, "Path should return false for \"/foo/foo\"");
}
END_TEST

START_TEST(test_valid_path_to_tag_zero_tags) {
	bool valid = valid_path_to_tag("/");

	fail_unless(valid, "/ should be a valid path.");
}
END_TEST

START_TEST(test_valid_path_to_tag_one_tag) {
	bool valid = valid_path_to_tag("/foo");

	fail_unless(valid, "/foo should be a valid path.");
}
END_TEST

START_TEST(test_invalid_path_to_tag_one_tag) {
	bool valid = valid_path_to_tag("/xyz");

	fail_unless(!valid, "/xyz should be an invalid path.");
}
END_TEST

START_TEST(test_valid_path_to_tag_two_tags) {
	bool valid = valid_path_to_tag("/foo/bar");

	fail_unless(valid, "/foo/bar should be a valid path.");
}
END_TEST

START_TEST(test_invalid_path_to_tag_two_tags) {
	bool valid = valid_path_to_tag("/foo/xyz");

	fail_unless(!valid, "/foo/xyz should be an invalid path.");
}
END_TEST

int main() {
	int num_failed = 0;
	SRunner *sr = NULL;

	Suite *common = suite_create("Common");

	TCase *tc_free_char_ptr_array = tcase_create("free_char_ptr_array");
	TCase *tc_path_to_array = tcase_create("path_to_array");
	TCase *tc_num_tags_in_path = tcase_create("num_tags_in_path");
	TCase *tc_array_contains_string = tcase_create("array_contains_string");
	TCase *tc_valid_path_to_file = tcase_create("valid_path_to_file");
	TCase *tc_unique_tags_in_path = tcase_create("unique_tags_in_path");
	TCase *tc_valid_path_to_tag = tcase_create("valid_path_to_tag");

	/* free_char_ptr_array */
	tcase_add_test(tc_free_char_ptr_array, test_free_char_ptr_array_null);
	tcase_add_test(tc_free_char_ptr_array, test_free_char_ptr_array_zero_elements);
	tcase_add_test(tc_free_char_ptr_array, test_free_char_ptr_array_one_element);
	tcase_add_test(tc_free_char_ptr_array, test_free_char_ptr_array_two_elements);
	suite_add_tcase(common, tc_free_char_ptr_array);

	/* path_to_array */
	tcase_add_test(tc_path_to_array, test_path_to_array_zero_tags);
	tcase_add_test(tc_path_to_array, test_path_to_array_one_tag);
	tcase_add_test(tc_path_to_array, test_path_to_array_two_tags);
	suite_add_tcase(common, tc_path_to_array);

	/* num_tags_in_path */
	tcase_add_test(tc_num_tags_in_path, test_num_tags_in_path_zero_tags);
	tcase_add_test(tc_num_tags_in_path, test_num_tags_in_path_one_tag);
	tcase_add_test(tc_num_tags_in_path, test_num_tags_in_path_two_tags);
	suite_add_tcase(common, tc_num_tags_in_path);

	/* array_contains_string */
	tcase_add_test(tc_array_contains_string, test_array_contains_string_match);
	tcase_add_test(tc_array_contains_string, test_array_contains_string_no_match);
	suite_add_tcase(common, tc_array_contains_string);

	/* valid_path_to_file */
	tcase_add_test(tc_valid_path_to_file, test_valid_path_to_file_match);
	tcase_add_test(tc_valid_path_to_file, test_valid_path_to_file_no_match);
	suite_add_tcase(common, tc_valid_path_to_file);

	/* unique_tags_in_path */
	tcase_add_test(tc_unique_tags_in_path, test_unique_tags_in_path_zero_tags);
	tcase_add_test(tc_unique_tags_in_path, test_unique_tags_in_path_one_tag);
	tcase_add_test(tc_unique_tags_in_path, test_unique_tags_in_path_two_tags_unique);
	tcase_add_test(tc_unique_tags_in_path, test_unique_tags_in_path_two_tags_not_unique);
	suite_add_tcase(common, tc_unique_tags_in_path);

	/* valid_path_to_tag */
	tcase_add_test(tc_valid_path_to_tag, test_valid_path_to_tag_zero_tags);
	tcase_add_test(tc_valid_path_to_tag, test_valid_path_to_tag_one_tag);
	tcase_add_test(tc_valid_path_to_tag, test_invalid_path_to_tag_one_tag);
	tcase_add_test(tc_valid_path_to_tag, test_valid_path_to_tag_two_tags);
	tcase_add_test(tc_valid_path_to_tag, test_invalid_path_to_tag_two_tags);
	suite_add_tcase(common, tc_valid_path_to_tag);

	sr = srunner_create(common);
	srunner_run_all(sr, CK_NORMAL);
	num_failed += srunner_ntests_failed(sr);
	srunner_free(sr);

	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* main */
