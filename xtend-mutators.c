/***************************************************************************
 *  This file is automatically generated by gen-get-set.  Be sure to keep
 *  track of any manual changes.
 *
 *  These generated functions are not expected to be perfect.  Check and
 *  edit as needed before adding to your code.
 ***************************************************************************/

#include <string.h>
#include <ctype.h>
#include "xtend.h"


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for array_size member in a dsv_line_t structure.
 *      Use this function to set array_size in a dsv_line_t variable
 *      from non-member functions.  This function performs a direct
 *      assignment for scalar or pointer structure members.  If
 *      array_size is a pointer, data previously pointed to should
 *      generally be freed before calling this function to avoid memory
 *      leaks.
 *
 *      Note that there is an equivalent macro (), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_array_size is guaranteed by other means.
 *      
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      new_array_size  The new value for array_size
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      size_t          new_array_size;
 *
 *      if ( dsv_line_set_array_size(&dsv_line, new_array_size) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      (3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_array_size(dsv_line_t *dsv_line_ptr, size_t new_array_size)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( 0 )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	dsv_line_ptr->array_size = new_array_size;
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for num_fields member in a dsv_line_t structure.
 *      Use this function to set num_fields in a dsv_line_t variable
 *      from non-member functions.  This function performs a direct
 *      assignment for scalar or pointer structure members.  If
 *      num_fields is a pointer, data previously pointed to should
 *      generally be freed before calling this function to avoid memory
 *      leaks.
 *
 *      Note that there is an equivalent macro (), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_num_fields is guaranteed by other means.
 *      
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      new_num_fields  The new value for num_fields
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      size_t          new_num_fields;
 *
 *      if ( dsv_line_set_num_fields(&dsv_line, new_num_fields) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      (3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_num_fields(dsv_line_t *dsv_line_ptr, size_t new_num_fields)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( 0 )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	dsv_line_ptr->num_fields = new_num_fields;
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for fields member in a dsv_line_t structure.
 *      Use this function to set fields in a dsv_line_t variable
 *      from non-member functions.  This function performs a direct
 *      assignment for scalar or pointer structure members.  If
 *      fields is a pointer, data previously pointed to should
 *      generally be freed before calling this function to avoid memory
 *      leaks.
 *
 *      Note that there is an equivalent macro (), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_fields is guaranteed by other means.
 *      
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      new_fields      The new value for fields
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      char **         new_fields;
 *
 *      if ( dsv_line_set_fields(&dsv_line, new_fields) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      (3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_fields(dsv_line_t *dsv_line_ptr, char ** new_fields)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( new_fields == NULL )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	dsv_line_ptr->fields = new_fields;
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for an array element of fields member in a dsv_line_t
 *      structure. Use this function to set an element of the array
 *      fields in a dsv_line_t variable from non-member functions.
 *
 *      Note that there is an equivalent macro DSV_LINE_SET_FIELDS_AE(), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_fields_element is guaranteed by other means.
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      c               Subscript to the fields array
 *      new_fields_element The new value for fields[c]
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      size_t          c;
 *      char **         new_fields_element;
 *
 *      if ( dsv_line_set_fields(&dsv_line, c, new_fields_element) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      DSV_LINE_SET_FIELDS_AE(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_fields_ae(dsv_line_t *dsv_line_ptr, size_t c, char * new_fields_element)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( new_fields_element == NULL )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	dsv_line_ptr->fields[c] = new_fields_element;
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for fields member in a dsv_line_t structure.
 *      Use this function to set fields in a dsv_line_t variable
 *      from non-member functions.  This function copies the array pointed to
 *      by new_fields to ->fields.
 *
 *      Note that there is an equivalent macro DSV_LINE_SET_FIELDS(), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_fields is guaranteed by other means.
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      new_fields      The new value for fields
 *      array_size      Size of the fields array.
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      char **         new_fields;
 *      size_t          array_size;
 *
 *      if ( dsv_line_set_fields(&dsv_line, new_fields, array_size) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      DSV_LINE_SET_FIELDS(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_fields_cpy(dsv_line_t *dsv_line_ptr, char ** new_fields, size_t array_size)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( new_fields == NULL )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	{
	    size_t  c;
	    
	    // FIXME: Assuming all elements should be copied
	    for (c = 0; c < array_size; ++c)
		dsv_line_ptr->fields[c] = new_fields[c];
	}
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for delims member in a dsv_line_t structure.
 *      Use this function to set delims in a dsv_line_t variable
 *      from non-member functions.  This function performs a direct
 *      assignment for scalar or pointer structure members.  If
 *      delims is a pointer, data previously pointed to should
 *      generally be freed before calling this function to avoid memory
 *      leaks.
 *
 *      Note that there is an equivalent macro (), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_delims is guaranteed by other means.
 *      
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      new_delims      The new value for delims
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      char *          new_delims;
 *
 *      if ( dsv_line_set_delims(&dsv_line, new_delims) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      (3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_delims(dsv_line_t *dsv_line_ptr, char * new_delims)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( new_delims == NULL )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	dsv_line_ptr->delims = new_delims;
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for an array element of delims member in a dsv_line_t
 *      structure. Use this function to set an element of the array
 *      delims in a dsv_line_t variable from non-member functions.
 *
 *      Note that there is an equivalent macro DSV_LINE_SET_DELIMS_AE(), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_delims_element is guaranteed by other means.
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      c               Subscript to the delims array
 *      new_delims_element The new value for delims[c]
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      size_t          c;
 *      char *          new_delims_element;
 *
 *      if ( dsv_line_set_delims(&dsv_line, c, new_delims_element) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      DSV_LINE_SET_DELIMS_AE(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_delims_ae(dsv_line_t *dsv_line_ptr, size_t c, char  new_delims_element)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( 0 )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	dsv_line_ptr->delims[c] = new_delims_element;
	return XT_DATA_OK;
    }
}


/***************************************************************************
 *  Library:
 *      #include <XT_xtend.h>
 *      -lxtend
 *
 *  Description:
 *      Mutator for delims member in a dsv_line_t structure.
 *      Use this function to set delims in a dsv_line_t variable
 *      from non-member functions.  This function copies the array pointed to
 *      by new_delims to ->delims.
 *
 *      Note that there is an equivalent macro DSV_LINE_SET_DELIMS(), which performs
 *      this function with no data verification or function call overhead.
 *      Use the macro version to maximize performance where the validity
 *      of new_delims is guaranteed by other means.
 *
 *  Arguments:
 *      dsv_line_ptr    Pointer to the structure to set
 *      new_delims      The new value for delims
 *      array_size      Size of the delims array.
 *
 *  Returns:
 *      XT_DATA_OK if the new value is acceptable and assigned
 *      XT_DATA_OUT_OF_RANGE otherwise
 *
 *  Examples:
 *      dsv_line_t      dsv_line;
 *      char *          new_delims;
 *      size_t          array_size;
 *
 *      if ( dsv_line_set_delims(&dsv_line, new_delims, array_size) == XT_DATA_OK )
 *      {
 *      }
 *
 *  See also:
 *      DSV_LINE_SET_DELIMS(3)
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-08-10  gen-get-set Auto-generated from xtend.h
 ***************************************************************************/

int     dsv_line_set_delims_cpy(dsv_line_t *dsv_line_ptr, char * new_delims, size_t array_size)

{
    /* FIXME: Replace this with a proper sanity check */
    if ( new_delims == NULL )
	return XT_DATA_OUT_OF_RANGE;
    else
    {
	// FIXME: Assuming char array is a null-terminated string
	strlcpy(dsv_line_ptr->delims, new_delims, array_size);
	return XT_DATA_OK;
    }
}
