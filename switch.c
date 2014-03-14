/* 
 * This is the source code for the host.  
 * hostMain is the main function for the host.  It is an infinite
 * loop that repeatedy polls the connection from the manager and
 * its input link.  
 *
 * If there is command message from the manager,
 * it parses the message and executes the command.  This will
 * result in it sending a reply back to the manager.  
 *
 * If there is a packet on its incoming lik, it checks if
 * the packet is destined for it.  Then it stores the packet
 * in its receive packet buffer.
 *
 * There is also a 10 millisecond delay in the loop caused by
 * the system call "usleep".  This puts the host to sleep.  This
 * should reduce wasted CPU cycles.  It should also help keep
 * all nodes in the network to be working at the same rate, which
 * helps ensure no node gets too much work to do compared to others.
 */

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

#define EMPTY_ADDR  0xffff  /* Indicates that the empty address */
                             /* It also indicates that the broadcast address */
#define MAXBUFFER 1000
#define PIPEWRITE 1 
#define PIPEREAD  0
#define TENMILLISEC 10000   /* 10 millisecond sleep */

#define debug

/* 
 * switchInit initializes the switch.  It calls
 * - switchInitState which initializes the host's state.
 * - switchInitRcvPacketBuff, which initializes the receive packet buffer
 * - switchInitSendPacketBuff, which initializes the send packet buffer
 */
void switchInitState(switchState * sstate, int physid); 
void switchInitRcvPacketBuff(packetBuffer * packetbuff);
void switchInitSendPacketBuff(packetBuffer * packetbuff);

/*
 * Functions
 */

/* 
 * Main loop of the host node
 *
 * It polls the manager connection for any requests from
 * the manager, and repliies
 *
 * Then it polls any incoming links and downloads any
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
      for (i=0; i<MAXPORT; i++) 
      {
         linkReceive(&(sstate->linkin[i]), &tmpbuff[i]);
	 if (tmpbuff[i].valid == 1 && tmpbuff[i].new == 1) 
	 {
	    forwardTable[i] = tmpbuff[i].srcaddr;
	    
	    /* target is empty */
	    target = -1;
	    for (j=0; j<MAXPORT; j++) 
	    {
               if (forwardTable[j] == tmpbuff[i].dstaddr) 
	       {
                  target = j;
	       }
	    } 
	    if (target != -1) 
	    {
               linkSend(&(sstate->linkout[target]), &tmpbuff[i]);
	    } 
	    else 
	    {
               for (j=0; j<MAXPORT; j++) 
	       {
                  linkSend(&(sstate->linkout[j]), &tmpbuff[i]);
		  usleep(TENMILLISEC);
	       }
	    }
         }
	 usleep(TENMILLISEC);
      }
      usleep(TENMILLISEC);
   } /* End of while loop */
}

/* 
 * Initializes the switch.   
 */
void switchInit(switchState * sstate, int physid)
{

   switchInitState(sstate, physid);     /* Initialize host's state */

   /* Initialize the receive and send packet buffers */
   switchInitRcvPacketBuff(&(sstate->rcvPacketBuff));  
   switchInitSendPacketBuff(&(sstate->rcvPacketBuff)); 
}

/* 
 * Initialize send packet buffer 
 */
void switchInitSendPacketBuff(packetBuffer * packetbuff)
{
   packetbuff->valid = 0;
   packetbuff->new = 0;
}

/* 
 * Initialize receive packet buffer 
 */ 
void switchInitRcvPacketBuff(packetBuffer * packetbuff) {
   packetbuff->valid = 0;
   packetbuff->new = 0;
}

/* 
 * Initialize the state of the host 
 */
void switchInitState(switchState * sstate, int physid) {
   sstate->physid = physid;
   sstate->rcvPacketBuff.valid = 0;
   sstate->rcvPacketBuff.new = 0;
}
