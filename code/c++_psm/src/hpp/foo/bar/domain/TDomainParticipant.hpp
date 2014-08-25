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
#ifndef OSPL_DDS_DOMAIN_TDOMAINPARTICIPANT_HPP_
#define OSPL_DDS_DOMAIN_TDOMAINPARTICIPANT_HPP_

/**
 * @file dds/domain/TDomainParticipant.hpp
 */

/*
 * OMG PSM class declaration
 */
#include <spec/dds/domain/TDomainParticipant.hpp>

// Implementation

namespace dds
{
namespace domain
{

template <typename DELEGATE>
TDomainParticipant<DELEGATE>::TDomainParticipant(uint32_t did): ::dds::core::TEntity<DELEGATE>(new DELEGATE(did)) { }

template <typename DELEGATE>
TDomainParticipant<DELEGATE>::TDomainParticipant(uint32_t id,
        const dds::domain::qos::DomainParticipantQos& qos,
        dds::domain::DomainParticipantListener* listener,
        const dds::core::status::StatusMask& mask) :
    ::dds::core::TEntity<DELEGATE>(new DELEGATE(id, qos, listener, mask)) { }

template <typename DELEGATE>
TDomainParticipant<DELEGATE>::~TDomainParticipant() { }

template <typename DELEGATE>
void TDomainParticipant<DELEGATE>::listener(Listener* the_listener,
        const ::dds::core::status::StatusMask& event_mask)
{
    this->delegate()->listener(the_listener, event_mask);
}

template <typename DELEGATE>
typename TDomainParticipant<DELEGATE>::Listener*  TDomainParticipant<DELEGATE>::listener() const
{
    return this->delegate()->listener();
}

template <typename DELEGATE>
const dds::domain::qos::DomainParticipantQos&
TDomainParticipant<DELEGATE>::qos() const
{
    return this->delegate()->qos();
}

template <typename DELEGATE>
void TDomainParticipant<DELEGATE>::qos(const dds::domain::qos::DomainParticipantQos& the_qos)
{
    this->delegate()->qos(the_qos);
}

template <typename DELEGATE>
uint32_t TDomainParticipant<DELEGATE>::domain_id() const
{
    return this->delegate()->domain_id();
}

template <typename DELEGATE>
void TDomainParticipant<DELEGATE>::assert_liveliness()
{
    this->delegate()->assert_liveliness();
}

template <typename DELEGATE>
bool TDomainParticipant<DELEGATE>::contains_entity(const ::dds::core::InstanceHandle& handle)
{
    return this->delegate()->contains_entity();
}

template <typename DELEGATE>
dds::core::Time TDomainParticipant<DELEGATE>::current_time()
{
    return this->delegate()->current_time();
}

template <typename DELEGATE>
dds::domain::qos::DomainParticipantQos TDomainParticipant<DELEGATE>::default_participant_qos()
{
    return DELEGATE::default_participant_qos();
}

template <typename DELEGATE>
void TDomainParticipant<DELEGATE>::default_participant_qos(const ::dds::domain::qos::DomainParticipantQos& qos)
{
    DELEGATE::default_participant_qos(qos);
}

template <typename DELEGATE>
dds::pub::qos::PublisherQos  TDomainParticipant<DELEGATE>::default_publisher_qos() const
{
    return this->delegate()->default_publisher_qos();
}

template <typename DELEGATE>
TDomainParticipant<DELEGATE>& TDomainParticipant<DELEGATE>::default_publisher_qos(
    const ::dds::pub::qos::PublisherQos& qos)
{
    this->delegate()->default_publisher_qos(qos);
    return *this;
}

template <typename DELEGATE>
dds::sub::qos::SubscriberQos  TDomainParticipant<DELEGATE>::default_subscriber_qos() const
{
    return this->delegate()->default_subscriber_qos();
}

template <typename DELEGATE>
TDomainParticipant<DELEGATE>& TDomainParticipant<DELEGATE>::default_subscriber_qos(
    const ::dds::sub::qos::SubscriberQos& qos)
{
    this->delegate()->default_subscriber_qos(qos);
    return *this;
}

template <typename DELEGATE>
dds::topic::qos::TopicQos  TDomainParticipant<DELEGATE>::default_topic_qos() const
{
    return this->delegate()->default_topic_qos();
}

template <typename DELEGATE>
TDomainParticipant<DELEGATE>&  TDomainParticipant<DELEGATE>::default_topic_qos(const dds::topic::qos::TopicQos& qos)
{
    this->delegate()->default_topic_qos(qos);
    return *this;
}
}
}
// End of implementation

#endif /* OSPL_DDS_DOMAIN_TDOMAINPARTICIPANT_HPP_ */