/* 
 * switch.h 
 */

#define NUMSWITCH 4
#define MAXPORT 5

typedef struct {              /* State of host. */
   int      physid;           /* Physical id. */
   LinkInfo linkin[MAXPORT];  /* Incoming communication link. */
   LinkInfo linkout[MAXPORT]; /* Outgoing communication link. */
} switchState;

typedef struct
{
   int leader;
   int distance;
   int parent;
} networkInfo;

void switchMain(switchState * sstate);

void switchInit(switchState * sstate, int physid);

