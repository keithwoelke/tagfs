/**
 * Debug related functions and macros.
 * @file tagfs_debug.h
 * @author Keith Woelke
 * @date 14/10/2010
 */
module tagfs_debug;
extern(C):
//#include <semaphore.h>
//TODO:

/* Debug level */
enum D_LEVEL_NONE             = 0x00;
enum D_LEVEL_INFORMATIONAL    = 0x01;
enum D_LEVEL_DEBUG            = 0x02;
enum D_LEVEL_WARNING          = 0x04;
enum D_LEVEL_ERROR            = 0x08;
enum D_LEVEL_ENTRY            = 0x10;
enum D_LEVEL_EXIT             = 0x20;
enum D_LEVEL_FOLDER_CONTENTS  = 0x40;

/* Debug meta-level */
enum DEBUG_LEVEL_NONE            = D_LEVEL_NONE;
enum DEBUG_LEVEL_INFORMATIONAL   = (D_LEVEL_INFORMATIONAL | D_LEVEL_WARNING | D_LEVEL_ERROR);
enum DEBUG_LEVEL_VERBOSE         = (D_LEVEL_INFORMATIONAL | D_LEVEL_DEBUG | D_LEVEL_WARNING | D_LEVEL_ERROR | D_LEVEL_ENTRY | D_LEVEL_EXIT);
enum DEBUG_LEVEL_ALL             = (D_LEVEL_INFORMATIONAL | D_LEVEL_DEBUG | D_LEVEL_WARNING | D_LEVEL_ERROR | D_LEVEL_ENTRY | D_LEVEL_EXIT | D_LEVEL_FOLDER_CONTENTS);

/* Final debug level */
enum DEBUG_LEVEL   = DEBUG_LEVEL_ALL;

/* Debug file */
enum D_FILE_NONE          = 0x0FFFFFF;
enum D_FILE_TAGFS_COMMON  = 0x1000000;
enum D_FILE_TAGFS_DB	     = 0x2000000;

/* Debug meta-file */
enum DEBUG_FILE_NONE      = D_FILE_NONE;
enum DEBUG_FILE_SANE      = (D_FILE_TAGFS_COMMON | D_FILE_TAGFS_DB);
enum DEBUG_FILE_ALL       = (D_FILE_TAGFS_COMMON | D_FILE_TAGFS_DB);

/* Final debug file */
enum DEBUG_FILE        = DEBUG_FILE_SANE;

/* Debug function */
enum D_FUNCTION_GET_FILE_DIRECTORY     = (0x0000001 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_FREE_CHAR_PTR_ARRAY    = (0x0000002 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_PATH_TO_ARRAY          = (0x0000004 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_NUM_TAGS_IN_PATH       = (0x0000008 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_ARRAY_CONTAINS_STRING  = (0x0000010 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_VALID_PATH_TO_FILE     = (0x0000020 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_UNIQUE_TAGS_IN_PATH    = (0x0000040 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_VALID_PATH_TO_TAG      = (0x0000080 | D_FILE_TAGFS_COMMON);
enum D_FUNCTION_NUM_DIGITS             = (0x0000100 | D_FILE_TAGFS_COMMON);

enum D_FUNCTION_DB_ENABLE_FOREIGN_KEYS       =(0x0000200 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_CONNECT                   =(0x0000400 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_DISCONNECT                =(0x0000800 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_COUNT_FROM_QUERY          =(0x0001000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_ARRAY_FROM_QUERY          =(0x0002000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_BUILD_FILE_QUERY          =(0x0004000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY =(0x0008000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_BUILD_TAG_QUERY           =(0x0010000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY = (0x0020000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_FILES_FROM_QUERY          =(0x0040000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_TAGS_FROM_QUERY           =(0x0080000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_DELETE_FILE               =(0x0100000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_GET_FILE_ID               =(0x0200000 | D_FILE_TAGFS_DB);
enum D_FUNCTION_DB_GET_FILE_LOCATION         =(0x0400000 | D_FILE_TAGFS_DB);

/* Debug meta-function */
/* tagfs_common */
enum DEBUG_FUNCTION_TAGFS_COMMON_SANE   = (D_FUNCTION_GET_FILE_DIRECTORY | D_FUNCTION_PATH_TO_ARRAY | D_FUNCTION_NUM_TAGS_IN_PATH | D_FUNCTION_ARRAY_CONTAINS_STRING | D_FUNCTION_VALID_PATH_TO_FILE | D_FUNCTION_UNIQUE_TAGS_IN_PATH | D_FUNCTION_VALID_PATH_TO_TAG | D_FUNCTION_NUM_DIGITS);
enum DEBUG_FUNCTION_TAGFS_COMMON_ALL    = (D_FUNCTION_GET_FILE_DIRECTORY | D_FUNCTION_FREE_CHAR_PTR_ARRAY | D_FUNCTION_PATH_TO_ARRAY | D_FUNCTION_NUM_TAGS_IN_PATH | D_FUNCTION_ARRAY_CONTAINS_STRING | D_FUNCTION_VALID_PATH_TO_FILE | D_FUNCTION_UNIQUE_TAGS_IN_PATH | D_FUNCTION_VALID_PATH_TO_TAG | D_FUNCTION_NUM_DIGITS);

/* tagfs_db */
enum DEBUG_FUNCTION_TAGFS_DB_SANE = (/*D_FUNCTION_DB_COUNT_FROM_QUERY | D_FUNCTION_DB_ARRAY_FROM_QUERY | D_FUNCTION_DB_BUILD_FILE_QUERY | D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY | D_FUNCTION_DB_BUILD_TAG_QUERY | D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY | D_FUNCTION_DB_FILES_FROM_QUERY | D_FUNCTION_DB_TAGS_FROM_QUERY | */D_FUNCTION_DB_DELETE_FILE);
enum DEBUG_FUNCTION_TAGFS_DB_ALL  = (D_FUNCTION_DB_ENABLE_FOREIGN_KEYS | D_FUNCTION_DB_CONNECT | D_FUNCTION_DB_DISCONNECT | D_FUNCTION_DB_COUNT_FROM_QUERY | D_FUNCTION_DB_ARRAY_FROM_QUERY | D_FUNCTION_DB_BUILD_FILE_QUERY | D_FUNCTION_DB_BUILD_RESTRICTED_FILE_QUERY | D_FUNCTION_DB_BUILD_TAG_QUERY | D_FUNCTION_DB_BUILD_RESTRICTED_TAG_QUERY | D_FUNCTION_DB_FILES_FROM_QUERY | D_FUNCTION_DB_TAGS_FROM_QUERY | D_FUNCTION_DB_DELETE_FILE);

/* Debug final function */
enum DEBUG_FUNCTION          = (/*DEBUG_FUNCTION_TAGFS_COMMON_SANE | */DEBUG_FUNCTION_TAGFS_DB_SANE);

/* Debug final */
enum DEBUG_PATH              = (DEBUG_FILE | (D_FILE_NONE & DEBUG_FUNCTION));

/*
 * Identical to the standand printf.
 * @param format String that contains the text to be written to stderr. It can optionally contain embedded format tags that are substituted by the values specified in subsequent argument(s). In this respect, the function is identical to printf, but outputs to stderr instead.
 * @param additional Depending on the format string, the function may expect a sequence of additional arguments, each containing one value to be inserted instead of each %-tag specified in the format parameter, if any. There should be the same number of these arguments as the number of %-tags that expect a value.
 * @return On success, the total number of characters written. On failure, a negative number is returned.
 */
void DEBUG(int, int, const char *, ...);
void debug_init();
void debug_indent();
void debug_deindent();
