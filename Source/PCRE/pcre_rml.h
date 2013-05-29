#ifndef _PCRE_RML_H
#define _PCRE_RML_H

#include <pcre.h>

inline pcre* compile_prce (const char* pattern)
{
	int erroffset;
	const char *error;
	pcre *re = pcre_compile(pattern,
						0,                    /* default options */
						&error,               /* for error message */
						&erroffset,           /* for error offset */
						NULL);                /* use default character tables */
	if (re == NULL)
	{
		ErrorMessage (Format("cannot parse regular expression \"%s\" offset=%i, error=%s",pattern, erroffset, error));
		return 0;
	}
	return re;
};

inline bool has_regular_expression (const pcre* re, const char* subject, size_t subject_len)
{
	const int OVECCOUNT = 30;    /* should be a multiple of 3 */
	int ovector[OVECCOUNT];
	int rc = pcre_exec(re,NULL,                 
				subject,              /* the subject string */
				subject_len,       /* the length of the subject */
				0,                    /* start at offset 0 in the subject */
				0,                    /* default options */
				ovector,              /* output vector for substring information */
				OVECCOUNT);           /* number of elements in the output vector */

	return rc >= 0;
};

#endif
