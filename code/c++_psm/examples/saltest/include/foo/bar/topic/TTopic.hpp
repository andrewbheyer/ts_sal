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
#ifndef OSPL_DDS_TOPIC_TTOPIC_HPP_
#define OSPL_DDS_TOPIC_TTOPIC_HPP_

/**
 * @file dds/topic/TTopic.hpp
 */

/*
 * OMG PSM class declaration
 */
#include <spec/dds/topic/TTopic.hpp>

// Implementation

namespace dds
{
namespace topic
{
template <typename T, template <typename Q> class DELEGATE>
Topic<T, DELEGATE>::Topic(const dds::domain::DomainParticipant& dp,
                          const std::string& topic_name)
    : dds::topic::TopicDescription<T, DELEGATE>(new DELEGATE<T>(dp,
            topic_name,
            topic_type_name<T>::value(),
            dp.default_topic_qos(),
            NULL,
            dds::core::status::StatusMask::all()))
{ }

template <typename T, template <typename Q> class DELEGATE>
Topic<T, DELEGATE>::Topic(const dds::domain::DomainParticipant& dp,
                          const std::string& topic_name,
                          const std::string& type_name)
    : dds::topic::TopicDescription<T, DELEGATE>(new DELEGATE<T>(dp,
            topic_name,
            type_name,
            dp.default_topic_qos(),
            NULL,
            dds::core::status::StatusMask::all()))
{ }

template <typename T, template <typename Q> class DELEGATE>
Topic<T, DELEGATE>::Topic(const dds::domain::DomainParticipant& dp,
                          const std::string& topic_name,
                          const dds::topic::qos::TopicQos& qos,
                          dds::topic::TopicListener<T>* listener,
                          const dds::core::status::StatusMask& mask)
    : dds::topic::TopicDescription<T, DELEGATE>(new DELEGATE<T>(dp,
            topic_name,
            topic_type_name<T>::value(),
            qos,
            listener,
            mask))
{ }

template <typename T, template <typename Q> class DELEGATE>
Topic<T, DELEGATE>::Topic(const dds::domain::DomainParticipant& dp,
                          const std::string& topic_name,
                          const std::string& type_name,
                          const dds::topic::qos::TopicQos& qos,
                          dds::topic::TopicListener<T>* listener,
                          const dds::core::status::StatusMask& mask)
    : dds::topic::TopicDescription<T, DELEGATE>(new DELEGATE<T>(dp,
            topic_name,
            type_name,
            qos,
            listener,
            mask))
{ }

template <typename T, template <typename Q> class DELEGATE>
Topic<T, DELEGATE>::~Topic() {}

/** @todo OSPL-1736 OMG_DDS_X_TYPE_DYNAMIC_TYPE_SUPPORT */

/** @todo OSPL-1736 no implementation */
template <typename T, template <typename Q> class DELEGATE>
void Topic<T, DELEGATE>::listener(Listener* the_listener,
                                  const ::dds::core::status::StatusMask& event_mask) { }

/** @todo OSPL-1736 no implementation */
template <typename T, template <typename Q> class DELEGATE>
typename Topic<T, DELEGATE>::Listener* Topic<T, DELEGATE>::listener() const { }

template <typename T, template <typename Q> class DELEGATE>
const dds::topic::qos::TopicQos& Topic<T, DELEGATE>::qos() const
{
    return this->::dds::core::Reference<DELEGATE<T> >::delegate()->qos();
}

template <typename T, template <typename Q> class DELEGATE>
void Topic<T, DELEGATE>::qos(const dds::topic::qos::TopicQos& the_qos)
{
    this->::dds::core::Reference<DELEGATE<T> >::delegate()->qos(the_qos);
}

template <typename T, template <typename Q> class DELEGATE>
const ::dds::core::status::InconsistentTopicStatus&
Topic<T, DELEGATE>::inconsistent_topic_status() const
{
    return this->delegate()->inconsistent_topic_status();
}

}
}

// End of implementation

#endif /* OSPL_DDS_TOPIC_TTOPIC_HPP_ */
