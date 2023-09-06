#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include "dsv-private.h"
#include "mem.h"
#include "file.h"
#include "common.h"

/***************************************************************************
 *  Name:
 *      xt_dsv_read_field() - Read next field from tabular input
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Read next delimiter-separated field from stream. The fields may be
 *      ended by any character in the string delims or by a newline ('\\\\n').
 *
 *      If the delimiter ending a field is a space, then subsequence spaces
 *      are discarded, so that multiple space characters serve as a single
 *      delimiter.
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      buff        Character buff into which field is copied
 *      buff_size   Size of the array passed to buff
 *      delims      Array of characters that may serve as delimiters
 *      len         Pointer to a variable which will receive the field length
 *
 *  Returns:
 *      Delimiter ending the field (either a member of delim or newline)
 *
 *  See also:
 *      xt_dsv_read_field_malloc(3), xt_dsv_skip_field(3),
 *      xt_dsv_skip_rest_of_line(3), xt_dsv_line_read(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-02-24  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_read_field(FILE *stream, char buff[], size_t buff_size,
		       const char *delims, size_t *len)

{
    size_t  c;
    char    *p;
    int     ch, ch2;

    // Quoted field
    // FIXME: Allow for alternate quote characters
    if ( (ch = getc(stream)) == '"' )
    {
	// Read to closing quote
	for (c = 0, p = buff; (c < buff_size) && 
			  ((ch = getc(stream)) != '"') &&
			  (ch != '\n') && (ch != EOF); ++c, ++p )
	    *p = ch;
	
	// Read to delimiter
	if ( (ch != '\n') && (ch != EOF) )
	    while ( (strchr(delims, ch = getc(stream)) == NULL) &&
			  (ch != '\n') && (ch != EOF) )
		;
    }
    else
    {
	ungetc(ch, stream);
	for (c = 0, p = buff; (c < buff_size) && 
			  ( strchr(delims, ch = getc(stream)) == NULL) &&
			  (ch != '\n') && (ch != EOF); ++c, ++p )
	    *p = ch;
    }
    *p = '\0';
    
    if ( c == buff_size )
    {
	fprintf(stderr, "xt_dsv_read_field(): Buffer overflow reading field.\n");
	fprintf(stderr, "Buffer size = %zu\n", buff_size);
	fputs(buff, stderr);
	// FIXME: Replace this with another sentinal value?
	// Would require all callers to handle both EOF and overflow
	exit(EX_SOFTWARE);
    }
    
    *len = c;
    
    /*
     *  Treat space specially in that multiple spaces are considered a single
     *  separator
     */
    if ( ch == ' ' )
    {
	while ( (ch2 = getc(stream)) == ch )
	    ;
	ungetc(ch2, stream);
    }
    return ch;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_read_field_malloc() - Read next field from tabular input,
 *                                allocating memory as needed
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Read next delimiter-separated field from stream, allocating a
 *      buffer to fit in the fashion of strdup(3). The fields may be
 *      ended by any character in the string delims or by a newline ('\\\\n').
 *
 *      If the delimiter ending a field is a space, then subsequence spaces
 *      are discarded, so that multiple space characters serve as a single
 *      delimiter.
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      buff        Character buffer into which field is copied
 *      buff_size   Size of the array passed to buff
 *      delims      Array of characters that may serve as delimiters
 *      len         Pointer to a variable which will receive the field length
 *
 *  Returns:
 *      Delimiter ending the field (either a member of delim or newline)
 *      or XT_MALLOC_FAILED.
 *
 *  See also:
 *      xt_dsv_read_field(3), xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3),
 *      xt_dsv_line_read(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-02-24  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_read_field_malloc(FILE *stream, char **buff, size_t *buff_size,
		       const char *delims, size_t *len)

{
    size_t  c;
    int     ch, ch2;
    
    if ( *buff_size == 0 )
    {
	*buff_size = 1024;
	*buff = xt_malloc(*buff_size, sizeof(**buff));
	if ( *buff == NULL )
	    return XT_MALLOC_FAILED;
    }
    
    for (c = 0; ( ((ch = getc(stream)) != '\n') && (ch != EOF) &&
		  strchr(delims, ch) == NULL); ++c )
    {
	if ( c == *buff_size - 1 )
	{
	    *buff_size *= 2;
	    *buff = xt_realloc(*buff, *buff_size, sizeof(**buff));
	    if ( *buff == NULL )
		return XT_MALLOC_FAILED;
	}
	(*buff)[c] = ch;
    }
    (*buff)[c] = '\0';
    *len = c;

    /* Trim array */
    if ( *buff_size != c + 1 )
    {
	*buff_size = c + 1;
	*buff = xt_realloc(*buff, *buff_size, sizeof(**buff));
    }

    /*
     *  Treat space specially in that multiple spaces are considered a single
     *  separator
     */
    if ( ch == ' ' )
    {
	while ( (ch2 = getc(stream)) == ch )
	    ;
	ungetc(ch2, stream);
    }
    return ch;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_skip_field() - Read and discard next field from tabular input
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Read and discard next delimiter-separated field from stream. The
 *      fields may be ended by any character in the string delims or by a
 *      newline ('\\\\n').
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      delims      Array of characters that may serve as delimiters
 *      len         Length of field discarded
 *
 *  Returns:
 *      Delimiter ending the field (either a member of delim or newline)
 *
 *  See also:
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_rest_of_line(3), xt_dsv_line_read(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-02-24  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_skip_field(FILE *stream, const char *delims, size_t *len)

{
    int     ch;
    
    for (*len = 0; (strchr(delims, ch = getc(stream)) == NULL) &&
	    (ch != '\n') && (ch != EOF); ++*len )
	;
    
    return ch;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_skip_rest_of_line() - Read and discard rest of input line
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Read and discard all remaining fields in a line from stream.
 *      I.e., discard everything up to and including the next newline ('\\\\n').
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *
 *  Returns:
 *      Delimiter ending the field (should always be newline ('\\\\n'))
 *
 *  See also:
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_line_read(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2019-12-06  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_skip_rest_of_line(FILE *stream)

{
    int     ch;
    
    while ( ((ch = getc(stream)) != EOF) && (ch != '\n') )
	;
    return ch;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_line_read() - Read a line of generic tabular data
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Read a line of an arbitrary DSV file into a xt_dsv_line_t object.
 *      Memory is allocated for the text of the fields, and it must
 *      be freed using fsv_line_free(3) before calling
 *      xt_dsv_line_read(3) on the same object again.
 *
 *      The xt_dsv_line_t structure contains an array of strings, each
 *      holding one field from the line, and an an array of delimiters,
 *      each holding the character that ended the corresponding field.
 *      Note that each field could potentially end with a different
 *      delimiter, as multiple delimiters can be specified.
 *
 *      This function serves a purpose similar to the split() functions
 *      present in many languages.  However, it does not need to read an
 *      entire line into a character array and then split the array.
 *      Instead, it separates fields as they are read from the input stream.
 *
 *  Arguments:
 *      xt_dsv_line    Pointer to a xt_dsv_line_t structure to hold the fields
 *      stream      FILE stream from which the line is read
 *      delims      Array of acceptable delimiters
 *
 *  Returns:
 *      Actual delimiter of last field (should be newline)
 *
 *  Examples:
 *      xt_dsv_line_t  *line = xt_dsv_line_new();
 *
 *      while ( xt_dsv_line_read(line, stdin, "\\\\\t") != EOF )
 *      {
 *          xt_dsv_line_write(line, stdout);
 *          xt_dsv_line_free(line);
 *      }
 *
 *  See also:
 *      xt_dsv_line_new(3), xt_dsv_line_free(3),
 *      xt_dsv_line_read(3), xt_dsv_line_write(3), xt_dsv_line_copy(3),
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-04-30  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_line_read(xt_dsv_line_t *xt_dsv_line, FILE *stream, const char *delims)

{
    int     actual_delim;
    char    field[DSV_FIELD_MAX_CHARS + 1];
    size_t  actual_len;
    
    xt_dsv_line->num_fields_array_size = 32;  // Start small and double each time we run out
    xt_dsv_line->num_fields = 0;
    
    // FIXME: Reuse previously allocated memory?
    if ( (xt_dsv_line->fields = xt_malloc(xt_dsv_line->num_fields_array_size,
				sizeof(*xt_dsv_line->fields))) == NULL )
    {
	fputs("xt_dsv_line_read(): Could not allocate fields.\n", stderr);
	exit(EX_UNAVAILABLE);
    }
    
    if ( (xt_dsv_line->delims = xt_malloc(xt_dsv_line->num_fields_array_size,
				sizeof(*xt_dsv_line->delims))) == NULL )
    {
	fputs("xt_dsv_line_read(): Could not allocate delims.\n", stderr);
	exit(EX_UNAVAILABLE);
    }
    
    // FIXME: Check actual_delim and/or actual_len to detect truncation
    while ( ((actual_delim = xt_dsv_read_field(stream, field,
		DSV_FIELD_MAX_CHARS, delims, &actual_len)) != EOF) )
    {
	if ( (xt_dsv_line->fields[xt_dsv_line->num_fields] = strdup(field)) == NULL )
	{
	    fprintf(stderr, "xt_dsv_line_read(): Could not strdup() field %zu.\n",
		    xt_dsv_line->num_fields - 1);
	    exit(EX_UNAVAILABLE);
	}
	
	xt_dsv_line->delims[xt_dsv_line->num_fields++] = actual_delim;
	
	if ( xt_dsv_line->num_fields == xt_dsv_line->num_fields_array_size )
	{
	    xt_dsv_line->num_fields_array_size *= 2;
	    if ( (xt_dsv_line->fields = xt_realloc(xt_dsv_line->fields,
		    xt_dsv_line->num_fields_array_size, sizeof(*xt_dsv_line->fields))) == NULL )
	    {
		fputs("xt_dsv_line_read(): Could not reallocate fields.\n", stderr);
		exit(EX_UNAVAILABLE);
	    }
	    
	    if ( (xt_dsv_line->delims = xt_realloc(xt_dsv_line->delims,
		    xt_dsv_line->num_fields_array_size, sizeof(*xt_dsv_line->delims))) == NULL )
	    {
		fputs("xt_dsv_line_read(): Could not reallocate delims.\n", stderr);
		exit(EX_UNAVAILABLE);
	    }
	}
	if ( actual_delim == '\n' )
	    break;
    }
    return actual_delim;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_line_write() - Write a line of generic tabular data
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Write an arbitrary DSV line to the specified stream.
 *      The xt_dsv_line_t structure contains an array of strings, each
 *      holding one field from the line, and an an array of delimiters,
 *      each holding the character that ended the corresponding field.
 *      Note that each field could potentially end with a different
 *      delimiter, as multiple delimiters can be specified.
 *
 *  Arguments:
 *      xt_dsv_line    Pointer to xt_dsv_line_t structure holding the fields
 *      stream      FILE stream to which fields are printed (e.g. stderr)
 *
 *  Returns:
 *      The number of fields successfully written
 *
 *  Examples:
 *      xt_dsv_line_t  *line = xt_dsv_line_new();
 *
 *      while ( xt_dsv_line_read(line, stdin, "\t") != EOF )
 *      {
 *          xt_dsv_line_write(line, stdout);
 *          xt_dsv_line_free(line);
 *      }
 *
 *  See also:
 *      xt_dsv_line_new(3), xt_dsv_line_free(3),
 *      xt_dsv_line_read(3), xt_dsv_line_write(3), xt_dsv_line_copy(3),
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-05-01  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_line_write(xt_dsv_line_t *xt_dsv_line, FILE *stream)

{
    int     c, count = 0;
    
    for (c = 0; c < xt_dsv_line->num_fields; ++c)
    {
	if ( fprintf(stream, "%s%c", xt_dsv_line->fields[c], xt_dsv_line->delims[c]) == 2 )
	    ++count;
    }
    return count;
}


/***************************************************************************
 *  Use auto-c2man to generate a man page from this comment
 *
 *  Name:
 *      xt_dsv_line_init() - Initialize generic tabular data object
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lxtend
 *
 *  Description:
 *      Initialize a xt_dsv_line_t structure.
 *      The xt_dsv_line_t structure contains an array of strings, each
 *      holding one field from the line, and an an array of delimiters,
 *      each holding the character that ended the corresponding field.
 *      Note that each field could potentially end with a different
 *      delimiter, as multiple delimiters can be specified.
 *
 *      Normally does not need to be called explicitly, since it is
 *      called by xt_dsv_line_new().
 *  
 *  Arguments:
 *      xt_dsv_line    Pointer to a xt_dsv_lint_t object.    
 *
 *  Examples:
 *      xt_dsv_line_t  *line = xt_dsv_line_new();
 *
 *      while ( xt_dsv_line_read(line, stdin, "\t") != EOF )
 *      {
 *          xt_dsv_line_write(line, stdout);
 *          xt_dsv_line_free(line);
 *      }
 *
 *      // Reinitialize
 *      xt_dsv_line_init(line);
 *
 *  See also:
 *      xt_dsv_line_new(3), xt_dsv_line_free(3),
 *      xt_dsv_line_read(3), xt_dsv_line_write(3), xt_dsv_line_copy(3),
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2022-04-11  Jason Bacon Begin
 ***************************************************************************/

void    xt_dsv_line_init(xt_dsv_line_t *xt_dsv_line)

{
    xt_dsv_line->num_fields_array_size = 0;
    xt_dsv_line->num_fields = 0;
    xt_dsv_line->fields = NULL;
    xt_dsv_line->delims = NULL;
}


/***************************************************************************
 *  Use auto-c2man to generate a man page from this comment
 *
 *  Name:
 *      xt_dsv_line_new() - Create a new generic tabular data object
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lxtend
 *
 *  Description:
 *      Allocate and initialize a xt_dsv_line_t structure.
 *      The xt_dsv_line_t structure contains an array of strings, each
 *      holding one field from the line, and an an array of delimiters,
 *      each holding the character that ended the corresponding field.
 *      Note that each field could potentially end with a different
 *      delimiter, as multiple delimiters can be specified.
 *  
 *  Arguments:
 *      None
 *
 *  Returns
 *      Pointer to a xt_dsv_lint_t object, or NULL if malloc() failed.
 *
 *  Examples:
 *      xt_dsv_line_t  *line = xt_dsv_line_new();
 *
 *      while ( xt_dsv_line_read(line, stdin, "\t") != EOF )
 *      {
 *          xt_dsv_line_write(line, stdout);
 *          xt_dsv_line_free(line);
 *      }
 *
 *  See also:
 *      xt_dsv_line_new(3), xt_dsv_line_free(3),
 *      xt_dsv_line_read(3), xt_dsv_line_write(3), xt_dsv_line_copy(3),
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2022-04-11  Jason Bacon Begin
 ***************************************************************************/


xt_dsv_line_t  *xt_dsv_line_new(void)

{
    xt_dsv_line_t  *line = malloc(sizeof(xt_dsv_line_t));
    
    if ( line == NULL )
    {
	fputs("xt_dsv_line_new(): malloc failed.\n", stderr);
	exit(EX_UNAVAILABLE);
    }
    
    xt_dsv_line_init(line);
    return line;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_line_copy() - Copy a generica tabular data object
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Duplicate an arbitrary xt_dsv_line_t object, allocating space for
 *      fields and delimiters as needed.
 *      The xt_dsv_line_t structure contains an array of strings, each
 *      holding one field from the line, and an an array of delimiters,
 *      each holding the character that ended the corresponding field.
 *      Note that each field could potentially end with a different
 *      delimiter, as multiple delimiters can be specified.
 *
 *  Arguments:
 *      src     Pointer to populated xt_dsv_line_t structure to be duplicated
 *      dest    Pointer to empty xt_dsv_lint_t structure to receive copy
 *
 *  Returns:
 *      XT_OK or XT_MALLOC_FAILED
 *      
 *  See also:
 *      xt_dsv_line_new(3), xt_dsv_line_free(3),
 *      xt_dsv_line_read(3), xt_dsv_line_write(3), xt_dsv_line_copy(3),
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-05-01  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_line_copy(xt_dsv_line_t *dest, xt_dsv_line_t *src)

{
    size_t  c;
    
    // Prune unused pointers in src
    dest->num_fields_array_size = dest->num_fields = src->num_fields;
    
    // FIXME: Check malloc() success
    dest->fields = xt_malloc(dest->num_fields_array_size, sizeof(*dest->fields));
    if ( dest->fields == NULL )
	return XT_MALLOC_FAILED;
    dest->delims = xt_malloc(dest->num_fields_array_size, sizeof(*dest->delims));
    if ( dest->delims == NULL )
	return XT_MALLOC_FAILED;
    
    for (c = 0; c < src->num_fields; ++c)
    {
	if ( (dest->fields[c] = strdup(src->fields[c])) == NULL )
	    return XT_MALLOC_FAILED;
	dest->delims[c] = src->delims[c];
    }
    return XT_OK;
}


/***************************************************************************
 *  Name:
 *      xt_dsv_line_free() - Destroy a generic tabular data object
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Free allocated memory for a DSV object.
 *      The xt_dsv_line_t structure contains an array of strings, each
 *      holding one field from the line, and an an array of delimiters,
 *      each holding the character that ended the corresponding field.
 *      Note that each field could potentially end with a different
 *      delimiter, as multiple delimiters can be specified.
 *
 *  Arguments:
 *      xt_dsv_line    Pointer to a populated xt_dsv_line_t structure
 *
 *  Returns:
 *      The number of fields freed.  Fields set to NULL are not freed.
 *
 *  Examples:
 *      xt_dsv_line_t  *line = xt_dsv_line_new();
 *
 *      while ( xt_dsv_line_read(line, stdin, "\t") != EOF )
 *      {
 *          xt_dsv_line_write(line, stdout);
 *          xt_dsv_line_free(line);
 *      }
 *
 *  See also:
 *      xt_dsv_line_new(3), xt_dsv_line_free(3),
 *      xt_dsv_line_read(3), xt_dsv_line_write(3), xt_dsv_line_copy(3),
 *      xt_dsv_read_field(3), xt_dsv_read_field_malloc(3),
 *      xt_dsv_skip_field(3), xt_dsv_skip_rest_of_line(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-05-01  Jason Bacon Begin
 ***************************************************************************/

int     xt_dsv_line_free(xt_dsv_line_t *xt_dsv_line)

{
    int     c, count = 0;
    
    if ( xt_dsv_line->fields != NULL )
    {
	for (c = 0; c < xt_dsv_line->num_fields; ++c)
	    if ( xt_dsv_line->fields[c] != NULL )
	    {
		free(xt_dsv_line->fields[c]);
		++count;
	    }
	if ( xt_dsv_line->fields != NULL )
	    free(xt_dsv_line->fields);
    }
    xt_dsv_line->num_fields = 0;
    return count;
}


/***************************************************************************
 *  Name:
 *      xt_tsv_read_field() - Read next tab-separated field
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_read_field(stream, buff, buff_size, '\\\\\t', len)
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      buff        Character buff into which field is copied
 *      buff_size   Size of the array passed to buff
 *      len         Pointer to a variable which will receive the field length
 *
 *  See also:
 *      xt_dsv_read_field(3)
 ***************************************************************************/

int     xt_tsv_read_field(FILE *stream, char buff[], size_t buff_size,
		       size_t *len)

{
    return xt_dsv_read_field(stream, buff, buff_size, "\t", len);
}


/***************************************************************************
 *  Name:
 *      xt_tsv_read_field_malloc() - Read next tab-separated field, allocating
 *                                memory as needed
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_read_field_malloc(stream, *buff, *buff_size, '\\\\\t', len)
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      buff        Character buff into which field is copied
 *      buff_size   Size of the array passed to buff
 *      len         Pointer to a variable which will receive the field length
 *
 *  See also:
 *      xt_dsv_read_field_malloc(3)
 ***************************************************************************/

int     xt_tsv_read_field_malloc(FILE *stream, char **buff, size_t *buff_size,
		       size_t *len)

{
    return xt_dsv_read_field_malloc(stream, buff, buff_size, "\t", len);
}


/***************************************************************************
 *  Name:
 *      xt_tsv_skip_field() - Read and discard next tab-separated field
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_skip_field(stream, '\\\\\t')
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      len         Length of field discarded
 *
 *  See also:
 *      xt_dsv_skip_field(3)
 ***************************************************************************/

int     xt_tsv_skip_field(FILE *stream, size_t *len)

{
    return xt_dsv_skip_field(stream, "\t", len);
}


/***************************************************************************
 *  Name:
 *      xt_tsv_skip_rest_of_line() - Read and discard rest of input line
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_skip_rest_of_line(stream)
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *
 *  See also:
 *      xt_dsv_skip_rest_of_line(3)
 ***************************************************************************/

int     xt_tsv_skip_rest_of_line(FILE *stream)

{
    return xt_dsv_skip_rest_of_line(stream);
}


/***************************************************************************
 *  Name:
 *      xt_csv_read_field() - Read next comma-separated field
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_read_field(stream, buff, buff_size, ',', len)
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      buff        Character buff into which field is copied
 *      buff_size   Size of the array passed to buff
 *      len         Pointer to a variable which will receive the field length
 *
 *  See also:
 *      xt_dsv_read_field(3)
 ***************************************************************************/

int     xt_csv_read_field(FILE *stream, char buff[], size_t buff_size,
		       size_t *len)

{
    return xt_dsv_read_field(stream, buff, buff_size, ",", len);
}


/***************************************************************************
 *  Name:
 *      xt_csv_read_field_malloc() - Read next comma-separated field, allocating
 *                                memory as needed
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_read_field_malloc(stream, *buff, *buff_size, ',', len)
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      buff        Character buff into which field is copied
 *      buff_size   Size of the array passed to buff
 *      len         Pointer to a variable which will receive the field length
 *
 *  See also:
 *      xt_dsv_read_field_malloc(3)
 ***************************************************************************/

int     xt_csv_read_field_malloc(FILE *stream, char **buff, size_t *buff_size,
		       size_t *len)

{
    return xt_dsv_read_field_malloc(stream, buff, buff_size, ",", len);
}


/***************************************************************************
 *  Name:
 *      xt_csv_skip_field() - Read and discard next comma-separated field
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_skip_field(stream, ',')
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *      len         Length of field discarded
 *
 *  See also:
 *      xt_dsv_skip_field(3)
 ***************************************************************************/

int     xt_csv_skip_field(FILE *stream, size_t *len)

{
    return xt_dsv_skip_field(stream, ",", len);
}


/***************************************************************************
 *  Name:
 *      xt_csv_skip_rest_of_line() - Read and discard rest of input line
 *
 *  Library:
 *      #include <xtend/dsv.h>
 *      -lbiolibc
 *
 *  Description:
 *      Equivalent to xt_dsv_skip_rest_of_line(stream)
 *
 *  Arguments:
 *      stream      FILE stream from which field is read
 *
 *  See also:
 *      xt_dsv_skip_rest_of_line(3)
 ***************************************************************************/

int     xt_csv_skip_rest_of_line(FILE *stream)

{
    return xt_dsv_skip_rest_of_line(stream);
}
