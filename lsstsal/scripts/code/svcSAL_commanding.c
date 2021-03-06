

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/time.h>
#include "svcSAL.h"
#include "system_Command_history_cache.h"

extern svcSAL_cachehandle svcSAL_handle[SAL__MAX_HANDLES];
int cmdshmid=-1;
int respshmid=-1;
int pingshmid=-1;
int shmcmd_size=SAL__COMMAND_SIZE;
int shmresp_size=SAL__RESPONSE_SIZE;
svcSAL_command  *shmcmd_ref;
svcSAL_response *shmresp_ref;

int svcSAL_receiveCommand (char *subsystem, 
                           int timeout, 
                           int (*commandCallback)(char *subsystem, 
                                 svcSAL_command  *shmcmd_ref, 
                                 svcSAL_response *shmresp_ref ) ) {

   int status;
   int cmdHandle;
   int respHandle;
   int pingHandle;
   int historyHandle;
   int deltams;
   long clkdrift;
   svcSAL_command  *shmcmd_ref;
   svcSAL_response *shmresp_ref;
   system_Command_history_cache *shmhistory_ref;
   struct timeval now1,now2;
   struct timezone zone;
   char cmdstream[64];
   char respstream[64];
   char pingstream[64];

   sprintf(cmdstream,"%s_command",subsystem);
   sprintf(respstream,"%s_response",subsystem);

   cmdHandle = svcSAL_connect (cmdstream);
   if ( cmdHandle <= 0 ) {
       printf("svcSAL_receiveCommand : %s not defined\n",cmdstream);
       return SAL__NOT_DEFINED;
   }
   respHandle = svcSAL_connect (respstream);
   if ( respHandle <= 0 ) {
       printf("svcSAL_receiveCommand : %s not defined\n",respstream);
       return SAL__NOT_DEFINED;
   }

   historyHandle = svcSAL_connect ( "system_Command_history" );
   if ( historyHandle <= 0 ) {
        printf("SAL command processor : system_Command_history not defined\n");
   }
   shmhistory_ref  = (system_Command_history_cache *) svcSAL_handle[historyHandle].ref;

   shmcmd_ref  = (svcSAL_command *) svcSAL_handle[cmdHandle].ref;
   shmresp_ref  = (svcSAL_response *) svcSAL_handle[respHandle].ref;

   gettimeofday(&now1, &zone);
   while (shmcmd_ref->syncI == 0) {
      svcSAL_sleep(10);
      if (timeout > 0) {
        gettimeofday(&now2, &zone);
        deltams = (now2.tv_sec*1000 + now2.tv_usec/1000)
                   - (now1.tv_sec*1000 + now1.tv_usec/1000);
        if (deltams > timeout) {
           return SAL__TIMEOUT ;
        }
      }
   }

   shmcmd_ref->private_rcvStamp = svcSAL_timestamp();
   clkdrift = shmcmd_ref->private_rcvStamp - shmcmd_ref->private_sndStamp;

   printf("%s command received : %d %s.%s %s deltat=%ld\n",subsystem,
             shmcmd_ref->cmdID,
             shmcmd_ref->device,
             shmcmd_ref->property,
             shmcmd_ref->action, clkdrift);

 
   shmresp_ref->cmdID = shmcmd_ref->cmdID;
   strcpy(shmresp_ref->ack,"SAL OK");
   shmcmd_ref->syncI = 0;
   status = SAL__OK;

   if ( strcmp(shmcmd_ref->property,"ping" ) == 0 ) {
      sprintf(pingstream,"%s_%s",subsystem,shmcmd_ref->device);
      pingHandle = svcSAL_connect (pingstream);
      if ( pingHandle == 0 ) {
         return SAL__NOT_DEFINED ;
      }
      svcSAL_accessSync(pingHandle, SAL__SYNC_OUT, SAL__SYNC_SET);
      return SAL__OK ;
   }

   if (historyHandle > 0) {
      shmhistory_ref->syncO = 1;
   }

   if ( commandCallback != NULL ) {
      status = (*commandCallback)(subsystem, 
                                  (svcSAL_command *)shmcmd_ref, 
                                  (svcSAL_response  *)shmresp_ref ) ;
   }
   shmresp_ref->syncO = 1;

   return status ;
}


