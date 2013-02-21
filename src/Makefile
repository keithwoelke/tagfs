tagfs : tagfs.c tagfs_db.c tagfs_common.c tagfs_debug.c
	gcc -g -Wall -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ tagfs.c tagfs_db.c tagfs_common.c tagfs_debug.c -o tagfs `pkg-config fuse --cflags --libs` -lsqlite3 -I/usr/include/glib-2.0/ -lglib-2.0

run : tagfs
	./tagfs -f -s TagFS

run-valgrind : tagfs
	export G_DEBUG=gc-friendly && export G_SLICE=always-malloc && valgrind --leak-check=full ./tagfs -f TagFS

clean :
	rm tagfs
	fusermount -qu TagFS

unmount :
	fusermount -qu TagFS

delete :
	rm tagfs
