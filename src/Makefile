tagfs :
	gcc -g -lsqlite3 -lglib-2.0 -Wall `pkg-config fuse --cflags --libs` -I/usr/include/glib-2.0/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ tagfs.c tagfs_db.c tagfs_common.c tagfs_debug.c -o tagfs

run : tagfs
	./tagfs -f TagFS

run-valgrind : tagfs
	valgrind ./tagfs -f TagFS

clean :
	rm tagfs
	fusermount -qu TagFS
