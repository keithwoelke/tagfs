/**
 * General functions that are not tagfs callbacks nor database functions.
 * @file tagfs_common.h
 * @author Keith Woelke
 * @date 30/07/2010
 */

#ifndef TAGFS_COMMON_H
#define TAGFS_COMMON_H

/**
 * Counts the number if digits in an integer.
 *
 * @param num The number from which to count digits.
 * @return The number of digits in num.
 */
int num_digits(int num);

#endif
