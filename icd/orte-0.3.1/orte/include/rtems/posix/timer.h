/**
 * @file rtems/posix/timer.h
 */

/*
 *  $Id: timer.h,v 1.13 2007/04/02 21:51:52 joel Exp $
 */

#ifndef _RTEMS_POSIX_TIMER_H
#define _RTEMS_POSIX_TIMER_H

#include <rtems/posix/config.h>

/* Timer is free */
#define POSIX_TIMER_STATE_FREE        0x01
                                                                
/* Created timer but not running          */
#define POSIX_TIMER_STATE_CREATE_NEW  0x02

/* Created timer and running              */
#define POSIX_TIMER_STATE_CREATE_RUN  0x03

/* Created, ran and stopped timer         */
#define POSIX_TIMER_STATE_CREATE_STOP 0x04

/* Indicates that the fire time is relative to the current one */
#define POSIX_TIMER_RELATIVE       0

/*
 * POSIX defines TIMER_ABSTIME but no constant for relative.  So
 * we have one internally but we need to be careful it has a different
 * value.
 */
#if (POSIX_TIMER_RELATIVE == TIMER_ABSTIME)
#error "POSIX_TIMER_RELATIVE == TIMER_ABSTIME"
#endif


/*
 * Data for a timer
 */
typedef struct {
  Objects_Control   Object;
  Watchdog_Control  Timer;      /* Internal Timer                        */
  pthread_t         thread_id;  /* Thread identifier                     */
  char              state;      /* State of the timer                    */
  struct sigevent   inf;        /* Information associated to the timer   */
  struct itimerspec timer_data; /* Timing data of the timer              */
  uint32_t          ticks;      /* Number of ticks of the initialization */
  uint32_t          overrun;    /* Number of expirations of the timer    */
  struct timespec   time;       /* Time in which the timer was started   */
} POSIX_Timer_Control;

/*
 *  _POSIX_Timers_Manager_initialization
 *
 *  DESCRIPTION:
 *
 *  This routine performs the initialization necessary for this manager.
 */
void _POSIX_Timer_Manager_initialization ( int max_timers );

/*
 *  The following defines the information control block used to manage
 *  this class of objects.
 */
POSIX_EXTERN Objects_Information  _POSIX_Timer_Information;

#ifndef __RTEMS_APPLICATION__
#include <rtems/posix/timer.inl>
#endif

#endif
/* end of include file */
