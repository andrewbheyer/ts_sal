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
#ifndef OSPL_DDS_TOPIC_DETAIL_TOPIC_HPP_
#define OSPL_DDS_TOPIC_DETAIL_TOPIC_HPP_

/**
 * @file dds/topic/detail/Topic.hpp
 */

// Implementation

#include <dds/core/ref_traits.hpp>
#include <dds/topic/TopicTraits.hpp>
#include <dds/core/status/Status.hpp>
#include <dds/core/status/State.hpp>
#include <dds/domain/DomainParticipant.hpp>
#include <dds/topic/qos/TopicQos.hpp>
#include <dds/topic/detail/TopicDescription.hpp>

#include <org/opensplice/core/config.hpp>
#include <org/opensplice/topic/TopicTraits.hpp>
#include <org/opensplice/topic/qos/QosConverter.hpp>
#include <org/opensplice/core/exception_helper.hpp>

namespace dds
{
namespace topic
{
template <typename T>
class TopicListener;
}
}

namespace dds
{
namespace topic
{
namespace detail
{
template <typename T>
class Topic;
}
}
}

template <typename T>
class dds::topic::detail::Topic : public dds::topic::detail::TopicDescription<T>
{
public:

    Topic(const dds::domain::DomainParticipant& dp,
          const std::string& name,
          const std::string& type_name,
          const dds::topic::qos::TopicQos& qos,
          dds::topic::TopicListener<T>* listener,
          const dds::core::status::StatusMask& mask)
        : dds::topic::detail::TopicDescription<T>(dp, name, type_name),
          dp_(dp),
          qos_(qos),
          listener_(listener),
          mask_(mask)
    {
        char* tn = ts_.get_type_name();
        ts_.register_type(dp->dp_.get(), tn);

        DDS::TopicQos tqos = convertQos(qos);
        t_ = dp->dp_->create_topic(name.c_str(), tn, tqos, 0,
                                   mask.to_ulong());

        if (t_ == 0)
            throw dds::core::NullReferenceError(
                        org::opensplice::core::exception_helper(
                            OSPL_CONTEXT_LITERAL(
                                "dds::core::NullReferenceError : Unable to create Topic! "
                                "Nil return from ::create_topic")));
    }

    virtual ~Topic()
    {
        DDS::ReturnCode_t result = dp_->dp_->delete_topic(t_);
        org::opensplice::core::check_and_throw(result, OSPL_CONTEXT_LITERAL("Calling ::delete_topic"));
    }



public:
    const dds::topic::qos::TopicQos& qos() const
    {
        return qos_;
    }

    void qos(const dds::topic::qos::TopicQos& the_qos)
    {
        qos_ = the_qos;
    }

    const ::dds::core::status::InconsistentTopicStatus& inconsistent_topic_status()
    {
        return its_;
    }

private:
    dds::domain::DomainParticipant dp_;
    dds::topic::qos::TopicQos qos_;
    dds::topic::TopicListener<T>* listener_;
    const dds::core::status::StatusMask& mask_;
    ::dds::core::status::InconsistentTopicStatus its_;

    typename dds::topic::topic_type_support<T>::type ts_;

public:
    DDS::Topic* t_;

};

// End of implementation

#endif /* OSPL_DDS_TOPIC_DETAIL_TOPIC_HPP_ */
