// ============================================================================
//
// = CONTEXT
//    SALLV Generic Client for Labview
//
// = FILENAME
//    SharedObject.i
//
// = AUTHOR
//    Nicolas Leclercq
//
// ============================================================================

// ============================================================================
// SharedObject::lock
// ============================================================================
LV_INLINE void SharedObject::lock (void)
{
  this->lock_.acquire();
}

// ============================================================================
// SharedObject::unlock
// ============================================================================
LV_INLINE void SharedObject::unlock (void)
{
  this->lock_.release();
}



