/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2013 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
package org.lsst.sal;

import org.opensplice.dds.dcps.TypeSupportImpl;
import DDS.*;
import DDS.STATUS_MASK_NONE; 
import DDS.DOMAIN_ID_DEFAULT;
import DDS.DATAREADER_QOS_USE_TOPIC_QOS;
import DDS.DataReader;
import DDS.DataWriter;
import DDS.DataWriterQosHolder;
import DDS.DomainParticipant;
import DDS.DomainParticipantFactory;
import DDS.DurabilityQosPolicyKind;
import DDS.PARTICIPANT_QOS_DEFAULT;
import DDS.Publisher;
import DDS.PublisherQosHolder;
import DDS.ReliabilityQosPolicyKind;
import DDS.Subscriber;
import DDS.SubscriberQosHolder;
import DDS.Topic;
import DDS.TopicQosHolder;
import DDS.DurabilityQosPolicyKind;
import DDS.HANDLE_NIL;
import DDS.ANY_INSTANCE_STATE;
import DDS.ANY_SAMPLE_STATE;
import DDS.ANY_VIEW_STATE;
import DDS.DataReader;
import DDS.LENGTH_UNLIMITED;
import DDS.SampleInfoSeqHolder;
import SALData.SALTopic;
import SALData.SALTopicDataWriter;
import SALData.SALTopicDataWriterHelper;
import SALData.SALTopicDataReader;
import SALData.SALTopicDataReaderHelper;
import SALData.SALTopicTypeSupport;
import SALData.SALTopicSeqHolder;
import SALData.SALCommand;
import SALData.SALCommandDataWriter;
import SALData.SALCommandDataWriterHelper;
import SALData.SALCommandDataReader;
import SALData.SALCommandDataReaderHelper;
import SALData.SALCommandSeqHolder;
import SALData.SALCommandTypeSupport;
import SALData.SALResponse;
import SALData.SALResponseDataWriter;
import SALData.SALResponseDataWriterHelper;
import SALData.SALResponseDataReader;
import SALData.SALResponseDataReaderHelper;
import SALData.SALResponseSeqHolder;
import SALData.SALResponseTypeSupport;
import SALData.SALEvent;
import SALData.SALEventDataWriter;
import SALData.SALEventDataWriterHelper;
import SALData.SALEventDataReader;
import SALData.SALEventDataReaderHelper;
import SALData.SALEventSeqHolder;
import SALData.SALEventTypeSupport;

public class SALDDS {

	private DomainParticipantFactory dpf;
	private DomainParticipant participant;
	private Topic topic;
	private TopicQosHolder topicQos = new TopicQosHolder();
	private PublisherQosHolder pubQos = new PublisherQosHolder();
	private SubscriberQosHolder subQos = new SubscriberQosHolder();

	private DataWriterQosHolder WQosH = new DataWriterQosHolder();

	private Publisher publisher;
	private DataWriter writer;

	private Subscriber subscriber;
	private DataReader reader;

	private String typeName;
	private String partitionName;
        private Boolean hasReader;
        private Boolean hasWriter;
        private int debugLevel;
        private int sndSeqNum;
        private int rcvSeqNum;

// INSERT TYPE SUPPORT      
        public void salTypeSupport(String topicName) {
		String[] parts = topicName.split("_");
                if ("SALData".equals(parts[0]) ) {
                    if (topicName == "SALData_SALTopic") {
			SALTopicTypeSupport SALTopicTS = new SALTopicTypeSupport();
			registerType(SALTopicTS);
		    }
		}
	}
	public int putSample(SALTopic data)
	{
          int status = SAL__OK;
	  DataWriter dwriter = getWriter();
	  SALTopicDataWriter SALWriter = SALTopicDataWriterHelper.narrow(dwriter);
	  data.private_revCode = "LSST TEST REVCODE";
	  if (debugLevel > 0) {
	    System.out.println("=== [putSample] writing a message containing :");
	    System.out.println("    revCode  : " + data.private_revCode);
	  }
	  SALWriter.register_instance(data);
	  status = SALWriter.write(data, HANDLE_NIL.value);
	  checkStatus(status, "SALCommandDataWriter.write");
	  return status;
	}


