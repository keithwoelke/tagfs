tagfs :
	gcc -g -lsqlite3 -Wall `pkg-config fuse --cflags --libs` tagfs.c tagfs_db.c tagfs_common.c tagfs_debug.c -o tagfs
run : tagfs
	./tagfs TagFS
