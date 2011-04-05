#include "tagfs_debug.h"

#include <assert.h>
#include <semaphore.h>
#include <string.h>

sem_t debug_sem;

void log_timestamp() {
	char *buf = NULL;
	const char *format = "mm/dd/yy hh/mm/ss";
	int length = 0;
	struct tm *timeinfo;
	time_t rawtime;

	length = strlen(format);
	buf = malloc(length * sizeof(*buf) + 1);
	assert(buf != NULL);

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buf, length + 1, "%x %X", timeinfo);

	fprintf(TAGFS_DATA->log_file, "%s", buf);

	free(buf);
} /* log_timestamp */

void debug_init() {
	sem_init(&debug_sem, 0, 1);
}

void debug_wait() {
	sem_wait(&debug_sem);
}
void debug_post() {
	sem_post(&debug_sem);
}
