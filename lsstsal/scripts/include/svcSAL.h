/////////////////////////////////////////////////////////////
//
// svcSAL.h:  Interface file for the svcSAL class.
//
// Copyright (c) 2006 NOAO.
//
/////////////////////////////////////////////////////////////


#if !defined(svcSAL__INCLUDED_)
#define svcSAL__INCLUDED_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
 
typedef unsigned int 	svcUINT;   
typedef int 		svcINT;  
typedef unsigned short 	svcUSHORT;
typedef short 		svcSHORT;
typedef float 		svcFLOAT;
typedef double 		svcDOUBLE;
typedef unsigned char 	svcUCHAR;
typedef char 		svcCHAR;

// Generic return codes
#define SAL__OK   		 0
#define SAL__ERR 		-1
#define SAL__ILLEGAL_REVCODE 	-2
#define SAL__TOO_MANY_HANDLES   -3
#define SAL__NOT_DEFINED        -4

// Timeout return codes
#define SAL__TIMEOUT            -5
#define SAL__SIGNAL_INTERRUPT   -6

// getItem timeout specifiers (+ve is a time in microseconds)
#define SAL__WAIT_FOR_NEXT_UPDATE -10000
#define SAL__WAIT_FOR_CHANGE      -10001



// telemetry stream update types
#define SAL__NO_UPDATES		-7
#define SAL__WAITING_FOR_NEXT 	1
#define SAL__GOT_UPDATE 	2
#define SAL__SYNC_IN    	3
#define SAL__SYNC_OUT   	4
#define SAL__SYNC_SET   	5
#define SAL__SYNC_CLEAR 	6
#define SAL__SYNC_READ  	7

// generateAlert types
#define SAL__WARN_ALERT   	1
#define SAL__ERROR_ALERT  	2
#define SAL__ABORT_ALERT 	-8

// issueCommand/getResponse return codes
#define SAL__CMD_ACK        	1
#define SAL__CMD_INPROGRESS 	2
#define SAL__CMD_STALLED    	3
#define SAL__CMD_COMPLETE   	4
#define SAL__CMD_NOPERM    	-9
#define SAL__CMD_NOACK     	-10
#define SAL__CMD_FAILED    	-11
#define SAL__CMD_ABORTED   	-12
#define SAL__CMD_TIMEOUT   	-13

#define svcDebugOutputString(__X__) fprintf(stderr,__X__)
#define SVC__MAX_DBGMSG_SIZE   1024
#define SAL__MAX_HANDLES       128
#define SAL__MAX_NAMESIZE      128


typedef struct svcSAL_command
{
    int     cppDummy;
    int     syncI;
    int     syncO;
    char    private_revCode[32];
    int     private_sndStamp;
    int     private_rcvStamp;
    int     private_seqNum;
    int     private_origin;
    long    cmdID;
    char    device[16];
    char    property[16];
    char    action[16];
    char    value[16];
    char    modifiers[128];
} svcSAL_command;
 
typedef struct svcSAL_response
{
    int      cppDummy;
    int      syncI;
    int      syncO;
    char     private_revCode[32];
    int      private_sndStamp;
    int      private_rcvStamp;
    int      private_seqNum;
    int      private_origin;
    long     cmdID;
    char     ack[16];
    char     error[16];
    long     timeout;
    short    repeat;
    short    submits;
    char     result[64];
} svcSAL_response;

typedef struct svcSAL_tlmhdr_cache {
    int      cppDummy;
    int      syncI;
    int      syncO;
    char     private_revCode[32];
    int      private_sndStamp;
    int      private_rcvStamp;
    int      private_seqNum;
    int      private_origin;
} svcSAL_tlmhdr_cache;

typedef struct svcSAL_cachehandle {
    char     streamname[128];
    long     shmid;
    size_t   shmsize;
    svcSAL_tlmhdr_cache      *ref;
} svcSAL_cachehandle;

/* svcSAL_cachehandle svcSAL_handle[SAL__MAX_HANDLES]; */


#define SAL__COMMAND_SIZE sizeof(svcSAL_command)
#define SAL__RESPONSE_SIZE sizeof(svcSAL_response)

