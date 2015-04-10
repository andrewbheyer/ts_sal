/**
 * @file rtems/bdbuf.h
 *
 * block device buffer management
 */
 
/*
 * Copyright (C) 2001 OKTET Ltd., St.-Petersburg, Russia
 * Author: Victor V. Vengerov <vvv@oktet.ru>
 *
 * @(#) bdbuf.h,v 1.9 2005/02/02 00:06:18 joel Exp
 */

#ifndef _RTEMS_BDBUF_H
#define _RTEMS_BDBUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/chain.h>

#include "rtems/blkdev.h"
#include "rtems/diskdevs.h"


/*
 * To manage buffers we using Buffer Descriptors.
 * To speed-up buffer lookup descriptors are organized in AVL-Tree.
 * The fields 'dev' and 'block' are search key.
 */

/* Buffer descriptors
 * Descriptors organized in AVL-tree to speedup buffer lookup.
 * dev and block fields are search key in AVL-tree.
 * Modified buffers, free buffers and used buffers linked in 'mod', 'free' and
 * 'lru' chains appropriately.
 */

typedef struct bdbuf_buffer {
    Chain_Node link; /* Link in the lru, mod or free chains */

    struct bdbuf_avl_node {
    	signed char cache;           /* Cache */

	    struct bdbuf_buffer* left;  /* Left Child */
	    struct bdbuf_buffer* right; /* Right Child */

    	signed char bal;             /* The balance of the sub-tree */
    } avl;

    dev_t       dev;     /* device number */
    blkdev_bnum block;   /* block number on the device */

    unsigned char    *buffer;  /* Pointer to the buffer memory area */
    rtems_status_code status; /* Last I/O operation completion status */
    int         error;   /* If status != RTEMS_SUCCESSFUL, this field contains
                            errno value which can be used by user later */
    boolean     modified:1;    /* =1 if buffer was modified */
    boolean     in_progress:1; /* =1 if exchange with disk is in progress;
                                  need to wait on semaphore */
    boolean     actual:1;      /* Buffer contains actual data */
    int         use_count; /* Usage counter; incremented when somebody use
                              this buffer; decremented when buffer released
                              without modification or when buffer is flushed
                              by swapout task */

    rtems_bdpool_id pool;  /* Identifier of buffer pool to which this buffer
                              belongs */
    CORE_mutex_Control transfer_sema;
                           /* Transfer operation semaphore */
} bdbuf_buffer;


/*
 * the following data structures are internal to the bdbuf layer,
 * but it is convenient to have them visible from the outside for inspection
 */
/*
 * The groups of the blocks with the same size are collected in the
 * bd_pool. Note that a several of the buffer's groups with the
 * same size can exists.
 */
typedef struct bdbuf_pool
{
    bdbuf_buffer *tree;         /* Buffer descriptor lookup AVL tree root */

    Chain_Control free;         /* Free buffers list */
    Chain_Control lru;          /* Last recently used list */

    int           blksize;      /* The size of the blocks (in bytes) */
    int           nblks;        /* Number of blocks in this pool */
    rtems_id      bufget_sema;  /* Buffer obtain counting semaphore */
    void         *mallocd_bufs; /* Pointer to the malloc'd buffer memory,
                                   or NULL, if buffer memory provided in
                                   buffer configuration */
    bdbuf_buffer *bdbufs;       /* Pointer to table of buffer descriptors
                                   allocated for this buffer pool. */
} bdbuf_pool;

/* Buffering layer context definition */
struct bdbuf_context {
    bdbuf_pool    *pool;         /* Table of buffer pools */
    int            npools;       /* Number of entries in pool table */

    Chain_Control  mod;          /* Modified buffers list */
    rtems_id       flush_sema;   /* Buffer flush semaphore; counting
                                    semaphore; incremented when buffer
                                    flushed to the disk; decremented when
                                    buffer modified */
    rtems_id       swapout_task; /* Swapout task ID */
};
  /*
   * the context of the buffering layer, visible for inspection 
   */
extern struct bdbuf_context rtems_bdbuf_ctx;

/* bdbuf_config structure describes block configuration (size,
 * amount, memory location) for buffering layer
 */
typedef struct rtems_bdbuf_config {
    int      size;      /* Size of block */
    int      num;       /* Number of blocks of appropriate size */
    unsigned char  *mem_area;
                        /* Pointer to the blocks location or NULL, in this
                           case memory for blocks will be allocated by
                           Buffering Layer with the help of RTEMS partition
                           manager */
} rtems_bdbuf_config;

extern rtems_bdbuf_config rtems_bdbuf_configuration[];
extern int rtems_bdbuf_configuration_size;

#define SWAPOUT_TASK_DEFAULT_PRIORITY 15
extern rtems_task_priority swapout_task_priority;

/* rtems_bdbuf_init --
 *     Prepare buffering layer to work - initialize buffer descritors
 *     and (if it is neccessary) buffers. Buffers will be allocated accoriding
 *     to the configuration table, each entry describes kind of block and
 *     amount requested. After initialization all blocks is placed into
 *     free elements lists.
 *
 * PARAMETERS:
 *     conf_table - pointer to the buffers configuration table
 *     size       - number of entries in configuration table
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_init(rtems_bdbuf_config *conf_table, int size);


/* rtems_bdbuf_get --
 *     Obtain block buffer. If specified block already cached (i.e. there's
 *     block in the _modified_, or _recently_used_), return address
 *     of appropriate buffer descriptor and increment reference counter to 1.
 *     If block is not cached, allocate new buffer and return it. Data
 *     shouldn't be read to the buffer from media; buffer may contains
 *     arbitrary data. This primitive may be blocked if there are no free
 *     buffer descriptors available and there are no unused non-modified
 *     (or synchronized with media) buffers available.
 *
 * PARAMETERS:
 *     device - device number (constructed of major and minor device number)
 *     block  - linear media block number
 *     bd     - address of variable to store pointer to the buffer descriptor
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 *
 * SIDE EFFECTS:
 *     bufget_sema semaphore obtained by this primitive.
 */
