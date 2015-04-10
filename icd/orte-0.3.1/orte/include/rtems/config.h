/**
 * @file rtems/config.h
 */
 
/*
 *  This include file contains the table of user defined configuration
 *  parameters.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: config.h,v 1.31.2.1 2008/05/06 15:16:26 joel Exp $
 */

#ifndef _RTEMS_CONFIG_H
#define _RTEMS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Unlimited object support. Changes the configuration table entry for POSIX
 *  or RTEMS APIs to bounded only by the memory of the work-space.
 *
 *  Use the macro to define the resource unlimited before placing in
 *  the configuration table.
 */

#include <rtems/score/object.h>
#define RTEMS_UNLIMITED_OBJECTS OBJECTS_UNLIMITED_OBJECTS

#define rtems_resource_unlimited(resource) \
  ( resource | RTEMS_UNLIMITED_OBJECTS )

/*
 *  This is kind of kludgy but it allows targets to totally ignore the
 *  optional APIs like POSIX and ITRON safely.
 */

#ifdef RTEMS_POSIX_API
#include <rtems/posix/config.h>
#else
typedef void *posix_api_configuration_table;
#endif

#ifdef RTEMS_ITRON_API
#include <rtems/itron.h>
#include <rtems/itron/config.h>
#else
typedef void *itron_api_configuration_table;
#endif

#include <rtems/rtems/config.h>

/*  XXX <rtems/rtems/config.h> should cover these
#include <rtems/rtems/status.h>
#include <rtems/rtems/types.h>
#include <rtems/rtems/tasks.h>
*/

#include <rtems/extension.h>
#include <rtems/io.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#endif

/*
 *  The following records define the Multiprocessor Configuration
 *  Table.  This table defines the multiprocessor system
 *  characteristics which must be known by RTEMS in a multiprocessor
 *  system.
 */

typedef struct {
  uint32_t      node;             /* local node number */
  uint32_t      maximum_nodes;    /* maximum # nodes in system */
  uint32_t      maximum_global_objects; /* maximum # global objects */
  uint32_t      maximum_proxies;  /* maximum # proxies */
#if defined(RTEMS_MULTIPROCESSING)
  rtems_mpci_table   *User_mpci_table;  /* pointer to MPCI table */
#else
  void         *User_mpci_table;        /* pointer to MPCI table */
#endif
} rtems_multiprocessing_table;

/*
 *  The following records define the Configuration Table.  The
 *  information contained in this table is required in all
 *  RTEMS systems, whether single or multiprocessor.  This
 *  table primarily defines the following:
 *
 *     + location and size of the RTEMS Workspace
 *     + microseconds per clock tick
 *     + clock ticks per task timeslice
 *     + required number of each object type for each API configured
 */

typedef struct {
  void                          *work_space_start;
  uint32_t                       work_space_size;
  uint32_t                       maximum_extensions;
  uint32_t                       microseconds_per_tick;
  uint32_t                       ticks_per_timeslice;
  uint32_t                       maximum_drivers;
  uint32_t                       number_of_device_drivers;
  rtems_driver_address_table    *Device_driver_table;
  uint32_t                       number_of_initial_extensions;
  rtems_extensions_table        *User_extension_table;
  rtems_multiprocessing_table   *User_multiprocessing_table;
  rtems_api_configuration_table *RTEMS_api_configuration;
  posix_api_configuration_table *POSIX_api_configuration;
  itron_api_configuration_table *ITRON_api_configuration;
} rtems_configuration_table;

/*
 *  The following are provided strictly for the convenience of
 *  the user.  They are not used in RTEMS itself.
 */

SAPI_EXTERN rtems_configuration_table    *_Configuration_Table;
#if defined(RTEMS_MULTIPROCESSING)
  SAPI_EXTERN rtems_multiprocessing_table  *_Configuration_MP_table;
#endif

/*
 *  Some handy macros to avoid dependencies on either the BSP
 *  or the exact format of the configuration table.
 */

#define rtems_configuration_get_table() \
        (&_Configuration_Table)

#define rtems_configuration_get_work_space_start() \
        (_Configuration_Table->work_space_start)

#define rtems_configuration_get_work_space_size() \
        (_Configuration_Table->work_space_size)

#define rtems_configuration_get_maximum_extensions() \
        (_Configuration_Table->maximum_extensions)

#define rtems_configuration_get_microseconds_per_tick() \
        (_Configuration_Table->microseconds_per_tick)
#define rtems_configuration_get_milliseconds_per_tick() \
        (_Configuration_Table->microseconds_per_tick / 1000)

#define rtems_configuration_get_ticks_per_timeslice() \
        (_Configuration_Table->ticks_per_timeslice)

#define rtems_configuration_get_number_of_device_drivers() \
        (_Configuration_Table->number_of_device_drivers)

#define rtems_configuration_get_device_driver_table() \
        (_Configuration_Table->device_driver_table)

#define rtems_configuration_get_number_of_initial_extensions() \
        (_Configuration_Table->number_of_initial_extensions)

#define rtems_configuration_get_user_extension_table() \
        (_Configuration_Table->user_extension_table)

#define rtems_configuration_get_user_multiprocessing_table() \
        (_Configuration_Table->User_multiprocessing_table)

#define rtems_configuration_get_rtems_api_configuration() \
        (_Configuration_Table->RTEMS_api_configuration)

#define rtems_configuration_get_posix_api_configuration() \
        (_Configuration_Table->POSIX_api_configuration)

#define rtems_configuration_get_itron_api_configuration() \
        (_Configuration_Table->ITRON_api_configuration)

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
