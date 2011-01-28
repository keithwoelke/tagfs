#include "tagfs_debug.h"

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

char *get_timestamp() {
	time_t seconds = time(NULL);
	char *time = ctime(&seconds);
	time[strlen(time) - 1] = '\0';

	return time;
}
