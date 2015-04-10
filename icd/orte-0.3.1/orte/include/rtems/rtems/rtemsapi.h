/**
 * @file rtems/rtems/rtemsapi.h
 */

/*
 *  RTEMS API Support
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsapi.h,v 1.10 2005/01/28 05:00:20 ralf Exp $
 */

#ifndef _RTEMS_RTEMS_RTEMSAPI_H
#define _RTEMS_RTEMS_RTEMSAPI_H

#include <rtems/config.h>

/*PAGE
 *
 *  _RTEMS_API_Initialize
 *
 *  This routine initializes the RTEMS API by invoking the initialization
 *  routine for each RTEMS manager with the appropriate parameters
 *  from the configuration_table.
 */

void _RTEMS_API_Initialize(
  rtems_configuration_table *configuration_table
);

#endif
/* end of include file */
