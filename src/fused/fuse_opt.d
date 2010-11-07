/* Converted to D from .\fuse_opt.h by htod */
module fuse_opt;
/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU LGPLv2.
  See the file COPYING.LIB.
*/

//C     #ifndef _FUSE_OPT_H_
//C     #define _FUSE_OPT_H_

/** @file
 *
 * This file defines the option parsing interface of FUSE
 */

//C     #ifdef __cplusplus
//C     extern "C" {
//C     #endif

/**
 * Option description
 *
 * This structure describes a single option, and and action associated
 * with it, in case it matches.
 *
 * More than one such match may occur, in which case the action for
 * each match is executed.
 *
 * There are three possible actions in case of a match:
 *
 * i) An integer (int or unsigned) variable determined by 'offset' is
 *    set to 'value'
 *
 * ii) The processing function is called, with 'value' as the key
 *
 * iii) An integer (any) or string (char *) variable determined by
 *    'offset' is set to the value of an option parameter
 *
 * 'offset' should normally be either set to
 *
 *  - 'offsetof(struct foo, member)'  actions i) and iii)
 *
 *  - -1			      action ii)
 *
 * The 'offsetof()' macro is defined in the <stddef.h> header.
 *
 * The template determines which options match, and also have an
 * effect on the action.  Normally the action is either i) or ii), but
 * if a format is present in the template, then action iii) is
 * performed.
 *
 * The types of templates are:
 *
 * 1) "-x", "-foo", "--foo", "--foo-bar", etc.	These match only
 *   themselves.  Invalid values are "--" and anything beginning
 *   with "-o"
 *
 * 2) "foo", "foo-bar", etc.  These match "-ofoo", "-ofoo-bar" or
 *    the relevant option in a comma separated option list
 *
 * 3) "bar=", "--foo=", etc.  These are variations of 1) and 2)
 *    which have a parameter
 *
 * 4) "bar=%s", "--foo=%lu", etc.  Same matching as above but perform
 *    action iii).
 *
 * 5) "-x ", etc.  Matches either "-xparam" or "-x param" as
 *    two separate arguments
 *
 * 6) "-x %s", etc.  Combination of 4) and 5)
 *
 * If the format is "%s", memory is allocated for the string unlike
 * with scanf().
 */
//C     struct fuse_opt {
	/** Matching template and optional parameter formatting */
//C     	const char *templ;

	/**
	 * Offset of variable within 'data' parameter of fuse_opt_parse()
	 * or -1
	 */
//C     	unsigned long offset;

	/**
	 * Value to set the variable to, or to be passed as 'key' to the
	 * processing function.	 Ignored if template has a format
	 */
//C     	int value;
//C     };

struct fuse_opt
{
    char *templ;
    uint offset;
    int value;
}

/**
 * Key option.	In case of a match, the processing function will be
 * called with the specified key.
 */
//C     #define FUSE_OPT_KEY(templ, key) { templ, -1U, key }

/**
 * Last option.	 An array of 'struct fuse_opt' must end with a NULL
 * template value
 */
//C     #define FUSE_OPT_END { NULL, 0, 0 }

/**
 * Argument list
 */
//C     struct fuse_args {
	/** Argument count */
//C     	int argc;

	/** Argument vector.  NULL terminated */
//C     	char **argv;

	/** Is 'argv' allocated? */
//C     	int allocated;
//C     };
struct fuse_args
{
    int argc;
    char **argv;
    int allocated;
}

/**
 * Initializer for 'struct fuse_args'
 */
//C     #define FUSE_ARGS_INIT(argc, argv) { argc, argv, 0 }

/**
 * Key value passed to the processing function if an option did not
 * match any template
 */
//C     #define FUSE_OPT_KEY_OPT     -1

const FUSE_OPT_KEY_OPT = -1;
/**
 * Key value passed to the processing function for all non-options
 *
 * Non-options are the arguments beginning with a charater other than
 * '-' or all arguments after the special '--' option
 */
//C     #define FUSE_OPT_KEY_NONOPT  -2

const FUSE_OPT_KEY_NONOPT = -2;
/**
 * Special key value for options to keep
 *
 * Argument is not passed to processing function, but behave as if the
 * processing function returned 1
 */
//C     #define FUSE_OPT_KEY_KEEP -3

const FUSE_OPT_KEY_KEEP = -3;
/**
 * Special key value for options to discard
 *
 * Argument is not passed to processing function, but behave as if the
 * processing function returned zero
 */
