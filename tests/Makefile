check_tagfs_db = gcc -Wall -ansi -pedantic check_tagfs_db.c ../src/tagfs_common.c ../src/tagfs_db.c -o check_tagfs_db -lsqlite3 -lcheck
check_tagfs_common = gcc -Wall -ansi -pedantic check_tagfs_common.c ../src/tagfs_common.c ../src/tagfs_db.c -o check_tagfs_common -lsqlite3 -lcheck

all : check_tagfs_db

run-all : check_tagfs_db check_tagfs_common
	./check_tagfs_db; ./check_tagfs_common

run-all-NDEBUG : check_tagfs_db-NDEBUG check_tagfs_common-NDEBUG
	./check_tagfs_db; ./check_tagfs_common

run-check_tagfs_db : check_tagfs_db
	./check_tagfs_db

run-check_tagfs_db-NDEBUG : check_tagfs_db-NDEBUG
	./check_tagfs_db

check_tagfs_db :
	$(check_tagfs_db)

check_tagfs_db-NDEBUG :
	$(check_tagfs_db) -DNDEBUG

run-check_tagfs_common : check_tagfs_common-NDEBUG
	./check_tagfs_common

run-check_tagfs_common-NDEBUG : check_tagfs_common-NDEBUG
	./check_tagfs_common

check_tagfs_common :
	$(check_tagfs_common)

check_tagfs_common-NDEBUG :
	$(check_tagfs_common) -DNDEBUG

clean :
	rm -f check_tagfs_db check_tagfs_common