rtems_status_code
rtems_bdbuf_get(dev_t device, blkdev_bnum block, bdbuf_buffer **bdb_ptr);

/* rtems_bdbuf_read --
 *     (Similar to the rtems_bdbuf_get, except reading data from media)
 *     Obtain block buffer. If specified block already cached, return address
 *     of appropriate buffer and increment reference counter to 1. If block is
 *     not cached, allocate new buffer and read data to it from the media.
 *     This primitive may be blocked on waiting until data to be read from
 *     media, if there are no free buffer descriptors available and there are
 *     no unused non-modified (or synchronized with media) buffers available.
 *
 * PARAMETERS:
 *     device - device number (consists of major and minor device number)
 *     block  - linear media block number
 *     bd     - address of variable to store pointer to the buffer descriptor
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 *
 * SIDE EFFECTS:
 *     bufget_sema and transfer_sema semaphores obtained by this primitive.
 */
rtems_status_code
rtems_bdbuf_read(dev_t device, blkdev_bnum block, bdbuf_buffer **bdb_ptr);

/* rtems_bdbuf_release --
 *     Release buffer allocated before. This primitive decrease the
 *     usage counter. If it is zero, further destiny of buffer depends on
 *     'modified' status. If buffer was modified, it is placed to the end of
 *     mod list and flush task waken up. If buffer was not modified,
 *     it is placed to the end of lru list, and bufget_sema released, allowing
 *     to reuse this buffer.
 *
 * PARAMETERS:
 *     bd_buf - pointer to the bdbuf_buffer structure previously obtained using
 *              get/read primitive.
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 *
 * SIDE EFFECTS:
 *     flush_sema and bufget_sema semaphores may be released by this primitive.
 */
rtems_status_code
rtems_bdbuf_release(bdbuf_buffer *bd_buf);

/* rtems_bdbuf_release_modified --
 *     Release buffer allocated before, assuming that it is _modified_ by
 *     it's owner. This primitive decrease usage counter for buffer, mark
 *     buffer descriptor as modified. If usage counter is 0, insert it at
 *     end of mod chain and release flush_sema semaphore to activate the
 *     flush task.
 *
 * PARAMETERS:
 *     bd_buf - pointer to the bdbuf_buffer structure previously obtained using
 *              get/read primitive.
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 *
 * SIDE EFFECTS:
 *     flush_sema semaphore may be released by this primitive.
 */
rtems_status_code
rtems_bdbuf_release_modified(bdbuf_buffer *bd_buf);

/* rtems_bdbuf_sync --
 *     Wait until specified buffer synchronized with disk. Invoked on exchanges
 *     critical for data consistency on the media. This primitive mark owned
 *     block as modified, decrease usage counter. If usage counter is 0,
 *     block inserted to the mod chain and flush_sema semaphore released.
 *     Finally, primitives blocked on transfer_sema semaphore.
 *
 * PARAMETERS:
 *     bd_buf - pointer to the bdbuf_buffer structure previously obtained using
 *              get/read primitive.
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 *
 * SIDE EFFECTS:
 *     Primitive may be blocked on transfer_sema semaphore.
 */
rtems_status_code
rtems_bdbuf_sync(bdbuf_buffer *bd_buf);

/* rtems_bdbuf_syncdev --
 *     Synchronize with disk all buffers containing the blocks belonging to
 *     specified device.
 *
 * PARAMETERS:
 *     dev - block device number
 *
 * RETURNS:
 *     RTEMS status code (RTEMS_SUCCESSFUL if operation completed successfully
 *     or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_syncdev(dev_t dev);

/* rtems_bdbuf_find_pool --
 *     Find first appropriate buffer pool. This primitive returns the index
 *     of first buffer pool which block size is greater than or equal to
 *     specified size.
 *
 * PARAMETERS:
 *     block_size - requested block size
 *     pool       - placeholder for result
 *
 * RETURNS:
 *     RTEMS status code: RTEMS_SUCCESSFUL if operation completed successfully,
 *     RTEMS_INVALID_SIZE if specified block size is invalid (not a power
 *     of 2), RTEMS_NOT_DEFINED if buffer pool for this or greater block size
 *     is not configured.
 */
rtems_status_code
rtems_bdbuf_find_pool(int block_size, rtems_bdpool_id *pool);

/* rtems_bdbuf_get_pool_info --
 *     Obtain characteristics of buffer pool with specified number.
 *
 * PARAMETERS:
 *     pool       - buffer pool number
 *     block_size - block size for which buffer pool is configured returned
 *                  there
 *     blocks     - number of buffers in buffer pool returned there
 *
 * RETURNS:
 *     RTEMS status code: RTEMS_SUCCESSFUL if operation completed successfully,
 *     RTEMS_INVALID_NUMBER if appropriate buffer pool is not configured.
 *
 * NOTE:
 *     Buffer pools enumerated contiguously starting from 0.
 */
rtems_status_code
rtems_bdbuf_get_pool_info(rtems_bdpool_id pool, int *block_size, int *blocks);

#ifdef __cplusplus
}
#endif

#endif
