#include "tagfs_debug.h"

#include <assert.h>
#include <string.h>

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