	public int getSample(SALTopic data)
	{
	  int status =  -1;
          int last = 0;
          SALTopicSeqHolder SALInstance = new SALTopicSeqHolder();
	  DataReader dreader = getReader();
	  SALTopicDataReader SALReader = SALTopicDataReaderHelper.narrow(dreader);
  	  SampleInfoSeqHolder infoSeq = new SampleInfoSeqHolder();
	  SALReader.take(SALInstance, infoSeq, LENGTH_UNLIMITED.value,
					ANY_SAMPLE_STATE.value, ANY_VIEW_STATE.value,
					ANY_INSTANCE_STATE.value);
	  if (debugLevel > 0) {
		for (int i = 0; i < SALInstance.value.length; i++) {
				System.out.println("=== [getSample] message received :");
				System.out.println("    revCode  : "
						+ SALInstance.value[i].private_revCode);
                   last = i;
		}
	  }
          if (last > 0) {
            data = SALInstance.value[last];
          }
          status = SALReader.return_loan (SALInstance, infoSeq);
	  return last;
	}



// END OF INSERT TYPE SUPPORT      


        public void salTelemetryPub(String topicName) {
		String partitionName = "SAL LSST";

		// create Domain Participant
		createParticipant(partitionName);

		// create Type
		salTypeSupport(topicName);

		// create Topic
		createTopic(topicName);

		// create Publisher
		createPublisher();

		// create DataWriter
		createWriter();

		// Publish Events

		DataWriter dwriter = getWriter();
        }

        public void salTelemetrySub(String topicName) {
		String partitionName = "SAL LSST";
		// create Domain Participant
		createParticipant(partitionName);

		// create Type
		salTypeSupport(topicName);
		// create Type

		// create Topic
		createTopic(topicName);

		// create Subscriber
		createSubscriber();

		// create DataReader
		createReader();

	}

	public void logError(int status) 
	{
	   System.out.println("=== ERROR return value = " + status); 
	}


        public void salInitialize()
	{
	   hasReader = false;
	   hasWriter = false;
	   debugLevel = 1;
	}

	int issueCommand( SALCommand data )
	{
          int status;
	  if (!hasWriter) {
	     createWriter();
	     hasWriter = true;
	     sndSeqNum = 1;
	  }
	  DataWriter dwriter = getWriter();	
	  SALCommandDataWriter SALWriter = SALCommandDataWriterHelper.narrow(dwriter);
	  data.private_revCode = "LSST TEST COMMAND";
	  data.private_seqNum = sndSeqNum;
	  if (debugLevel > 0) {
	    System.out.println( "=== [issueCommand] writing a command containing :");
	    System.out.println( " " + data.device + "." + data.property + "." + data.action + " : " + data.value);
	  }
	  SALWriter.register_instance(data);
	  status = SALWriter.write(data, HANDLE_NIL.value);
	  checkStatus(status, "SALCommandDataWriter.write");
	  sndSeqNum++;
	  return (sndSeqNum-1);
	}

	public int checkCommand( int cmdSeqNum )
	{
	   int status;
	   SALResponseSeqHolder response = new SALResponseSeqHolder();
	   status = getResponse(response);
	   return status;
	}

	public int getResponse(SALResponseSeqHolder data)
	{
	  int status =  -1;
          int lastsample = 0;

	  DataReader dreader = getReader();
	  SALResponseDataReader SALReader = SALResponseDataReaderHelper.narrow(dreader);
  	  SampleInfoSeqHolder infoSeq = new SampleInfoSeqHolder();
	  SALReader.take(data, infoSeq, LENGTH_UNLIMITED.value,
					ANY_SAMPLE_STATE.value, ANY_VIEW_STATE.value,
					ANY_INSTANCE_STATE.value);
	  if (debugLevel > 0) {
		for (int i = 0; i < data.value.length; i++) {
				System.out.println("=== [getResponse] message received :");
				System.out.println("    revCode  : "
						+ data.value[i].private_revCode);
                    lastsample = i;
		}
	  }
	  status = data.value[lastsample].ack;
	  rcvSeqNum = data.value[lastsample].private_seqNum;
	  return status;
	}

	public int cancelCommand( int cmdSeqNum )
	{
	   int status = 0;
	   return status;
	}


	public int abortCommand( int cmdSeqNum )
	{
	   int status = 0;
	   return status;
	}

        public static final int SAL__CMD_COMPLETE = 300;
        public static final int SAL__CMD_NOACK = -300;
        public static final int SAL__SLOWPOLL= 1;
        public static final int SAL__OK = 0;
        public static final int SAL__LOG_ROUTINES = 1;
        
