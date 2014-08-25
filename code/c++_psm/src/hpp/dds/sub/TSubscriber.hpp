#ifndef OMG_TDDS_SUB_SUBSCRIBER_HPP_
#define OMG_TDDS_SUB_SUBSCRIBER_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dds/core/TEntity.hpp>
#include <dds/core/cond/StatusCondition.hpp>
#include <dds/domain/DomainParticipant.hpp>
#include <dds/sub/qos/DataReaderQos.hpp>

namespace dds { namespace sub {
  template <typename DELEGATE>
  class TSubscriber;

  class SubscriberListener;
} }

template <typename DELEGATE>
class dds::sub::TSubscriber : public dds::core::TEntity<DELEGATE> {
public:
  OMG_DDS_REF_TYPE(TSubscriber, dds::core::TEntity, DELEGATE)

public:
  typedef dds::sub::SubscriberListener                 Listener;

public:
  /**
   * Create a <code>Subscriber</code> attached to the given domain participant.
   * The subscriber QoS will be set to the default as provided by the domain
   * participant.
   *
   * @param dp the domain participant that will own this subscriber.
   */
  TSubscriber(const ::dds::domain::DomainParticipant& dp);

  /**
   * Create a <code>Subscriber</code> attached to the given domain participant.
   *
   * @param dp the domain participant that will own this subscriber.
   * @param qos the subscriber qos
   * @param listenr the subscriber listener.
   * @param mask the listener event mask.
   */
  TSubscriber(const ::dds::domain::DomainParticipant& dp,
      const dds::sub::qos::SubscriberQos& qos,
      dds::sub::SubscriberListener* listener = NULL,
      const dds::core::status::StatusMask& mask = dds::core::status::StatusMask::all());

public:
  ~TSubscriber();

public:
  /**
   * This operation invokes the operation on_data_available on the
   * DataReaderListener objects attached to contained DataReader
   * entities with a DATA_AVAILABLE status that is considered changed
   * as described in Section 7.1.4.2.2, Changes in Read Communication
   * Statuses.
   */
  void notify_datareaders();

  /**
   * Attach a listener to this.
   *
   * @param listener the listener
   * @param event_mask the event mask for the listener.
   */
  void listener(Listener*,
      const dds::core::status::StatusMask& event_mask);

  /**
   * Get the <code>Subscriber</code> listener.
   */
  Listener* listener() const;


  /**
   * Get the <code>Subscriber</code> QoS.
   */
  const dds::sub::qos::SubscriberQos& qos() const;

  /**
   * Set the <code>Subscriber</code> QoS.
   *
   * @param qos the new QoS.
   */
  void qos(const dds::sub::qos::SubscriberQos& the_qos);

  /**
   * Get the default <code>DataReader</code> QoS.
   */
  dds::sub::qos::DataReaderQos default_datareader_qos() const;

  /**
   * Set the default <code>DataReader</code> QoS.
   *
   * @param qos the default <code>DataReader</code> QoS.
   */
  TSubscriber& default_datareader_qos(const dds::sub::qos::DataReaderQos &qos) const;

  /**
   * Return the <code>DomainParticipant<code> that owns this Subscriber.
   */
  const dds::domain::DomainParticipant& participant() const;
};


#endif /* OMG_TDDS_SUB_SUBSCRIBER_HPP_ */
