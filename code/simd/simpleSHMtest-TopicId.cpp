// -- STD C++ Include
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
using namespace std;

#include "svcSAL_TopicId_onlyshm.h"

int main(int argc, char* argv[]) 
{
        std::string op(argv[1]);
	svcRTN result = 0;
	svcUINT idim;
        salTopicId *salHandle;
// INSERT TOPIC_LOCALITEMS

	salHandle = (salTopicId *)new salTopicId();
	printf ("SAL timestamp is %lf\n", salHandle->getCurrentTime());
	result = salHandle->salConnect(SAL_SID_TopicId,op);

	if (result != SAL__OK) {
	   printf ("Topic initialization failed %d\n",result);
           exit(1);
	}
	
        while (1) {

	 	if (strcmp(argv[1],"publish")==0) {
// INSERT PUBLISH_SAMPLE
   		  sleep(5);
	  
       		} else { 
// INSERT READ_SAMPLE
		}
		usleep(SAL__FASTPOLL);
       }

}