//C     #define FUSE_OPT_KEY_DISCARD -4

const FUSE_OPT_KEY_DISCARD = -4;
/**
 * Processing function
 *
 * This function is called if
 *    - option did not match any 'struct fuse_opt'
 *    - argument is a non-option
 *    - option did match and offset was set to -1
 *
 * The 'arg' parameter will always contain the whole argument or
 * option including the parameter if exists.  A two-argument option
 * ("-x foo") is always converted to single arguemnt option of the
 * form "-xfoo" before this function is called.
 *
 * Options of the form '-ofoo' are passed to this function without the
 * '-o' prefix.
 *
 * The return value of this function determines whether this argument
 * is to be inserted into the output argument vector, or discarded.
 *
 * @param data is the user data passed to the fuse_opt_parse() function
 * @param arg is the whole argument or option
 * @param key determines why the processing function was called
 * @param outargs the current output argument list
 * @return -1 on error, 0 if arg is to be discarded, 1 if arg should be kept
 */
//C     typedef int (*fuse_opt_proc_t)(void *data, const char *arg, int key,
//C     			       struct fuse_args *outargs);
extern (C):
alias int  function(void *data, char *arg, int key, fuse_args *outargs)fuse_opt_proc_t;

/**
 * Option parsing function
 *
 * If 'args' was returned from a previous call to fuse_opt_parse() or
 * it was constructed from
 *
 * A NULL 'args' is equivalent to an empty argument vector
 *
 * A NULL 'opts' is equivalent to an 'opts' array containing a single
 * end marker
 *
 * A NULL 'proc' is equivalent to a processing function always
 * returning '1'
 *
 * @param args is the input and output argument list
 * @param data is the user data
 * @param opts is the option description array
 * @param proc is the processing function
 * @return -1 on error, 0 on success
 */
//C     int fuse_opt_parse(struct fuse_args *args, void *data,
//C     		   const struct fuse_opt opts[], fuse_opt_proc_t proc);
int  fuse_opt_parse(fuse_args *args, void *data, fuse_opt *opts, fuse_opt_proc_t proc);

/**
 * Add an option to a comma separated option list
 *
 * @param opts is a pointer to an option list, may point to a NULL value
 * @param opt is the option to add
 * @return -1 on allocation error, 0 on success
 */
//C     int fuse_opt_add_opt(char **opts, const char *opt);
int  fuse_opt_add_opt(char **opts, char *opt);

/**
 * Add an option, escaping commas, to a comma separated option list
 *
 * @param opts is a pointer to an option list, may point to a NULL value
 * @param opt is the option to add
 * @return -1 on allocation error, 0 on success
 */
//C     int fuse_opt_add_opt_escaped(char **opts, const char *opt);
int  fuse_opt_add_opt_escaped(char **opts, char *opt);

/**
 * Add an argument to a NULL terminated argument vector
 *
 * @param args is the structure containing the current argument list
 * @param arg is the new argument to add
 * @return -1 on allocation error, 0 on success
 */
//C     int fuse_opt_add_arg(struct fuse_args *args, const char *arg);
int  fuse_opt_add_arg(fuse_args *args, char *arg);

/**
 * Add an argument at the specified position in a NULL terminated
 * argument vector
 *
 * Adds the argument to the N-th position.  This is useful for adding
 * options at the beggining of the array which must not come after the
 * special '--' option.
 *
 * @param args is the structure containing the current argument list
 * @param pos is the position at which to add the argument
 * @param arg is the new argument to add
 * @return -1 on allocation error, 0 on success
 */
//C     int fuse_opt_insert_arg(struct fuse_args *args, int pos, const char *arg);
int  fuse_opt_insert_arg(fuse_args *args, int pos, char *arg);

/**
 * Free the contents of argument list
 *
 * The structure itself is not freed
 *
 * @param args is the structure containing the argument list
 */
//C     void fuse_opt_free_args(struct fuse_args *args);
void  fuse_opt_free_args(fuse_args *args);


/**
 * Check if an option matches
 *
 * @param opts is the option description array
 * @param opt is the option to match
 * @return 1 if a match is found, 0 if not
 */
//C     int fuse_opt_match(const struct fuse_opt opts[], const char *opt);
int  fuse_opt_match(fuse_opt *opts, char *opt);

//C     #ifdef __cplusplus
//C     }
//C     #endif

//C     #endif /* _FUSE_OPT_H_ */
