#include "tagfs_debug.h"

#include <assert.h>
#include <semaphore.h>
#include <string.h>

sem_t debug_sem;

void debug_init() {
	int res = 0;

	res = sem_init(&debug_sem, 0, 1);

	if(res != 0) {
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

void log_timestamp() {
	char *buffer = NULL;
	char *format = "mm/dd/yy hh/mm/ss";
	int length = 0;
	struct tm *timeinfo;
	time_t rawtime;

	length = strlen(format);
	buffer = malloc(length * sizeof(*buffer) + 1);
	assert(buffer != NULL);

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, length + 1, "%x %X", timeinfo);

	fprintf(TAGFS_DATA->logfile, "%s", buffer);

	free(buffer);
}
