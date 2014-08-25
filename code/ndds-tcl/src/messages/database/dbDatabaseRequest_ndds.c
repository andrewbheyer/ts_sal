/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "dbDatabaseRequest.h"


bool_t
xdr_dbDatabaseRequestStruct(XDR *xdrs, dbDatabaseRequestStruct *objp)
{
	if (!xdr_int(xdrs, &objp->requestSource)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->function)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->fieldName)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->targetID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->numResults)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->int_data, 250, sizeof(int), xdr_int)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->int_len)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->float_data, 250, sizeof(float), xdr_float)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->float_len)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->char_data, 1500, sizeof(char), xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->char_len)) {
		return (FALSE);
	}
	return (TRUE);
}

