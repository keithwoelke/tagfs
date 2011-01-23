#include "tagfs_debug.h"

sem_t debug_sem;

FILE *debug_open_log(char *logpath) {
	FILE *logfile = NULL;

	logfile = fopen(logpath, "w");

	if(logfile == NULL) {
		fprintf(stderr, "WARNING: Could not open log file.\n");
	}

	return logfile;
} /* debug_open_log */

void debug_close_log(FILE *logfile) {
	int retstat = 0;

	assert(logfile != NULL);
	retstat = fclose(logfile);
	logfile = NULL;

	if(retstat != 0) {
		fprintf(stderr, "WARNING: Could not close log file.\n");
	}
} /* debug_close_log */

void debug_init() {
	int res = 0;

	res = sem_init(&debug_sem, 0, 1);

	if(res < 0) {
		fprintf(stderr, "ERROR: A problem occured during initialization.");
		exit(EXIT_FAILURE);
	}
} /* debug_init */

void debug_wait() {
	sem_wait(&debug_sem);
} /* debug_wait */

void debug_post() {
	sem_post(&debug_sem);
} /* debug_post */











//void DEBUG(int function, int msg_level, const char *format, ...) {
//	#if defined(NDEBUG)
//	#else
//		int i = 0;
//		va_list ap;
//
//		assert(function >= 0);
//		assert(msg_level >= 0);
//		assert(format != NULL);
//
//		va_start(ap, format);
//		/*if(function == 0 || msg_level == 0 || (DEBUG_PATH & DEBUG_FILE) == 0) { }
//		else */if((DEBUG_PATH & function) == function && (DEBUG_LEVEL & msg_level) == msg_level) {
//			sem_wait(&debug_sem);
//
//			if(msg_level == D_LEVEL_EXIT) { debug_indent_level--; }
//			for(i = 0; i < debug_indent_level; i++) { (void)printf("  "); }
//			if(msg_level == D_LEVEL_ENTRY) { printf("--> "); }
//			else if(msg_level == D_LEVEL_EXIT) { printf("<-- "); }
//			if((DEBUG_PATH & D_FILE_TAGFS_COMMON & function) == D_FILE_TAGFS_COMMON) { printf("[common] "); }
//			if((DEBUG_PATH & D_FILE_TAGFS_DB & function) == D_FILE_TAGFS_DB) { printf("[db] "); }
//
//			if(msg_level == D_LEVEL_WARNING) {
//				(void)printf("WARNING: ");
//				(void)vprintf(format, ap);
//			}
//			else if(msg_level == D_LEVEL_ERROR) {
//				(void)printf("ERROR: ");
//				(void)vprintf(format, ap);
//				printf("\n");
//				exit(EXIT_FAILURE);
//			}
//			else {
//				(void)vprintf(format, ap);
//			}
//			printf("\n");
//
//			if(msg_level == D_LEVEL_ENTRY) { debug_indent_level++; }
//
//			sem_post(&debug_sem);
//		}
////		if((DEBUG_PATH & function) == function && msg_level == D_LEVEL_EXIT && (DEBUG_LEVEL & msg_level) == 0) { printf("\n"); }
//		va_end(ap);
//	#endif
//} /* DEBUG */

//void debug_indent() {
//	sem_wait(&debug_sem);
//	debug_indent_level++;
//	sem_post(&debug_sem);
//}
//void debug_deindent() {
//	sem_wait(&debug_sem);
//	debug_indent_level--;
//	sem_post(&debug_sem);
//}