int svcSAL_test_commandCallback (char *subsystem, svcSAL_command *shmcmd_ref, svcSAL_response *shmresp_ref ) {
   printf("%s command received : cmdID=%d\n	device=%s\n	property=%s\n	action=%s\n	value=%s\n	modifiers=%s\n",subsystem,
             shmcmd_ref->cmdID,
             shmcmd_ref->device,
             shmcmd_ref->property,
             shmcmd_ref->action,
             shmcmd_ref->value,
             shmcmd_ref->modifiers);
   strcpy(shmresp_ref->ack,"Received\n");
   return SAL__OK;
}


int svcSAL_receiveResponse (char *subsystem, 
                           int timeout, 
                           int (*responseCallback)(char *subsystem, 
                                 svcSAL_command  *shmcmd_ref, 
                                 svcSAL_response *shmresp_ref ) ) {

   int status;
   int cmdHandle;
   int respHandle;
   int historyHandle;
   int deltams;
   long clkdrift;
   svcSAL_command  *shmcmd_ref;
   svcSAL_response *shmresp_ref;
   system_Command_history_cache *shmhistory_ref;
   struct timeval now1,now2;
   struct timezone zone;
   char cmdstream[64];
   char respstream[64];

   sprintf(cmdstream,"%s_command",subsystem);
   sprintf(respstream,"%s_response",subsystem);

   cmdHandle = svcSAL_connect (cmdstream);
   if ( cmdHandle == 0 ) {
      return SAL__NOT_DEFINED;
   }
   respHandle = svcSAL_connect (respstream);
   if ( respHandle == 0 ) {
      return SAL__NOT_DEFINED;
   }

   historyHandle = svcSAL_connect ( "system_Command_history" );
   if ( historyHandle == 0 ) {
        printf("SAL command processor : system_Command_history not defined\n");
   }
   shmhistory_ref  = (system_Command_history_cache *) svcSAL_handle[historyHandle].ref;

   shmcmd_ref  = (svcSAL_command *) svcSAL_handle[cmdHandle].ref;
   shmresp_ref  = (svcSAL_response *) svcSAL_handle[respHandle].ref;

   gettimeofday(&now1, &zone);
   while (shmcmd_ref->syncI == 0) {
      svcSAL_sleep(10);
      if (timeout > 0) {
        gettimeofday(&now2, &zone);
        deltams = (now2.tv_sec*1000 + now2.tv_usec/1000)
                   - (now1.tv_sec*1000 + now1.tv_usec/1000);
        if (deltams > timeout) {
           return SAL__TIMEOUT ;
        }
      }
   }

   shmcmd_ref->private_rcvStamp = svcSAL_timestamp();
   clkdrift = shmresp_ref->private_rcvStamp - shmresp_ref->private_sndStamp;

   printf("%s response received : %d %s %s %s deltat=%ld\n",subsystem,
             shmresp_ref->cmdID,
             shmresp_ref->ack,
             shmresp_ref->error,
             shmresp_ref->result, clkdrift);

 
   status = SAL__OK;


   if (historyHandle > 0) {
      shmhistory_ref->syncO = 1;
   }

   if ( responseCallback != NULL ) {
      status = (*responseCallback)(subsystem, 
                                  (svcSAL_command *)shmcmd_ref, 
                                  (svcSAL_response  *)shmresp_ref ) ;
   }
   shmresp_ref->syncI = 0;

   return status ;
}



int svcSAL_test_responseCallback (char *subsystem, svcSAL_command *shmcmd_ref, svcSAL_response *shmresp_ref ) {
   printf("%s response received : cmdID=%d\n	device=%s\n	property=%s\n	action=%s\n	ack=%s\n	error=%s\n	result=%s\n	timeout=%d\n	repeat=%d\n",subsystem,
             shmresp_ref->cmdID,
             shmcmd_ref->device,
             shmcmd_ref->property,
             shmcmd_ref->action,
             shmresp_ref->ack,
             shmresp_ref->error,
             shmresp_ref->result,
             shmresp_ref->timeout,
             shmresp_ref->repeat);
   return SAL__OK;
}