	public int waitForCompletion( int cmdSeqNum , int timeout )
	{
	   int status = 0;
	   int countdown = timeout;
	   SALResponseSeqHolder response = new SALResponseSeqHolder();

	   while (status != SAL__CMD_COMPLETE && countdown > 0) {
	      status = getResponse(response);
	      if (response.value[0].private_seqNum != cmdSeqNum) { 
	         status = SAL__CMD_NOACK;
	      }
	      try
		{
	 	  Thread.sleep(1);
		}
		catch(InterruptedException ie)
		{
			// nothing to do
	      }
	      countdown--;
	   }
	   if (status != SAL__CMD_COMPLETE) {
	          logError(status);
	   }
 	   return status;
	}


	public int getEvent(SALEventSeqHolder data)
	{
	  int status =  -1;

	  DataReader dreader = getReader();
	  SALEventDataReader SALReader = SALEventDataReaderHelper.narrow(dreader);
  	  SampleInfoSeqHolder infoSeq = new SampleInfoSeqHolder();
	  SALReader.take(data, infoSeq, LENGTH_UNLIMITED.value,
					ANY_SAMPLE_STATE.value, ANY_VIEW_STATE.value,
					ANY_INSTANCE_STATE.value);
	  if (debugLevel > 0) {
		for (int i = 0; i < data.value.length; i++) {
				System.out.println("=== [getSample] message received :");
				System.out.println("    revCode  : "
						+ data.value[i].private_revCode);
		}
	  }
	  return status;
	}

	public int setDebugLevel( int level )
	{
           int status = 0;
	   debugLevel = level;
	   return status;	
	}

	public int getDebugLevel( int level )
	{
	   return debugLevel;
	}

	public int getOrigin()
	{
	   int status = 0;
	   return status;
	}

	public int getProperty(String property, String value)
	{
	   int status = SAL__OK;
	   value = "UNKNOWN";
	   if (status != SAL__OK) {
	      if (debugLevel >= SAL__LOG_ROUTINES) {
	          logError(status);
	      }
	   }
	   return status;
	}

	public int setProperty(String property, String value)
	{
           int status = SAL__OK;
	   if (status != SAL__OK) {
	      if (debugLevel >= SAL__LOG_ROUTINES) {
	          logError(status);
	      }
	   }
	   return status;
	}
 

	public void salCommand(String topicName)
	{
		String partitionName = "SAL LSST";

		// create Domain Participant
		createParticipant(partitionName);

		// create Type
		SALCommandTypeSupport SALCommandTS = new SALCommandTypeSupport();
		registerType(SALCommandTS);

		// create Topic
		createTopic(topicName);
	}


	public void salShutdown()
	{
	  if ( hasReader ) { 
	     deleteSubscriber();     
	  }
	  if ( hasWriter ) {
	     deletePublisher();
	  }
	  deleteTopic();
	  deleteParticipant();
	}

	public void salEvent(String topicName)
	{
		String partitionName = "SAL LSST";

		// create Domain Participant
		createParticipant(partitionName);

		// create Type
		SALEventTypeSupport SALEventTS = new SALEventTypeSupport();
		registerType(SALEventTS);

		// create Topic
		createTopic(topicName);
	}

	public void createParticipant(String partitionName) {
		dpf = DomainParticipantFactory.get_instance();
		checkHandle(dpf, "DomainParticipantFactory.get_instance");

		participant = dpf.create_participant(DOMAIN_ID_DEFAULT.value,
				PARTICIPANT_QOS_DEFAULT.value, null, STATUS_MASK_NONE.value);
		checkHandle(dpf,
				"DomainParticipantFactory.create_participant");
		this.partitionName = partitionName;
	}

	public void deleteParticipant() {
		dpf.delete_participant(participant);
	}

	public void registerType(TypeSupportImpl ts) {
		typeName = ts.get_type_name();
		int status = ts.register_type(participant, typeName);
		checkStatus(status, "register_type");
	}

	public void createTopic(String topicName) {
		int status = -1;
		participant.get_default_topic_qos(topicQos);
		topicQos.value.reliability.kind = ReliabilityQosPolicyKind.RELIABLE_RELIABILITY_QOS;
		topicQos.value.durability.kind = DurabilityQosPolicyKind.TRANSIENT_DURABILITY_QOS;
		status = participant.set_default_topic_qos(topicQos.value);
		checkStatus(status,
				"DomainParticipant.set_default_topic_qos");
		topic = participant.create_topic(topicName, typeName, topicQos.value,
				null, STATUS_MASK_NONE.value);
		checkHandle(topic, "DomainParticipant.create_topic");
	}

