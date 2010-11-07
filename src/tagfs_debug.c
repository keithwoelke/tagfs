#include "tagfs_debug.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int debug_indent_level = 0;
sem_t debug_sem;

void DEBUG(int function, int msg_level, char *format, ...) {
	#if defined(NDEBUG)
	#else
		int i = 0;
		va_list ap;

		assert(function >= 0);
		assert(msg_level >= 0);
		assert(format != NULL);

		va_start(ap, format);
		/*if(function == 0 || msg_level == 0 || (DEBUG_PATH & DEBUG_FILE) == 0) { }
		else */if((DEBUG_PATH & function) == function && (DEBUG_LEVEL & msg_level) == msg_level) {
			sem_wait(&debug_sem);

			if(msg_level == D_LEVEL_EXIT) { debug_indent_level--; }
			for(i = 0; i < debug_indent_level; i++) { (void)printf("  "); }
			if(msg_level == D_LEVEL_ENTRY) { printf("--> "); }
			else if(msg_level == D_LEVEL_EXIT) { printf("<-- "); }
//			printf("\nDEBUG_PATH: %d\n", DEBUG_PATH);
//			printf("D_FILE_TAGFS_COMMON: %d\n", D_FILE_TAGFS_COMMON);
//			printf("D_FILE_TAGFS_DB: %d\n", D_FILE_TAGFS_DB);
//			printf("function: %d\n", function);
			if((DEBUG_PATH & D_FILE_TAGFS_COMMON & function) == D_FILE_TAGFS_COMMON) { printf("[common] "); }
			if((DEBUG_PATH & D_FILE_TAGFS_DB & function) == D_FILE_TAGFS_DB) { printf("[db] "); }

			if(msg_level == D_LEVEL_WARNING) {
				(void)printf("WARNING: ");
				(void)vprintf(format, ap);
			}
			else if(msg_level == D_LEVEL_ERROR) {
				(void)printf("ERROR: ");
				(void)vprintf(format, ap);
				printf("\n");
				exit(EXIT_FAILURE);
			}
			else {
				(void)vprintf(format, ap);
			}
			printf("\n");

			if(msg_level == D_LEVEL_ENTRY) { debug_indent_level++; }

			sem_post(&debug_sem);
		}
//		if((DEBUG_PATH & function) == function && msg_level == D_LEVEL_EXIT && (DEBUG_LEVEL & msg_level) == 0) { printf("\n"); }
		va_end(ap);
	#endif
} /* DEBUG */

void debug_init() {
	sem_init(&debug_sem, 0, 1);
}

void debug_indent() {
	sem_wait(&debug_sem);
	debug_indent_level++;
	sem_post(&debug_sem);
}
void debug_deindent() {
	sem_wait(&debug_sem);
	debug_indent_level++;
	sem_post(&debug_sem);
}
