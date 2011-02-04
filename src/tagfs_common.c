#include "tagfs_debug.h"

int num_digits(unsigned int num) {
	int count = 0;

	DEBUG(ENTRY);
	DEBUG("Counting number of digits in %u", num);

	do {
		num /= 10;
		count++;
	} while(num != 0);

	DEBUG("Number of digits: %d", count);
	DEBUG(EXIT);
	return count;
} /* num_digits */
