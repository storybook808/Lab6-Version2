#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "utilities.h"
#include "link.h"
#include "switch.h"

#define TENMILLISEC 10000   /* 10 millisecond sleep */

#define debug

/* 
 * switchInit initializes the switch.  It calls
 * - switchInitState which initializes the host's state.
 * - switchInitRcvPacketBuff, which initializes the receive packet buffer
 * - switchInitSendPacketBuff, which initializes the send packet buffer
 */
void switchInitState(switchState * sstate, int physid); 

/*
 * Functions
 */

/* 
 * Main loop of the switch node
 *
 * Polls any incoming links and downloads any
 * incoming packets to its receive packet buffer
 *
 * Then it sleeps for 10 milliseconds
 *
 * Then back to the top of the loop
 *
 */
void switchMain(switchState * sstate)
{
   int size;
   int  i;
   int  j;
   int  target;
   int  forwardTable[MAXPORT];
   packetBuffer tmpbuff[MAXPORT];
   networkInfo A;

   /* Clear Forwarding Table */
   for (i=0; i<MAXPORT; i++){
      forwardTable[i] = -1;
      tmpbuff[i].valid = 0;
      tmpbuff[i].new = 0;
      A.child[i] = -1;
   }

   A.leader = sstate->physid;
   A.distance = 0;
   A.parent = -1;

   while(1) {
      
      /* Scroll through the different ports scanning for a new packet. */
      for (i=0; i<MAXPORT; i++) {
         size = linkReceive(&(sstate->linkin[i]), &tmpbuff[i]);
	 /* New packet detected. */
	 if (tmpbuff[i].valid == 1 && tmpbuff[i].new == 1) {
	    /* Save address onto forwarding table. */
	    forwardTable[i] = tmpbuff[i].srcaddr;
	    
	   if (tmpbuff[i].dstaddr != -2) {



	    /* Initalize target to empty. */
	    target = -1;

	    /* Sweep forwarding table for stored address. */
	    for (j=0; j<MAXPORT; j++) {
	       
	       /* If there was a match, replace target with connected port. */
               if (forwardTable[j] == tmpbuff[i].dstaddr) {
                  target = j;
	       }
	    } 
	    /* When target was found, transmit to correct port. */
	    if (target != -1) {
               linkSend(&(sstate->linkout[target]), &tmpbuff[i]);
	    } 
	    
	    /* Otherwise, mass transmit message to all other ports. */
	    else {
               for(j=0; j<MAXPORT; j++) {
	          if(i!=j)
                     linkSend(&(sstate->linkout[j]), &tmpbuff[i]);

		  /* Sleep in between each message. */
		  usleep(TENMILLISEC);
	       }
	       usleep(TENMILLISEC);
	    }
	    /* Message reset. */
	    tmpbuff[i].valid = 0;
	    tmpbuff[i].new = 0;

	    }
            
	    else
	    {
	    	// if new leader is smaller than current leader
	    	if (tmpbuff[i].length < A.leader)
	    	{
			// replace parent the incoming port number
               		A.parent = i;

			// update distance from root leader
			A.distance = tmpbuff[i].type + 1;

			// replace leader information
			A.leader = tmpbuff[i].length;
	       	}

		if (tmpbuff[i].payload[0] == 'y')
		{
			A.child[i] = 1;
		}
		// if leader are the same, and this node isn't the leader
               	/*
		else if (tmpbuff[i].length == A.leader && A.leader != sstate->physid)
	       	{
			// and distance to the leader is smaller than current path
	         	if(tmpbuff[i].type + 1 < A.distance)
		  	{
                     		A.parent = i;
		     		A.distance = tmpbuff[i].type + 1;
		  	}
	       	}
		*/
	    	tmpbuff[i].valid = 0;
	    	tmpbuff[i].new = 0;
	    }
         }

	 tmpbuff[0].dstaddr = -2;
	 tmpbuff[0].srcaddr = -2;
	 tmpbuff[0].length = A.leader;
         tmpbuff[0].type = A.distance;
	 tmpbuff[0].valid = 1;
         for(j=0; j<MAXPORT; j++)
	 {
	    if(j == A.parent)
	    {
	       tmpbuff[0].payload[0] = 'y';
	    }
	    else
	    {
	       tmpbuff[0].payload[0] = 'n';
	    }
            linkSend(&(sstate->linkout[j]), &tmpbuff[0]);
	    usleep(TENMILLISEC);
         }
         tmpbuff[0].valid = 0;
printf("ID: %d Leader: %d Distance: %d Parent: %d child[0]: %d child [1]: %d\n\n\n", sstate->physid, A.leader, A.distance, A.parent, A.child[0], A.child[1]);

      } /* End of for-loop. */
   } /* End of while-loop. */
}

/* 
 * Initializes the switch.   
 */
void switchInit(switchState * sstate, int physid) {
   switchInitState(sstate, physid);     /* Initialize host's state */
}

/* 
 * Initialize the state of the host 
 */
void switchInitState(switchState * sstate, int physid) {
   sstate->physid = physid;
}