/*     
  int (*commandCallback)(char *subsystem, svcSAL_command *shmcmd_ref, svcSAL_response *shmresp_ref ) = NULL;
  int (*datastreamCallback)(char *subsystem, int *shmdata_ref ) = NULL;
*/


int svcSAL_initialize ();
int svcSAL_connect ( char *name );
int svcSAL_connect1 ( char *name, svcSAL_cachehandle *handle );
int svcSAL_accessSync ( int handle , int direction, int optype );
int svcSAL_accessPrivate ( int handle , char *operation, char *revCode , 
                           long *sndStamp, long *rcvStamp,
                           long *seqNum  , long *origin );
int svcSAL_test_datastreamCallback (char *subsystem, int *shmdata_ref );
int svcSAL_test_commandCallback (char *subsystem, 
                                 svcSAL_command *shmcmd_ref, 
                                 svcSAL_response *shmresp_ref);
int svcSAL_sleep(unsigned long milisec) ; 
int svcSAL_nsleep(unsigned long nanosec) ; 
int svcSAL_shmProperties ( char *subsys, int *streamid, int *size );
 
#ifdef SAL__SIMPLECLASS
class svcPubSub
{
public:
        svcPubSub();
        ~svcPubSub();
	svcINT setDebugLevel( svcUINT level );
        svcINT reSynchronize();

        svcINT getItem( svcCHAR *itemName , svcCHAR *textValue);
        svcINT getItem( svcCHAR *itemName , svcINT timeout, svcCHAR *textValue);
        svcINT putItem( svcCHAR *itemname , svcCHAR *textValue);

        svcINT getItem( svcCHAR *itemName , svcINT *intValue);
        svcINT getItem( svcCHAR *itemName , svcINT timeout , svcINT *intValue);
        svcINT putItem( svcCHAR *itemname , svcINT intValue);

        svcINT getItem( svcCHAR *itemName , svcUINT *intValue);
        svcINT getItem( svcCHAR *itemName , svcINT timeout , svcUINT *intValue);
        svcINT putItem( svcCHAR *itemname , svcUINT uintValue);

        svcINT getItem( svcCHAR *itemName , svcSHORT *intValue );
        svcINT getItem( svcCHAR *itemName , svcINT timeout, svcSHORT *intValue);
        svcINT putItem( svcCHAR *itemname , svcSHORT intValue);

        svcINT getItem( svcCHAR *itemName , svcUSHORT *intValue);
        svcINT getItem( svcCHAR *itemName , svcINT timeout , svcUSHORT *intValue);
        svcINT putItem( svcCHAR *itemname , svcUSHORT intValue);

        svcINT getItem( svcCHAR *itemName , svcFLOAT *fValue);
        svcINT getItem( svcCHAR *itemName , svcINT timeout , svcFLOAT *fValue);
        svcINT putItem( svcCHAR *itemname , svcFLOAT fValue);

        svcINT getItem( svcCHAR *itemName , svcDOUBLE *dValue);
        svcINT getItem( svcCHAR *itemName , svcINT timeout , svcDOUBLE *dValue);
        svcINT putItem( svcCHAR *itemname , svcDOUBLE dValue);

        svcINT domainID = 0;
        void *currentInstance= NULL;
        svcINT newData = SAL__NO_UPDATES;
        svcINT timeOfSnd = 0;
        svcINT timeOfRcv = 0;
};

class svcAlert
{
public:
        svcAlert();
        ~svcAlert();
        svcINT generateAlert( svcINT severity , svcCHAR *details );
        svcINT catchAlerts( svcCHAR *subsystemName , svcINT *callback());
};

    char    operation[16];

class svcCommand
{
public:
        svcCommand();
        ~svcCommand();
        svcINT issueCommand( svcCHAR *subsystemName , 
                             svcCHAR *device , 
                             svcCHAR *property , 
                             svcCHAR *action , 
                             svcCHAR *value , 
                             svcCHAR *modifiers);
        svcINT checkCommand( svcINT cmdSeqNum );
        svcINT cancelCommand( svcINT cmdSeqNum );
        svcINT abortCommand( svcINT cmdSeqNum );
        svcINT waitForCompletion( svcINT cmdSeqNum , svcINT timeout );
	svcINT setDebugLevel( svcUINT level );

};

#endif

#endif
