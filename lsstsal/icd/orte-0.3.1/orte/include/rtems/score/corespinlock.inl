/** 
 *  @file  rtems/score/corespinlock.inl
 *
 *  This include file contains all of the inlined routines associated
 *  with the SuperCore spinlock.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: corespinlock.inl,v 1.1 2006/09/25 13:36:58 joel Exp $
 */

#ifndef _RTEMS_SCORE_CORESPINLOCK_INL
#define _RTEMS_SCORE_CORESPINLOCK_INL

/**
 *  @addtogroup ScoreSpinlock 
 *  @{
 */

RTEMS_INLINE_ROUTINE boolean _CORE_spinlock_Is_busy(
  CORE_spinlock_Control  *the_spinlock
)
{
  return (the_spinlock->users != 0);
}


/**@}*/

#endif
/* end of include file */
