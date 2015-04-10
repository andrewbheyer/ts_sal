/** 
 *  @file  rtems/score/wkspace.inl
 *
 *  This include file contains the bodies of the routines which contains
 *  information related to the RAM Workspace.
 */

/*
 *  COPYRIGHT (c) 1989-2004.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: wkspace.inl,v 1.19 2007/03/16 07:19:24 ralf Exp $
 */

#ifndef _RTEMS_SCORE_WKSPACE_INL
#define _RTEMS_SCORE_WKSPACE_INL

/**
 *  @addtogroup ScoreWorkspace 
 *  @{
 */

/**
 *  This routine returns the address of a block of memory of size
 *  bytes.  If a block of the appropriate size cannot be allocated
 *  from the workspace, then NULL is returned.
 */

RTEMS_INLINE_ROUTINE void *_Workspace_Allocate(
  size_t   size
)
{
   return _Heap_Allocate( &_Workspace_Area, size );
}

/**
 *  This function frees the specified block of memory.  If the block
 *  belongs to the Workspace and can be successfully freed, then
 *  TRUE is returned.  Otherwise FALSE is returned.
 */

RTEMS_INLINE_ROUTINE boolean _Workspace_Free(
  void *block
)
{
   return _Heap_Free( &_Workspace_Area, block );
}

/**@}*/

#endif
/* end of include file */
