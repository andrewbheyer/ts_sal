
source $SAL_DIR/gencommandtests.tcl 
source $SAL_DIR/gencommandtestsjava.tcl 
source $SAL_DIR/gentestspython.tcl 

proc gencmdaliascode { subsys lang fout } {
global CMD_ALIASES CMDS DONE_CMDEVT
 if { [info exists CMD_ALIASES($subsys)] } {
  stdlog "Generate command alias support for $lang"
  if { $lang == "include" } {
     foreach i $CMD_ALIASES($subsys) { 
       if { [info exists CMDS($subsys,$i,param)] } {
         puts $fout "
      int issueCommand_[set i]( SALData_command_[set i]C *data);
      int acceptCommand_[set i]( SALData_command_[set i]C *data);
      salReturn waitForCompletion_[set i]( int cmdSeqNum , unsigned int timeout );
      salReturn ackCommand_[set i]( int cmdSeqNum, salLONG  ack, salLONG error, char *result );
      salReturn getResponse_[set i](SALData::ackcmdSeq data);"
       }
     }
  }
  if { $lang == "cpp" } {
     catch { set result [gencmdaliascpp $subsys $fout] } bad
     stdlog "$result"
     if { $DONE_CMDEVT == 0} {
       catch { set result [gencommandtestscpp $subsys] } bad
       stdlog "$result"
     }
  }
  if { $lang == "java" }  {
     catch { set result [gencmdaliasjava $subsys $fout] } bad
     stdlog "$result"
     if { $DONE_CMDEVT == 0} {
       catch { set result [gencommandtestsjava $subsys] } bad
       stdlog "$result"
     }
  }
  if { $lang == "python" } {
     catch { set result [gencmdaliaspython $subsys $fout] } bad
     stdlog "$result"
     if { $DONE_CMDEVT == 0} {
       catch { set result [gencommandtestspython $subsys] } bad
       stdlog "$result"
     }
  }
  if { $lang == "isocpp" } {
     catch { set result [gencmdaliasisocpp $subsys $fout] } bad
     stdlog "$result"
  }
 }
}