	public void deleteTopic() {
		int status = participant.delete_topic(topic);
		checkStatus(status, "DDS.DomainParticipant.delete_topic");
	}

	public void createPublisher() {
		int status = participant.get_default_publisher_qos(pubQos);
		checkStatus(status,
				"DomainParticipant.get_default_publisher_qos");

		pubQos.value.partition.name = new String[1];
		pubQos.value.partition.name[0] = partitionName;
		publisher = participant.create_publisher(pubQos.value, null,
				STATUS_MASK_NONE.value);
		checkHandle(publisher,
				"DomainParticipant.create_publisher");
	}

	public void deletePublisher() {
		participant.delete_publisher(publisher);
	}

	public void createWriter() {
		publisher.get_default_datawriter_qos(WQosH);
		publisher.copy_from_topic_qos(WQosH, topicQos.value);
		WQosH.value.writer_data_lifecycle.autodispose_unregistered_instances = false;
		writer = publisher.create_datawriter(topic, WQosH.value, null,STATUS_MASK_NONE.value);
		checkHandle(writer, "Publisher.create_datawriter");
	}
        
	public void createSubscriber() {
		int status = participant.get_default_subscriber_qos(subQos);
		checkStatus(status,
				"DomainParticipant.get_default_subscriber_qos");

		subQos.value.partition.name = new String[1];
		subQos.value.partition.name[0] = partitionName;
		subscriber = participant.create_subscriber(subQos.value, null,
				STATUS_MASK_NONE.value);
		checkHandle(subscriber,
				"DomainParticipant.create_subscriber");
	}

	public void deleteSubscriber() {
		participant.delete_subscriber(subscriber);
	}

	public void createReader() {
		reader = subscriber.create_datareader(topic,
				DATAREADER_QOS_USE_TOPIC_QOS.value, null, STATUS_MASK_NONE.value);
		checkHandle(reader, "Subscriber.create_datareader");
	}

	public DataReader getReader() {
		return reader;
	}

	public DataWriter getWriter() {
		return writer;
	}

	public Publisher getPublisher() {
		return publisher;
	}

	public Subscriber getSubscriber() {
		return subscriber;
	}

	public Topic getTopic() {
		return topic;
	}

	public DomainParticipant getParticipant() {
		return participant;
	}

	public static final int NR_ERROR_CODES = 13;

	/* Array to hold the names for all ReturnCodes. */
	public static String[] RetCodeName = new String[NR_ERROR_CODES];

	static {
		RetCodeName[0] = new String("DDS_RETCODE_OK");
		RetCodeName[1] = new String("DDS_RETCODE_ERROR");
		RetCodeName[2] = new String("DDS_RETCODE_UNSUPPORTED");
		RetCodeName[3] = new String("DDS_RETCODE_BAD_PARAMETER");
		RetCodeName[4] = new String("DDS_RETCODE_PRECONDITION_NOT_MET");
		RetCodeName[5] = new String("DDS_RETCODE_OUT_OF_RESOURCES");
		RetCodeName[6] = new String("DDS_RETCODE_NOT_ENABLED");
		RetCodeName[7] = new String("DDS_RETCODE_IMMUTABLE_POLICY");
		RetCodeName[8] = new String("DDS_RETCODE_INCONSISTENT_POLICY");
		RetCodeName[9] = new String("DDS_RETCODE_ALREADY_DELETED");
		RetCodeName[10] = new String("DDS_RETCODE_TIMEOUT");
		RetCodeName[11] = new String("DDS_RETCODE_NO_DATA");
		RetCodeName[12] = new String("DDS_RETCODE_ILLEGAL_OPERATION");
	}

	/**
	 * Returns the name of an error code.
	 **/
	public static String getErrorName(int status) {
		return RetCodeName[status];
	}

	/**
	 * Check the return status for errors. If there is an error, then terminate.
	 **/
	public static void checkStatus(int status, String info) {
		if (status != RETCODE_OK.value && status != RETCODE_NO_DATA.value) {
			System.out
					.println("Error in " + info + ": " + getErrorName
(status));
			System.exit(-1);
		}
	}

	/**
	 * Check whether a valid handle has been returned. If not, then terminate.
	 **/
	public static void checkHandle(Object handle, String info) {
		if (handle == null) {
			System.out.println("Error in " + info
					+ ": Creation failed: invalid handle");
			System.exit(-1);
		}
	}

}

