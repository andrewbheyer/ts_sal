/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "rtLaser.h"


bool_t
xdr_rtLaserStruct(XDR *xdrs, rtLaserStruct *objp)
{
	if (!xdr_char(xdrs, &objp->laserIgnore)) {
		return (FALSE);
	}
	if (!xdr_char(xdrs, &objp->hazardDetected)) {
		return (FALSE);
	}
	return (TRUE);
}

