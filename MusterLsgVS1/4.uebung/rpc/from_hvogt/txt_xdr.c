/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "txt.h"

bool_t
xdr_text_record(xdrs, objp)
	register XDR *xdrs;
	text_record *objp;
{

	register long *buf;

	if (!xdr_string(xdrs, &objp->element_text, MAX_STR))
		return (FALSE);
	return (TRUE);
}
