#ifndef ORG_LSST_SAL_SALRESPONSE_UTIL_HPP_
#define ORG_LSST_SAL_SALRESPONSE_UTIL_HPP_

#include <iostream>
// DDS Include
#include <dds/dds.hpp>

// Generated Files
#include <org/opensplice/core/config.hpp>
#include "gen/ccpp_SALTopic_response.h"

REGISTER_TOPIC_TRAITS(org::lsst::sal::SALTopic_responseType)

using namespace org::lsst::sal;

using namespace dds::core;
using namespace dds::core::policy;
using namespace dds::core::cond;
using namespace dds::core::status;

using namespace dds::domain;

using namespace dds::topic;

using namespace dds::pub;
using namespace dds::pub::qos;

using namespace dds::sub;
using namespace dds::sub::status;

std::ostream&
operator <<(std::ostream& os, const org::lsst::sal::SALTopic_responseType& s);

std::ostream&
operator <<(std::ostream& os, const dds::sub::SampleInfo& si);

namespace sal {
   namespace salresponse {
      struct Params;

      void printSALTopic_response(const org::lsst::sal::SALTopic_responseType& s);
      void printSALTopic_responseSample(const dds::sub::Sample<org::lsst::sal::SALTopic_responseType>& s);

      template<typename T>
      class Runner {
      public:
         virtual ~Runner() {
         }
         virtual void run(const dds::domain::DomainParticipant& dp,
               const dds::topic::Topic<T>& topic,
               const Params& params) = 0;
      };

      template<typename T>
      class IdleRunner: public Runner<T> {
      public:
         virtual ~IdleRunner() {
         }
      public:
         virtual void run(const dds::domain::DomainParticipant& dp,
               const dds::topic::Topic<T>& topic,
               const Params& params) {

         }
      };

      typedef boost::shared_ptr<sal::salresponse::Runner<SALTopic_responseType> > Runner_t;

      struct Params {
        Params() :
          name("test"),
          status("DONE"),
          timeout(0),
          repeat(0),
          submits(0),
          period(1000),
          samples(10),
          history_depth(1),
          data_state(dds::sub::status::DataState::new_data())
        { }
          std::string name;
          std::string status;
          uint32_t timeout;
          uint32_t repeats;
          uint32_t submits;
          uint32_t period;
          uint32_t samples;
          uint32_t history_depth;
          dds::sub::status::DataState data_state;
       };

       struct Config {
          Params params;
          Runner_t runner;
       };

   }
} /** sal :: salresponse */


#endif /* ORG_LSST_SAL_SALRESPONSE_UTIL_HPP_ */