/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "weatherState.h"


bool_t
xdr_weatherStateStruct(XDR *xdrs, weatherStateStruct *objp)
{
	if (!xdr_int(xdrs, &objp->insideTemperature)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->outsideTemperature)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->barometer)) {
		return (FALSE);
	}
	if (!xdr_u_char(xdrs, &objp->insideHumidity)) {
		return (FALSE);
	}
	if (!xdr_u_char(xdrs, &objp->outsideHumidity)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->DewPoint)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->rain)) {
		return (FALSE);
	}
	if (!xdr_u_char(xdrs, &objp->windSpeed)) {
		return (FALSE);
	}
	if (!xdr_u_char(xdrs, &objp->windGust)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->WindChill)) {
		return (FALSE);
	}
	if (!xdr_u_char(xdrs, &objp->windDirection)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->msecs)) {
		return (FALSE);
	}
	if (!xdr_u_char(xdrs, &objp->week)) {
		return (FALSE);
	}
	return (TRUE);
}

