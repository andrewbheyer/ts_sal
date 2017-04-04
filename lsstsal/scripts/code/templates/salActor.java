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

public class salActor {
	public String baseName;
	public String topicName;
	public Topic topic;
        public Topic topic2;
        public ContentFilteredTopic filteredtopic;
        public ContentFilteredTopic filteredtopic2;
	public TopicQosHolder topicQos    = new TopicQosHolder();
	public TopicQosHolder topicQos2   = new TopicQosHolder();
	public PublisherQosHolder pubQos  = new PublisherQosHolder();
	public SubscriberQosHolder subQos = new SubscriberQosHolder();
	public DataWriterQosHolder WQosH  = new DataWriterQosHolder();
	public Publisher publisher;
	public DataWriter writer;
	public DataWriter writer2;
	public Subscriber subscriber;
	public DataReader reader;
	public DataReader reader2;
	public String typeName;
	public String typeName2;
        public Boolean isActive;
        public Boolean isReader;
        public Boolean isWriter;
        public Boolean isEventReader;
        public Boolean isEventWriter;
        public Boolean isProcessor;
        public Boolean isCommand;
        public int debugLevel;
        public int maxSamples;
        public int sndSeqNum;
        public int cmdSeqNum;
        public int rcvSeqNum;
        public int rcvOrigin;
        public int error;
        public int ack;
        public String result;
        public double sndStamp;
        public double rcvStamp;
	public double sampleAge;

    public salActor() {
	this.isActive = false;
	this.isReader = false;
	this.isWriter = false;
	this.isCommand = false;
	this.isEventReader = false;
	this.isEventWriter = false;
	this.isProcessor = false;
        this.maxSamples = 999999999;
    }
}




