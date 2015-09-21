/**
 * @file rtems/score/object.inl
 */

/*
 *
 *  This include file contains the static inline implementation of all
 *  of the inlined routines in the Object Handler.
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: object.inl,v 1.30 2006/11/17 22:59:41 joel Exp $
 */

#ifndef _RTEMS_SCORE_OBJECT_INL
#define _RTEMS_SCORE_OBJECT_INL

/**
 *  This function builds an object's id from the processor node and index
 *  values specified.
 *
 *  @param[in] the_api indicates the API associated with this Id.
 *  @param[in] the_class indicates the class of object.
 *             It is specific to @a the_api.
 *  @param[in] node is the node where this object resides.
 *  @param[in] index is the instance number of this object.
 *
 *  @return This method returns an object Id constructed from the arguments.
 */
RTEMS_INLINE_ROUTINE Objects_Id _Objects_Build_id(
  Objects_APIs     the_api,
  uint32_t         the_class,
  uint32_t         node,
  uint32_t         index
)
{
  return (( (Objects_Id) the_api )   << OBJECTS_API_START_BIT)   |
         (( (Objects_Id) the_class ) << OBJECTS_CLASS_START_BIT) |
         (( (Objects_Id) node )      << OBJECTS_NODE_START_BIT)  |
         (( (Objects_Id) index )     << OBJECTS_INDEX_START_BIT);
}

/**
 *  This function returns the API portion of the ID.
 *
 *  @param[in] id is the object Id to be processed.
 *
 *  @return This method returns an object Id constructed from the arguments.
 */
RTEMS_INLINE_ROUTINE Objects_APIs _Objects_Get_API(
  Objects_Id id
)
{
  return (Objects_APIs) ((id >> OBJECTS_API_START_BIT) & OBJECTS_API_VALID_BITS);
}

/**
 *  This function returns the class portion of the ID.
 *
 *  @param[in] id is the object Id to be processed
 */
RTEMS_INLINE_ROUTINE uint32_t   _Objects_Get_class(
  Objects_Id id
)
{
  return (uint32_t  ) 
    ((id >> OBJECTS_CLASS_START_BIT) & OBJECTS_CLASS_VALID_BITS);
}
 
/**
 *  This function returns the node portion of the ID.
 *
 *  @param[in] id is the object Id to be processed
 *
 *  @return This method returns the node portion of an object ID.
 */
RTEMS_INLINE_ROUTINE uint32_t   _Objects_Get_node(
  Objects_Id id
)
{
  return (id >> OBJECTS_NODE_START_BIT) & OBJECTS_NODE_VALID_BITS;
}

/**
 *  This function returns the index portion of the ID.
 *
 *  @param[in] id is the Id to be processed
 *
 *  @return This method returns the class portion of the specified object ID.
 */
RTEMS_INLINE_ROUTINE uint32_t   _Objects_Get_index(
  Objects_Id id
)
{
  return (id >> OBJECTS_INDEX_START_BIT) & OBJECTS_INDEX_VALID_BITS;
}

/**
 *  This function returns TRUE if the class is valid.
 *
 *  @param[in] the_class is the class portion of an object ID.
 *
 *  @return This method returns TRUE if the specified class value is valid
 *          and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE boolean _Objects_Is_class_valid(
  uint32_t   the_class
)
{
  /* XXX how do we determine this now? */
  return TRUE; /* the_class && the_class <= OBJECTS_CLASSES_LAST; */
}

/**
 *  This function returns TRUE if the node is of the local object, and
 *  FALSE otherwise.
 *
 *  @param[in] node is the node number and corresponds to the node number
 *         portion of an object ID.
 *
 *  @return This method returns TRUE if the specified node is the local node
 *          and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE boolean _Objects_Is_local_node(
  uint32_t   node
)
{
  return ( node == _Objects_Local_node );
}

/**
 *  This function returns TRUE if the id is of a local object, and
 *  FALSE otherwise.
 *
 *  @param[in] id is an object ID
 *
 *  @return This method returns TRUE if the specified object Id is local
 *          and FALSE otherwise.
 *
 *  @note On a single processor configuration, this always returns TRUE.
 */
