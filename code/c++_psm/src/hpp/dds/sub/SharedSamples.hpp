#ifndef OMG_DDS_SUB_SHARED_SAMPLES_HPP_
#define OMG_DDS_SUB_SHARED_SAMPLES_HPP__


#include <dds/core/Reference.hpp>
#include <dds/sub/Sample.hpp>
#include <dds/sub/LoanedSamples.hpp>
#include <dds/sub/detail/SharedSamples.hpp>

namespace dds {
  namespace sub {
    template <typename T,
    template <typename Q> class DELEGATE = detail::SharedSamples>
    class SharedSamples;
  }
}

/**
 * The class <code>SharedSamples</code> is used as a container safe
 * and sharable version of the <code>LoanedSamples</code> class.
 *
 *
 */
template <typename T,
template <typename Q> class DELEGATE>
class dds::sub::SharedSamples : public dds::core::Reference< DELEGATE<T> >
{
public:
  typedef T                     DataType;
  typedef typename DELEGATE<T>::iterator              iterator;
  typedef typename DELEGATE<T>::const_iterator        const_iterator;



public:
  /**
   * Constructs an instance of <code>SharedSamples</code> and
   * removes the ownership of the loan from the <code>LoanedSamples</code>.
   * As a result the destruction of the <code>LoanedSamples</code> object
   * or the explicit invocation of its method return_loan will have no effect
   * on loaned data. Loaned data will be returned automatically once
   * the delegate of this reference type will have a zero reference count.
   *
   * @param ls the loaned samples.
   *
   */
  SharedSamples(LoanedSamples ls);

  ~SharedSamples();


public:
  const_iterator begin() const;

  const_iterator  end() const;

  const DELEGATE_REF_T& delegate() const;

  DELEGATE_REF_T& delegate();

  uint32_t length() const;

};

#endif /* OMG_DDS_SUB_SHARED_SAMPLES_HPP_ */
