fused=fused/statvfs.d fused/fuse_opt.d fused/fuse_helper.d fused/fuse.d fused/fuse_common.d
FLAGS=-L-lfuse -L-lrt -L-ldl -L-lsqlite3

tagfs : tagfs.d tagfs_db.o tagfs_common.d tagfs_db.d tagfs_debug.d tagfs_debug.o callback.o
	dmd -gc tagfs.d tagfs_common.d tagfs_db.d tagfs_debug.d tagfs_db.o tagfs_debug.o callback.o $(fused) $(FLAGS)
tagfs_db.o: tagfs_db.c tagfs_debug.c
	gcc -g -lsqlite3 -Wall -c `pkg-config fuse --cflags --libs` tagfs_db.c
tagfs_debug.o: tagfs_debug.c
	gcc -g -Wall -c tagfs_debug.c
callback.o: callback.c
	gcc -g -Wall -c `pkg-config fuse --cflags --libs` callback.c
