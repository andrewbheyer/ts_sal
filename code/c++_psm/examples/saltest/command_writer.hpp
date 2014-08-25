#ifndef ORG_LSST_SAL_SALCOMMAND_WRITER_HPP_
#define ORG_LSST_SAL_SALCOMMAND_WRITER_HPP_

#include "command_util.hpp"
#include <unistd.h>
#include <vector>
#include <algorithm>

namespace sal {
   namespace salcommand {
      template <typename T>
      class Writer;
   }
}
/**
 * This class shows a sample use of the (forward) iterator-based read API.
 * Beware that a back-inserting iterator API also exist.
 *
 */
template <typename T>
class sal::salcommand::Writer : public sal::salcommand::Runner<T> {
public:
   virtual ~Writer() { }

public:
   virtual void run(const dds::domain::DomainParticipant& dp,
         const dds::topic::Topic<T>& topic,
         const Params& params)
   {
      dds::pub::qos::PublisherQos pqos =
            dp.default_publisher_qos() << Partition("salcommand");

      dds::pub::Publisher pub(dp, pqos);

      dds::pub::qos::DataWriterQos dwqos =
            pub.default_writer_qos() << Durability::Transient() << Reliability::Reliable();

      dds::pub::DataWriter<T> dw(pub, topic, dwqos);

      const uint32_t period = params.period;
      const uint32_t samples = params.samples;
      uint32_t sleep_time = period * 1000;

      srand(clock());
      uint32_t private_revCode;
      uint32_t private_sndStamp;
      uint32_t private_rcvStamp; 
      uint32_t private_seqNum;
      uint32_t private_origin;
      uint32_t private_cmdID;

      // AnyDataWriter work just fine...
      AnyDataWriter adw = dw;
      DataWriter<SALTopic_commandType> xdw = adw.get<SALTopic_commandType>();
      std::cout << "Topic Name = " << xdw.topic().name()
                      << "\tType Name = " << xdw.topic().type_name() << std::endl;

      // SALTopic_commandType s = {params.private_revCode};
      SALTopic_commandType s = {
	  private_revCode,
	  private_sndStamp,
	  private_rcvStamp,
	  private_seqNum,
	  private_origin,
	  private_cmdID,
	  params.subsystem,
	  params.device,
	  params.property,
	  params.action,
	  params.value,
	  params.modifiers};

      std::cout << ">> Writing Data...";
               std::flush(std::cout);
      for (uint32_t i = 0; i < samples; ++i) {
         // Regular write
         dw.write(s);

         // Stream write
         dw << s;

         usleep(sleep_time); // period is in ms
      }
   }
};


#endif /* ORG_LSSST_SAL_SALCOMMAND_WRITER_HPP_ */
