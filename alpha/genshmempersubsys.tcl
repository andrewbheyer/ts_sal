#!/usr/bin/tclsh
set subsys $argv
puts stdout "Generating code for subsystem $argv"

proc doddsgen { topiclist } {
   parsepub $topiclist
   generatetester $topiclist
   parsemakefile $topiclist
}

proc calcshmid { subsys } {
  set fout [open /tmp/subsys.tmp w]
  puts $fout "$subsys"
  close $fout
  set id [string range [exec md5sum /tmp/subsys.tmp] 0 3]
  return $id
}


proc addshmemcode { topiclist fid } {
global VPROPS
  foreach subsys $topiclist {
    puts $fid "
   int [set subsys]_shmsize;
   int lshmid_[set subsys];
   int [set subsys]_shmid = 0x[calcshmid $subsys];
   [set subsys]_cache *[set subsys]_ref;"
  }
  foreach subsys $topiclist {
    puts $fid "
   [set subsys]_shmsize = sizeof(struct [set subsys]_cache);
   lshmid_[set subsys] = shmget([set subsys]_shmid, [set subsys]_shmsize , IPC_CREAT|0666);
   [set subsys]_ref  = ([set subsys]_cache *) shmat(lshmid_[set subsys], NULL, 0);"
  }
  puts $fid "
	while (1) \{
		nanosleep(10000);"
  foreach subsys $topiclist {
    puts $fid "
		if ([set subsys]_ref->syncO == 1) \{
			count++;"
     set fin [open ../shmem-[set subsys]/[set subsys]_putstub.txt r]
     while { [gets $fin rec] > -1 } {
       puts $fid $rec
     }
     close $fin
     puts $fid "		\}"
  }
  puts $fid "	\}"
}

proc shcoder { subsys } {
global SHC TOPICPROPS
  set fcod [open shmem_[set subsys].h w]
  puts $fcod $SHC(genericshmem.h)
  close $fcod
  set fcod [open shmem_[set subsys]_server.cpp w]
  puts $fcod $SHC(genericshmem.cpp)
  close $fcod
  set fcod [open test_shmem_[set subsys].cpp w]
  puts $fcod $SHC(genericshclient.cpp)
  close $fcod
  exec g++ -shared -g shmem_[set subsys]_server.cpp -o libshm_[set subsys]_server.so -lstdc++
  exec gcc -g test_shmem_[set subsys].cpp -o shm_test1 -lpthread -L. -lshm_[set subsys]_server
}


proc generatetester { subsys } {
  set fout [open [set subsys]_shmem_tester.c w]
  puts $fout "
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from [set subsys].idl using \"genshmem\".
  The genshmem tool is part of the LSST SAL middleware stack
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include \"[set subsys]_cache.h\"

int main(int argc, char *argv\[\])
\{
   int [set subsys]_shmsize;
   int lshmid;
   int icount=0;
   int [set subsys]_shmid = 0x[calcshmid $subsys];
   [set subsys]_cache *[set subsys]_ref;
   [set subsys]_shmsize = sizeof(struct [set subsys]_cache);
   lshmid = shmget([set subsys]_shmid, [set subsys]_shmsize , IPC_CREAT|0666);
   [set subsys]_ref  = ([set subsys]_cache *) shmat(lshmid, NULL, 0);
 
"
  set fin [open [set subsys]_cache.h r]
  gets $fin rec
  gets $fin rec
  while { [gets $fin rec] > -1 } {
     set v [string trim [lindex $rec 1] ";"]
     if { $v != "syncO" } {
       switch [lindex $rec 0] {
         byte -
         short -
         int - 
         long -
         float - 
         double { puts $fout "   [set subsys]_ref->$v++;" }
         char { set cv [lindex [split $v "\[\]"] 0]
                puts $fout "   icount++;
   sprintf([set subsys]_ref->$cv,\"string %d\",icount);" }
       }
     }
  }
  puts $fout "   [set subsys]_ref->syncO = 1;
   return(0);
\}
"  
  close $fout
}

  
proc generatetcllib { subsys } {
global LVERSION
  set fout [open shm_tcl_[set subsys].c w]
  set ftst [open test_[set subsys].tcl w]
  puts $ftst "set SHM[set subsys](shmid) 0x[calcshmid $subsys]"
  set sval [lindex [exec grep "size_t [set subsys]_shmsize =" shmem_[set subsys]_server.cpp] 3]
  puts $ftst "set SHM[set subsys](shmsize) [string trim $sval ";"]"
  puts $ftst "load /home/shared/lsst/lib/libshm.so"
  puts $fout "
    if (strcmp(subsysid,\"$subsys\") == 0) {
       [set subsys]_cache *[set subsys]_ref;
       [set subsys]_ref = ([set subsys]_cache *)shmdata_ref;
"
  set fin [open [set subsys]_cache.h r]
  gets $fin rec
  gets $fin rec
  while { [gets $fin rec] > -1 } {
     set v [string trim [lindex $rec 1] ";"]
     if { $v != "syncO" } {
       set v [lindex [split $v "\[\]"] 0]
       switch [lindex $rec 0] {
         byte - 
         short - 
         int  { puts $fout "
      text = Tcl_GetVar2(interp, \"SHM[set subsys]\", \"$v\", TCL_GLOBAL_ONLY);
      sscanf(text,\"%d\", &[set subsys]_ref->$v);"
                puts $ftst "set SHM[set subsys]($v) 1"
              }
         long { puts $fout "
      text = Tcl_GetVar2(interp, \"SHM[set subsys]\", \"$v\", TCL_GLOBAL_ONLY);
      sscanf(text,\"%ld\", &[set subsys]_ref->$v);"
                puts $ftst "set SHM[set subsys]($v) 1.2"
              }
         float { puts $fout "
      text = Tcl_GetVar2(interp, \"SHM[set subsys]\", \"$v\", TCL_GLOBAL_ONLY);
      sscanf(text,\"%f\", &[set subsys]_ref->$v);"
                puts $ftst "set SHM[set subsys]($v) 1.23"
              }
         double { puts $fout "
      text = Tcl_GetVar2(interp, \"SHM[set subsys]\", \"$v\", TCL_GLOBAL_ONLY);
      sscanf(text,\"%lf\", &[set subsys]_ref->$v);"
                puts $ftst "set SHM[set subsys]($v) 1.234"
              }
         char { puts $fout "
      text = Tcl_GetVar2(interp, \"SHM[set subsys]\", \"$v\", TCL_GLOBAL_ONLY);
      strcpy([set subsys]_ref->$v,text);"
                puts $ftst "set SHM[set subsys]($v) \"test $v\""
             }
       }
     }
  }
    puts $fout "      [set subsys]_ref->syncO = 1;
    }"
  close $fout
  close $ftst
  set ftst [open test_singleshot.tcl w]
  puts $ftst "#!/usr/bin/tclsh

source /home/shared/lsst/tests/shmem/shmem-[set subsys]/test_[set subsys].tcl 
set id \$SHM[set subsys](shmid)
set size \$SHM[set subsys](shmsize)
writeshm [set subsys] \$id \$size

"
  close $ftst
  exec chmod 755 test_singleshot.tcl
  set ftst [open startpubsub w]
  puts $ftst "#!/bin/sh
xterm -e /home/shared/lsst/tests/shmem/shmem-[set subsys]/objs/$LVERSION/[set subsys]_shmem_publisher &
xterm -e /home/shared/lsst/tests/shmem/shmem-[set subsys]/objs/$LVERSION/[set subsys]_subscriber &
echo \"Use /home/shared/lsst/tests/shmem/shmem-[set subsys]/test_singleshot.tcl to test\"
"
  close $ftst
  exec chmod 755 startpubsub
}

proc replaceshmemcode { topiclist op fid } {
  switch $op {
        includes {
           puts $fid "#include <sys/shm.h>"
           foreach t $topiclist {
             puts $fid "#include \"[set subsys].h\""
             puts $fid "#include \"[set subsys]Support.h\""
             puts $fid "#include \"[set subsys]_cache.h\""
           }
        }
        finalize {
           foreach t $topiclist {
 	     puts $fid "    [set  t]TypeSupport_finalize();"
           } 
        }
        tidyup {
           foreach t $topiclist {
             puts $fid "
    [set t]_retcode = [set t]DataWriter_unregister_instance(
        [set t]_writer, [set t]_instance, &[set t]_instance_handle);
    if ([set t]_retcode != DDS_RETCODE_OK) {
        printf(\"[set t] unregister instance error %d\\n\", [set t]_retcode);
    \}
    /* Delete data sample */
    [set t]_retcode = [set t]TypeSupport_delete_data_ex(instance, DDS_BOOLEAN_TRUE);
    if ([set t]_retcode != DDS_RETCODE_OK) {
        printf(\[set t\]TypeSupport_delete_data error %d\\n\", [set t]_retcode);
    \}"
           } 
        }
        variables {
           foreach t $topiclist {
             puts $fid "
    DDS_Topic *[set t]topic = NULL;
    DDS_DataWriter *[set t]_DDSwriter = NULL;
    [set t]DataWriter *[set t]_writer = NULL;
    [set t] *[set t]_instance = NULL;
    DDS_ReturnCode_t [set t]_retcode;
    DDS_InstanceHandle_t [set t]_instance_handle = DDS_HANDLE_NIL;
    const char *[set t]_type_name = NULL;
    int [set t]_count = 0;  
    struct DDS_Duration_t [set t]_send_period = \{1,0\};"
          } 
        }
        registertypes {
           foreach t $topiclist {
              puts $fid "
    [set t]_type_name = [set t]_TypeSupport_get_type_name();
    [set t]_retcode = [set t]_TypeSupport_register_type(
        participant, [set t]_type_name);
    if ([set t]_retcode != DDS_RETCODE_OK) {
        printf(\"[set t] register_type error %d\\n\", [set t]_retcode);
        publisher_shutdown(participant);
        return -1;
    \}"
           } 
        }
        createtopics {
           foreach t $topiclist {
             puts $fid "
     [set t]topic = DDS_DomainParticipant_create_topic(
        participant, "Example [set t]",
        [set t]type_name, &DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if ([set t]topic == NULL) \{
        printf(\"[set t] create_topic error\\n\");
        publisher_shutdown(participant);
        return -1;
    \}"
          } 
        }
        createwriters {
           foreach t $topiclist {
             puts $fid "
    [set t]_DDSwriter = DDS_Publisher_create_datawriter(
        publisher, [set t]_topic,
        &DDS_DATAWRITER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
    if ([set t]_DDSwriter == NULL) \{
        printf(\"[set t ] create_datawriter error\\n\");
        publisher_shutdown(participant);
        return -1;
    \}
    [set t]_writer = [set t]DataWriter_narrow([set t]_DDSwriter);
    if ([set t]_writer == NULL) \{
        printf(\"[set t] DataWriter narrow error\\n\");
        publisher_shutdown(participant);
        return -1;
    \}"
           } 
        }
        finalize {
           foreach t $topiclist {
             pts $fid "
    [set t]_instance = [set t]TypeSupport_create_data_ex(DDS_BOOLEAN_TRUE);
    if ([set t]_instance == NULL) \{
        printf(\"[set t]TypeSupport_create_data error\\n\");
        publisher_shutdown(participant);
        return -1;
    \}
    /* For data type that has key, if the same instance is going to be
       written multiple times, initialize the key here
       and register the keyed instance prior to writing */
    /*
    [set t]_instance_handle = [set t]DataWriter_register_instance(
        [set t]_writer, [set t]_instance);
    */"
           } 
        }
  }
}


proc parsepub { topiclist } {
global scriptdir
global NDDS_VERSION NDDSPUBLOOPSIZE
  set fin  [open $scriptdir/subsystem_publisher.c.template r]
  set basename [lindex [split [lindex $topiclist 0] _] 0]
  set fout [open [set basename]_shmem_publisher.c w]
  while { [gets $fin rec] > -1 } {
    if { [string trim $rec] == "/* Main loop */" } {
           addshmemcode $subsys $fout
           set i $NDDSPUBLOOPSIZE($NDDS_VERSION)
           while { $i > 0 } {gets $fin rec ; incr i -1}
    } else {
           if { [string range $rec 0 8] == "###INSERT" } {
              replaceshmemcode $topiclist [lindex $rec 1] $fout
           } else {
              puts $fout $rec
           }
    }
  }
  close $fin
  close $fout
}

proc parsemakefile { subsys } {
  set mf [glob makefile*i86Linux2.6gcc3.4.3]
  set fin [open $mf r]
  set fout [open makefile w]
  while { [gets $fin rec] > -1 } {
    if { [string range $rec 0 3] != "EXEC" } {
       puts $fout $rec
    } else {
       puts $fout "EXEC          = [set subsys]_subscriber \\
		[set subsys]_publisher \\
		[set subsys]_shmem_publisher \\
		[set subsys]_shmem_tester"
    }
  }
  close $fin 
  close $fout
}



set TYPESUBS(string) char
set TYPESUBS(int)    long
set TYPESUBS(short)  short
set TYPESUBS(long)   long
set TYPESUBS(byte)   byte
set TYPESUBS(float)  float
set TYPESUBS(double) double

set TYPESIZE(string) 1
set TYPESIZE(int)    4
set TYPESIZE(long)   4
set TYPESIZE(short)  2
set TYPESIZE(char)   1
set TYPESIZE(byte)   1
set TYPESIZE(float)  4
set TYPESIZE(double) 8

set NDDS_VERSION 4.2e
set NDDSPUBLOOPSIZE(4.2e) 17
set LVERSION i86Linux2.6gcc3.4.3
set basedir .

set all [glob $subsys*.idl]
set topiclist ""
foreach t [lsort $all] {
  lappend topiclist [file rootname $t]
}

set basename [lindex [split $subsys _] 0]
source $basedir/revCodes.tcl
set workdir $basedir/shmem-$basename
exec mkdir -p $workdir


puts stdout "Processing $subsys"
cd $workdir
doddsgen $topiclist
shcoder $subsys
generatetcllib $subsys