proc gencmdaliascpp { subsys fout } {
global CMD_ALIASES CMDS SAL_WORK_DIR
   foreach i $CMD_ALIASES($subsys) {
    if { [info exists CMDS($subsys,$i,param)] } {
      stdlog "	: command alias = $i"
      puts $fout "
int SAL_SALData::issueCommand_[set i]( SALData_command_[set i]C *data )
\{
  
  InstanceHandle_t cmdHandle = DDS::HANDLE_NIL;
  SALData::command_[set i] Instance;
  int actorIdx = SAL__SALData_command_[set i]_ACTOR;
  // create DataWriter :
  if (sal\[actorIdx\].isCommand == false) \{
     salCommand(sal\[actorIdx\].topicName);
     sal\[actorIdx\].isCommand = true;
     sal\[actorIdx\].sndSeqNum = rand();
  \}
  DataWriter_var dwriter = getWriter(actorIdx);
  SALCommand_[set i]DataWriter_var SALWriter = SALCommand_[set i]DataWriter::_narrow(dwriter.in());

#ifdef SAL_SUBSYSTEM_ID_IS_KEYED
  Instance.SALDataID = subsystemID;
  cmdHandle = SALWriter->register_instance(Instance);
#endif

  Instance.private_revCode =  DDS::string_dup(\"LSST TEST REVCODE\");
  Instance.private_sndStamp = getCurrentTime();
  Instance.private_origin =   1;
  Instance.private_seqNum =   sal\[actorIdx\].sndSeqNum;
  Instance.private_host =     1;"
        set fin [open $SAL_WORK_DIR/include/SAL_[set subsys]_command_[set i]Cput.tmp r]
        while { [gets $fin rec] > -1 } {
           puts $fout $rec
        }
        close $fin
        puts $fout "
  if (debugLevel > 0) \{
    cout << \"=== \[issueCommand_[set i]\] writing a command containing :\" << endl;"
        set fin [open $SAL_WORK_DIR/include/SAL_[set subsys]_command_[set i]Cout.tmp r]
        while { [gets $fin rec] > -1 } {
           puts $fout $rec
        }
        close $fin
        puts $fout "
  \}
  Instance.private_sndStamp = getCurrentTime();
  ReturnCode_t status = SALWriter->write(Instance, cmdHandle);
  sal\[actorIdx\].sndSeqNum++;
  checkStatus(status, \"SALCommand_[set i]DataWriter::write\");  
    SALWriter->unregister_instance(Instance, cmdHandle);
  if (status != SAL__OK) \{
      if (debugLevel >= SAL__LOG_ROUTINES) \{
          logError(status);
      \}
  \}
  return (sal\[actorIdx\].sndSeqNum-1);
\}
"
      puts $fout "
int SAL_SALData::acceptCommand_[set i]( SALData_command_[set i]C *data )
\{
   SampleInfoSeq info;
   ReturnCode_t status = 0;
   ReturnCode_t istatus =  -1;
   SALData::command_[set i]Seq Instances;
   SALData::ackcmd ackdata;
   InstanceHandle_t ackHandle = DDS::HANDLE_NIL;
   int actorIdx = SAL__SALData_command_[set i]_ACTOR;
   int j=0;

  // create DataWriter :
  if (sal\[actorIdx\].isProcessor == false) \{
     salProcessor(sal\[actorIdx\].topicName);
     sal\[actorIdx\].isProcessor = true;
  \}
  DataWriter_var dwriter = getWriter2(SAL__SALData_ackcmd_ACTOR);
  SALData::ackcmdDataWriter_var SALWriter = SALData::ackcmdDataWriter::_narrow(dwriter.in());
  DataReader_var dreader = getReader(actorIdx);
  SALData::command_[set i]DataReader_var SALReader = SALData::command_[set i]DataReader::_narrow(dreader.in());
  checkHandle(SALReader.in(), \"SALData::command_[set i]DataReader::_narrow\");
  istatus = SALReader->take(Instances, info, 1,ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
  checkStatus(istatus, \"SALData::command_[set i]DataReader::take\");
  if (Instances.length() > 0) \{
   j = Instances.length()-1;
   if (info\[j\].valid_data) \{
    if (debugLevel > 8) \{
      cout << \"=== \[acceptCommandC $i\] reading a command containing :\" << endl;
      cout << \"    seqNum   : \" << Instances\[j\].private_seqNum << endl;
      cout << \"    device   : \" << Instances\[j\].device << endl;
      cout << \"    property : \" << Instances\[j\].property << endl;
      cout << \"    action   : \" << Instances\[j\].action << endl;
      cout << \"    value    : \" << Instances\[j\].value << endl;
      cout << \"    sample-state : \" << info\[j\].sample_state << endl;
      cout << \"    view-state : \" << info\[j\].view_state << endl;
      cout << \"    instance-state : \" << info\[j\].instance_state << endl;
    \}
#ifdef SAL_SUBSYSTEM_ID_IS_KEYED
    ackdata.SALDataID = subsystemID;
#endif
    ackdata.private_origin = Instances\[j\].private_origin;
    ackdata.private_seqNum = Instances\[j\].private_seqNum;
    ackdata.error = 0;
    ackdata.result = DDS::string_dup(\"SAL ACK\");
    status = Instances\[j\].private_seqNum;
    rcvdTime = getCurrentTime();
    if ( (rcvdTime - Instances\[j\].private_sndStamp) < sal\[actorIdx\].sampleAge ) \{
      rcvSeqNum = status;
      rcvOrigin = Instances\[j\].private_origin;
      ackdata.ack = SAL__CMD_ACK;"
        set fin [open $SAL_WORK_DIR/include/SAL_[set subsys]_command_[set i]Cget.tmp r]
        while { [gets $fin rec] > -1 } {
           puts $fout $rec
        }
        close $fin
        puts $fout "
#ifdef SAL_SUBSYSTEM_ID_IS_KEYED
    ackHandle = SALWriter->register_instance(ackdata);
    ackdata.SALDataID = subsystemID;
#endif
    ackdata.private_sndStamp = getCurrentTime();
    istatus = SALWriter->write(ackdata, ackHandle);
    checkStatus(istatus, \"SALData::ackcmdDataWriter::write\");
    SALWriter->unregister_instance(ackdata, ackHandle);
     \} else \{
        if (debugLevel > 8) \{
          cout << \"    Old command ignored : \" << status << \":\" << int(rcvdTime) << endl;
        \}
        status = 0;
     \}
   \}
  \} else \{
    status = 0;
  \}
  istatus = SALReader->return_loan(Instances, info);
  checkStatus(istatus, \"SALData::command_[set i]DataReader::return_loan\");
  return status;
\}
"
   puts $fout "
salReturn SAL_SALData::waitForCompletion_[set i]( int cmdSeqNum , unsigned int timeout )
\{
   salReturn status = SAL__OK;
   int countdown = timeout*100;
   SALData::ackcmdSeq response;
   int actorIdx = SAL__SALData_command_[set i]_ACTOR;

   while (status != SAL__CMD_COMPLETE && countdown != 0) \{
      status = getResponse_[set i](response);
      if (status != SAL__CMD_NOACK) \{
        if (sal\[actorIdx\].rcvSeqNum != cmdSeqNum) \{ 
           status = SAL__CMD_NOACK;
        \}
      \}
      usleep(SAL__SLOWPOLL);
      countdown--;
   \}
   if (status != SAL__CMD_COMPLETE) \{
      if (debugLevel >= SAL__LOG_ROUTINES) \{
          logError(status);
      \}
      if (debugLevel > 0) \{
         cout << \"=== \[waitForCompletion_[set i]\] command \" << cmdSeqNum <<  \" timed out :\" << endl;
      \} 
   \} else \{
      if (debugLevel > 0) \{
         cout << \"=== \[waitForCompletion_[set i]\] command \" << cmdSeqNum << \" completed ok :\" << endl;
      \} 
   \}
   return status;
\}
"
   puts $fout "
salReturn SAL_SALData::getResponse_[set i](SALData::ackcmdSeq data)
\{
  int actorIdx = SAL__SALData_ackcmd_ACTOR;
  int actorIdxCmd = SAL__SALData_command_[set i]_ACTOR;
  SampleInfoSeq info;
  ReturnCode_t status = SAL__CMD_NOACK;
  ReturnCode_t istatus =  -1;
  int j=0;
  DataReader_var dreader = getReader2(actorIdx);
  SALData::ackcmdDataReader_var SALReader = SALData::ackcmdDataReader::_narrow(dreader.in());
  checkHandle(SALReader.in(), \"SALData::ackcmdDataReader::_narrow\");
  istatus = SALReader->take(data, info, 1,ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
  sal\[actorIdxCmd\].rcvSeqNum = 0;
  sal\[actorIdxCmd\].rcvOrigin = 0;
  checkStatus(istatus, \"SALData::ackcmdDataReader::take\");
  if (data.length() > 0) \{
   j = data.length()-1;
   if (data\[j\].private_seqNum > 0) \{
    if (debugLevel > 8) \{
      cout << \"=== \[getResponse_[set i]\] reading a message containing :\" << endl;
      cout << \"    seqNum   : \" << data\[j\].private_seqNum << endl;
      cout << \"    error    : \" << data\[j\].error << endl;
      cout << \"    ack      : \" << data\[j\].ack << endl;
      cout << \"    result   : \" << data\[j\].result << endl;
      cout << \"    sample-state : \" << info\[j\].sample_state << endl;
      cout << \"    view-state : \" << info\[j\].view_state << endl;
      cout << \"    instance-state : \" << info\[j\].instance_state << endl;
    \}
    status = data\[j\].ack;
    rcvdTime = getCurrentTime();
    sal\[actorIdxCmd\].rcvSeqNum = data\[j\].private_seqNum;
    sal\[actorIdxCmd\].rcvOrigin = data\[j\].private_origin;
    sal\[actorIdxCmd\].ack = data\[j\].ack;
    sal\[actorIdxCmd\].error = data\[j\].error;
    strcpy(sal\[actorIdxCmd\].result,DDS::string_dup(data\[j\].result));
   \} else \{
      cout << \"=== \[getResponse_[set i]\] No ack yet!\" << endl;
      status = SAL__CMD_NOACK;
   \}
  \}
  istatus = SALReader->return_loan(data, info);
  checkStatus(istatus, \"SALData::ackcmdDataReader::return_loan\");
  return status;
\}
"
   puts $fout "
salReturn SAL_SALData::ackCommand_[set i]( int cmdId, salLONG ack, salLONG error, char *result )
\{
   ReturnCode_t istatus = -1;
   InstanceHandle_t ackHandle = DDS::HANDLE_NIL;
   int actorIdx = SAL__SALData_ackcmd_ACTOR;

   SALData::ackcmd ackdata;
   DataWriter_var dwriter = getWriter2(actorIdx);
   SALData::ackcmdDataWriter_var SALWriter = SALData::ackcmdDataWriter::_narrow(dwriter.in());

   ackdata.private_origin = sal\[actorIdx\].rcvOrigin;
   ackdata.private_seqNum = cmdId;
   ackdata.error = error;
   ackdata.ack = ack;
   ackdata.result = DDS::string_dup(result);
#ifdef SAL_SUBSYSTEM_ID_IS_KEYED
   ackdata.SALDataID = subsystemID;
#endif
   if (debugLevel > 0) \{
      cout << \"=== \[ackCommand_[set i]\] acknowledging a command with :\" << endl;
      cout << \"    seqNum   : \" << ackdata.private_seqNum << endl;
      cout << \"    ack      : \" << ackdata.ack << endl;
      cout << \"    error    : \" << ackdata.error << endl;
      cout << \"    result   : \" << ackdata.result << endl;
   \}
#ifdef SAL_SUBSYSTEM_ID_IS_KEYED
   ackHandle = SALWriter->register_instance(ackdata);
   ackdata.SALDataID = subsystemID;
#endif
   ackdata.private_sndStamp = getCurrentTime();
   istatus = SALWriter->write(ackdata, ackHandle);
   checkStatus(istatus, \"SALData::ackcmdDataWriter::return_loan\");
    SALWriter->unregister_instance(ackdata, ackHandle);
   return SAL__OK;
\}
"
     } else {
#      stdlog "Alias $i has no parameters - uses standard [set subsys]_command"
     }
   }
}



proc gencmdaliasjava { subsys fout } {
global CMD_ALIASES CMDS SYSDIC
   foreach i $CMD_ALIASES($subsys) {
    stdlog "	: alias = $i"
    if { [info exists CMDS($subsys,$i,param)] } {
      puts $fout "
	public int issueCommand_[set i]( command_[set i] data )
	\{
          Random randGen = new java.util.Random();
  	  long cmdHandle = HANDLE_NIL.value;
          int status;
          int actorIdx = SAL__SALData_command_[set i]_ACTOR;
	  if (sal\[actorIdx\].isCommand == false) \{
	     salCommand(sal\[actorIdx\].topicName);
	     sal\[actorIdx\].isCommand = true;
	     sal\[actorIdx\].sndSeqNum = (int)randGen.nextInt(99999999);
	  \}
	  DataWriter dwriter = getWriter(actorIdx);	
	  command_[set i]DataWriter SALWriter = command_[set i]DataWriterHelper.narrow(dwriter);
	  data.private_revCode = \"LSST TEST COMMAND\";
	  data.private_seqNum = sal\[actorIdx\].sndSeqNum;
          data.private_sndStamp = getCurrentTime();"
      if { [info exists SYSDIC($subsys,keyedID)] } {
        puts $fout "	  data.SALDataID = subsystemID;
	  cmdHandle = SALWriter.register_instance(data);"
      } else {
        puts $fout "	  SALWriter.register_instance(data);"
      }
      puts $fout "
	  if (debugLevel > 0) \{
	    System.out.println( \"=== \[issueCommand\] $i writing a command containing :\");
	    System.out.println( data.device + \".\" + data.property + \".\" + data.action + \" : \" + data.value);
	  \}
	  status = SALWriter.write(data, cmdHandle);
	  sal\[actorIdx\].sndSeqNum++;
	  return (sal\[actorIdx\].sndSeqNum-1);
	\}
"
      puts $fout "
	public int acceptCommand_[set i]( SALData.command_[set i] data )
	\{
                command_[set i]SeqHolder aCmd = new command_[set i]SeqHolder();
   		SampleInfoSeqHolder info;
   		int status = 0;
   		int istatus =  -1;
   		SALData.ackcmd ackdata;
   		long ackHandle = HANDLE_NIL.value;
                int actorIdx = SAL__SALData_command_[set i]_ACTOR;

  		// create DataWriter :
  		if (sal\[actorIdx\].isProcessor == false) \{
     			salProcessor(sal\[actorIdx\].topicName);
     			sal\[actorIdx\].isProcessor = true;
  		\}
  		DataWriter dwriter = getWriter2(SAL__SALData_ackcmd_ACTOR);
  		ackcmdDataWriter SALWriter = ackcmdDataWriterHelper.narrow(dwriter);
  		DataReader dreader = getReader(actorIdx);
  		command_[set i]DataReader SALReader = command_[set i]DataReaderHelper.narrow(dreader);
                info = new SampleInfoSeqHolder();
  		istatus = SALReader.take(aCmd, info, 1,ANY_SAMPLE_STATE.value, ANY_VIEW_STATE.value, ANY_INSTANCE_STATE.value);
		if (aCmd.value.length > 0) \{
   		  if (info.value\[0\].valid_data) \{
    		     if (debugLevel > 8) \{
      			System.out.println(  \"=== \[acceptCommand\] $i reading a command containing :\" );
      			System.out.println(  \"    seqNum   : \" + aCmd.value\[0\].private_seqNum );
      			System.out.println(  \"    device   : \" + aCmd.value\[0\].device);
      			System.out.println(  \"    property : \" + aCmd.value\[0\].property);
      			System.out.println(  \"    action   : \" + aCmd.value\[0\].action);
      			System.out.println(  \"    value    : \" + aCmd.value\[0\].value);
    		    \}
    		    status = aCmd.value\[0\].private_seqNum;
    		    double rcvdTime = getCurrentTime();
		    double dTime = rcvdTime - aCmd.value\[0\].private_sndStamp;
    		    if ( dTime < sal\[actorIdx\].sampleAge ) \{
                      ackdata = new SALData.ackcmd();"
      if { [info exists SYSDIC($subsys,keyedID)] } {
         puts $fout "	              ackdata.SALDataID = subsystemID;"
      }
      puts $fout "		      ackdata.private_origin = aCmd.value\[0\].private_origin;
		      ackdata.private_seqNum = aCmd.value\[0\].private_seqNum;
		      ackdata.error  = 0;
		      ackdata.result = \"SAL ACK\";
                      data.device    = aCmd.value\[0\].device;
                      data.property  = aCmd.value\[0\].property;
                      data.action    = aCmd.value\[0\].action;
                      data.value     = aCmd.value\[0\].value;"
           foreach p $CMDS($subsys,$i,param) {
              set apar [lindex [split [lindex [string trim $p "\{\}"] 1] "()"] 0] 
              puts $fout "                      data.$apar = aCmd.value\[0\].$apar;"
           }
           puts $fout "
		      status = aCmd.value\[0\].private_seqNum;
		      rcvSeqNum = status;
		      rcvOrigin = aCmd.value\[0\].private_origin;
		      ackdata.ack = SAL__CMD_ACK;"
      if { [info exists SYSDIC($subsys,keyedID)] } {
         puts $fout "		      ackdata.SALDataID = subsystemID;
		      ackHandle = SALWriter.register_instance(ackdata);"
      }
      puts $fout "
		      istatus = SALWriter.write(ackdata, ackHandle);
		      SALWriter.unregister_instance(ackdata, ackHandle);"
      puts $fout "  \} else \{
    		     if (debugLevel > 8) \{
      			System.out.println(  \"    Old command ignored :   \" + dTime );
                     \}
                   \}
		 \}
                \} else \{
  	           status = 0;
                \}
                SALReader.return_loan(aCmd, info);
	        return status;
	\}
"
   puts $fout "
	public int waitForCompletion_[set i]( int cmdSeqNum , int timeout )
	\{
	   int status = 0;
	   int countdown = timeout*1000;
           int actorIdx = SAL__SALData_command_[set i]_ACTOR;
	   ackcmdSeqHolder ackcmd = new ackcmdSeqHolder();

	   while (status != SAL__CMD_COMPLETE && countdown != 0) \{
	      status = getResponse_[set i](ackcmd);
	      if (status != SAL__CMD_NOACK) \{
	        if (sal\[actorIdx\].rcvSeqNum != cmdSeqNum) \{ 
	           status = SAL__CMD_NOACK;
	        \}
	      \}
	      try
		\{
	 	  Thread.sleep(timeout);
		\}
		catch(InterruptedException ie)
		\{
			// nothing to do
	      \}
	      countdown--;
	   \}
	   if (status != SAL__CMD_COMPLETE) \{
	      if (debugLevel > 0) \{
	         System.out.println( \"=== \[waitForCompletion_[set i]\] command \" + cmdSeqNum +  \" timed out\");
	      \} 
	      logError(status);
	   \} else \{
	      if (debugLevel > 0) \{
	         System.out.println( \"=== \[waitForCompletion_[set i]\] command \" + cmdSeqNum +  \" completed ok\");
	      \} 
           \}
 	   return status;
	\}
"
   puts $fout "
	public int getResponse_[set i](ackcmdSeqHolder data)
	\{
	  int status =  -1;
          int lastsample = 0;
          int actorIdx = SAL__SALData_ackcmd_ACTOR;
          int actorIdxCmd = SAL__SALData_command_[set i]_ACTOR;

	  DataReader dreader = getReader2(actorIdx);
	  ackcmdDataReader SALReader = ackcmdDataReaderHelper.narrow(dreader);
  	  SampleInfoSeqHolder infoSeq = new SampleInfoSeqHolder();
	  SALReader.take(data, infoSeq, 1, 
					ANY_SAMPLE_STATE.value,
					ANY_VIEW_STATE.value,
					ANY_INSTANCE_STATE.value);
	  if (data.value.length > 0) \{
 		for (int i = 0; i < data.value.length; i++) \{
                     if ( debugLevel > 8) \{
				System.out.println(\"=== \[getResponse_[set i]\] message received :\");
				System.out.println(\"    revCode  : \"
						+ data.value\[i\].private_revCode);
		    \}
                    lastsample = i;
		\}
	 	status = data.value\[lastsample\].ack;
	  	sal\[actorIdxCmd\].rcvSeqNum = data.value\[lastsample\].private_seqNum;
	  	sal\[actorIdxCmd\].rcvOrigin = data.value\[lastsample\].private_origin;
	  \} else \{
	        System.out.println(\"=== \[getResponse_[set i]\] No ack yet!\"); 
	        status = SAL__CMD_NOACK;
	  \}
    	  SALReader.return_loan(data, infoSeq);
	  return status;
	\}
"
   puts $fout "
	public int ackCommand_[set i]( int cmdId, int ack, int error, String result )
	\{
   		int istatus = -1;
   		long ackHandle = HANDLE_NIL.value;
                int actorIdx = SAL__SALData_command_[set i]_ACTOR;
                int actorIdx2 = SAL__SALData_ackcmd_ACTOR;

   		SALData.ackcmd ackdata;
   		DataWriter dwriter = getWriter2(actorIdx2);
   		ackcmdDataWriter SALWriter = ackcmdDataWriterHelper.narrow(dwriter);
                ackdata = new SALData.ackcmd();
   		ackdata.private_origin = sal\[actorIdx\].rcvOrigin;
   		ackdata.private_seqNum = cmdId;
   		ackdata.error = error;
   		ackdata.ack = ack;
   		ackdata.result = result;"
      if { [info exists SYSDIC($subsys,keyedID)] } {
         puts $fout "   		ackdata.SALDataID = subsystemID;"
      }
      puts $fout "
   		if (debugLevel > 0) \{
      			System.out.println(  \"=== \[ackCommand_[set i]\] acknowledging a command with :\" );
      			System.out.println(  \"    seqNum   : \" + ackdata.private_seqNum );
      			System.out.println(  \"    ack      : \" + ackdata.ack );
      			System.out.println(  \"    error    : \" + ackdata.error );
      			System.out.println(  \"    result   : \" + ackdata.result );
   		\}"
      if { [info exists SYSDIC($subsys,keyedID)] } {
         puts $fout "   		ackdata.SALDataID = subsystemID;
   		ackHandle = SALWriter.register_instance(ackdata);"
      }
      puts $fout "   		istatus = SALWriter.write(ackdata, ackHandle);"
      puts $fout "    		SALWriter.unregister_instance(ackdata, ackHandle);"
      puts $fout "
   		return SAL__OK;
	\}
"
    } else {
#      stdlog "Alias $i has no parameters - uses standard [set subsys]_command"
    }
  }
}



proc gencmdaliaspython { subsys fout } {
global CMD_ALIASES CMDS
   foreach i $CMD_ALIASES($subsys) {
    if { [info exists CMDS($subsys,$i,param)] } {
      stdlog "	: alias = $i"
      puts $fout "
        .def( \"issueCommand_[set i]\",       &::SAL_SALData::issueCommand_[set i] )
        .def( \"acceptCommand_[set i]\",      &::SAL_SALData::acceptCommand_[set i] )
        .def( \"ackCommand_[set i]\",         &::SAL_SALData::ackCommand_[set i] )
        .def( \"waitForCompletion_[set i]\",  &::SAL_SALData::waitForCompletion_[set i] )
      "
    } else {
      stdlog "Alias $i has no parameters - uses standard [set subsys]_command"
    }
   }
}



proc gencmdaliasisocpp { subsys fout } {
global CMD_ALIASES CMDS
   foreach i $CMD_ALIASES($subsys) { 
    if { [info exists CMDS($subsys,$i,param)] } {
      stdlog "	: alias = $i"
    } else {
      stdlog "Alias $i has no parameters - uses standard [set subsys]_command"
    }
   }
}







