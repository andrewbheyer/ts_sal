/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2012 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE
 *
 *   for full copyright notice and license terms.
 *
 */
#ifndef OSPL_DDS_CORE_COND_TGUARDCONDITION_HPP_
#define OSPL_DDS_CORE_COND_TGUARDCONDITION_HPP_

/**
 * @file dds/core/cond/TGuardCondition.hpp
 */

/*
 * OMG PSM class declaration
 */
#include <spec/dds/core/cond/TGuardCondition.hpp>

// Implementation
namespace dds
{
namespace core
{
namespace cond
{

template <typename DELEGATE>
TGuardCondition<DELEGATE>::TGuardCondition() { }

template <typename DELEGATE>
TGuardCondition<DELEGATE>::~TGuardCondition() { }

template <typename DELEGATE>
template <typename Functor>
void TGuardCondition<DELEGATE>::handler(const Functor& func)
{
    this->delegate()->set_handler(func);
}

template <typename DELEGATE>
void TGuardCondition<DELEGATE>::reset_handler()
{
    this->delegate()->reset_handler();
}


template <typename DELEGATE>
void TGuardCondition<DELEGATE>::trigger_value(bool value)
{
    this->delegate()->trigger_value(value);
}


}
}
}
// End of implementation

#endif /* OSPL_DDS_CORE_COND_TGUARDCONDITION_HPP_ */
