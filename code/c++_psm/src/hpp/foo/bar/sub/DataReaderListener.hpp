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
#ifndef OSPL_DDS_SUB_DATAREADERLISTENER_HPP_
#define OSPL_DDS_SUB_DATAREADERLISTENER_HPP_

/**
 * @file dds/sub/DataReaderListener.hpp
 */

/*
 * OMG PSM class declaration
 */
#include <spec/dds/sub/DataReaderListener.hpp>

// Implementation

namespace dds
{
namespace sub
{

template <typename T>
DataReaderListener<T>::~DataReaderListener() { }

template <typename T>
NoOpDataReaderListener<T>::~NoOpDataReaderListener() { }

template <typename T>
void NoOpDataReaderListener<T>::on_requested_deadline_missed(
    DataReader<T>& the_reader,
    const dds::core::status::RequestedDeadlineMissedStatus& status) { }

template <typename T>
void NoOpDataReaderListener<T>::on_requested_incompatible_qos(
    DataReader<T>& the_reader,
    const dds::core::status::RequestedIncompatibleQosStatus& status) { }

template <typename T>
void NoOpDataReaderListener<T>::on_sample_rejected(
    DataReader<T>& the_reader,
    const dds::core::status::SampleRejectedStatus& status) { }

template <typename T>
void NoOpDataReaderListener<T>::on_liveliness_changed(
    DataReader<T>& the_reader,
    const dds::core::status::LivelinessChangedStatus& status) { }

template <typename T>
void NoOpDataReaderListener<T>::on_data_available(DataReader<T>& the_reader) { }

template <typename T>
void NoOpDataReaderListener<T>::on_subscription_matched(
    DataReader<T>& the_reader,
    const dds::core::status::SubscriptionMatchedStatus& status) { }

template <typename T>
void NoOpDataReaderListener<T>::on_sample_lost(
    DataReader<T>& the_reader,
    const dds::core::status::SampleLostStatus& status) { }
}
}

// End of implementation

#endif /* OSPL_DDS_SUB_DATAREADERLISTENER_HPP_ */