RTEMS_INLINE_ROUTINE boolean _Objects_Is_local_id(
  Objects_Id id
)
{
#if defined(RTEMS_MULTIPROCESSING)
  return _Objects_Is_local_node( _Objects_Get_node(id) );
#else
  return TRUE;
#endif
}

/**
 *  This function returns TRUE if left and right are equal,
 *  and FALSE otherwise.
 *
 *  @param[in] left is the Id on the left hand side of the comparison
 *  @param[in] right is the Id on the right hand side of the comparison
 *
 *  @return This method returns TRUE if the specified object IDs are equal
 *          and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE boolean _Objects_Are_ids_equal(
  Objects_Id left,
  Objects_Id right
)
{
  return ( left == right );
}

/**
 *  This function returns a pointer to the local_table object
 *  referenced by the index.
 *
 *  @param[in] information points to an Object Information Table
 *  @param[in] index is the index of the object the caller wants to access
 *
 *  @return This method returns a pointer to a local object or NULL if the
 *          index is invalid.
 */
RTEMS_INLINE_ROUTINE Objects_Control *_Objects_Get_local_object(
  Objects_Information *information,
  uint16_t             index
)
{
  if ( index > information->maximum )
    return NULL;
  return information->local_table[ index ];
}

/**
 *  This function sets the pointer to the local_table object
 *  referenced by the index.
 *
 *  @param[in] information points to an Object Information Table
 *  @param[in] index is the index of the object the caller wants to access
 *  @param[in] the_object is the local object pointer
 */

RTEMS_INLINE_ROUTINE void _Objects_Set_local_object(
  Objects_Information *information,
  uint16_t             index,
  Objects_Control     *the_object
)
{
  if ( index <= information->maximum )
    information->local_table[ index ] = the_object;
}

/**
 *  This function return the information structure given
 *  an id of an object.
 *
 *  @param[in] id is an object ID
 *
 *
 *  @return This method returns a pointer to the Object Information Table
 *          for the class of objects which corresponds to this object ID.
 */
RTEMS_INLINE_ROUTINE Objects_Information *_Objects_Get_information(
  Objects_Id  id
)
{
  Objects_APIs  the_api;
  uint16_t      the_class;


  the_class = _Objects_Get_class( id );

  if ( !_Objects_Is_class_valid( the_class ) )
    return NULL;

  the_api = _Objects_Get_API( id );
  return _Objects_Information_table[ the_api ][ the_class ];
}

/**
 *  This function places the_object control pointer and object name
 *  in the Local Pointer and Local Name Tables, respectively.
 *
 *  @param[in] information points to an Object Information Table
 *  @param[in] the_object is a pointer to an object
 *  @param[in] name is the name of the object to make accessible
 */
RTEMS_INLINE_ROUTINE void _Objects_Open(
  Objects_Information *information,
  Objects_Control     *the_object,
  Objects_Name         name
)
{
  uint32_t    index;

  index = _Objects_Get_index( the_object->id );
  _Objects_Set_local_object( information, index, the_object );

  if ( information->is_string ) 
    /* _Objects_Copy_name_string( name, the_object->name ); */
    the_object->name = name;
  else
    /* _Objects_Copy_name_raw( name, the_object->name, information->name_length ); */
    the_object->name = name;
}

/**
 *  This function removes the_object control pointer and object name
 *  in the Local Pointer and Local Name Tables.
 *
 *  @param[in] information points to an Object Information Table
 *  @param[in] the_object is a pointer to an object
 */
RTEMS_INLINE_ROUTINE void _Objects_Close(
  Objects_Information  *information,
  Objects_Control      *the_object
)
{
  uint32_t   index;

  index = _Objects_Get_index( the_object->id );
  _Objects_Set_local_object( information, index, NULL );
  /* _Objects_Clear_name( the_object->name, information->name_length ); */
  the_object->name = 0;
}

/**
 *  This function removes the_object from the namespace.
 *
 *  @param[in] information points to an Object Information Table
 *  @param[in] the_object is a pointer to an object
 */
RTEMS_INLINE_ROUTINE void _Objects_Namespace_remove(
  Objects_Information  *information,
  Objects_Control      *the_object
)
{
  /* _Objects_Clear_name( the_object->name, information->name_length ); */
  the_object->name = 0;
}

#endif
/* end of include file */
