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
   int  i;
   int  j;
   int  target;
   int  forwardTable[MAXPORT];
   packetBuffer tmpbuff[MAXPORT];

   /* Clear Forwarding Table */
   for (i=0; i<MAXPORT; i++)
      forwardTable[i] = -1;

   while(1) {
      
      /* Scroll through the different ports scanning for a new packet. */
      for (i=0; i<MAXPORT; i++) {
         linkReceive(&(sstate->linkin[i]), &tmpbuff[i]);

	 /* New packet detected. */
	 if (tmpbuff[i].valid == 1 && tmpbuff[i].new == 1) {

	    /* Save address onto forwarding table. */
	    forwardTable[i] = tmpbuff[i].srcaddr;
	    
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
	    }
         }
	 /* Sleep in between each linkreceive. */
	 usleep(TENMILLISEC);
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
