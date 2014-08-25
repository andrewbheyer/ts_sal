/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class uShortArray {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected uShortArray(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(uShortArray obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        svcSAL_TopicIdJNI.delete_uShortArray(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public uShortArray(int nelements) {
    this(svcSAL_TopicIdJNI.new_uShortArray(nelements), true);
  }

  public int getitem(int index) {
    return svcSAL_TopicIdJNI.uShortArray_getitem(swigCPtr, this, index);
  }

  public void setitem(int index, int value) {
    svcSAL_TopicIdJNI.uShortArray_setitem(swigCPtr, this, index, value);
  }

  public SWIGTYPE_p_unsigned_short cast() {
    long cPtr = svcSAL_TopicIdJNI.uShortArray_cast(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_unsigned_short(cPtr, false);
  }

  public static uShortArray frompointer(SWIGTYPE_p_unsigned_short t) {
    long cPtr = svcSAL_TopicIdJNI.uShortArray_frompointer(SWIGTYPE_p_unsigned_short.getCPtr(t));
    return (cPtr == 0) ? null : new uShortArray(cPtr, false);
  }

}