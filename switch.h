/* 
 * switch.h 
 */

#define NUMSWITCH 1
#define MAXPORT 100 

typedef struct {                  /* State of host */
   int   physid;                  /* physical id */
   int   rcvflag;
   packetBuffer sendPacketBuff;   /* send packet buffer */
   packetBuffer rcvPacketBuff;    /* receive packet buffer */
   LinkInfo linkin[MAXPORT];      /* Incoming communication link */
   LinkInfo linkout[MAXPORT];     /* Outgoing communication link */
} switchState;

void switchMain(switchState * sstate);

void switchInit(switchState * sstate, int physid);

