#include "../src/tagfs_db.h"
#include "../src/tagfs_common.h"
#include "../src/tagfs.h"

#include <check.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(void) {
	int num_failed = 0;

	Suite *tagfs = suite_create("TagFS");

	SRunner *sr = srunner_create(db);
	srunner_run_all(sr, CK_NORMAL);
	num_failed += srunner_ntests_failed(sr);
	srunner_free(sr);

	sr = srunner_create(tagfs);
	srunner_run_all(sr, CK_NORMAL);
	num_failed += srunner_ntests_failed(sr);
	srunner_free(sr);

	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* main */
