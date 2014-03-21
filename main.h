
#define PAYLOAD_LENGTH 210 /* Maximum payload size */
#define MAX_PAYLOAD 2000

typedef struct { /* Packet buffer */
   int srcaddr;  /* Source address */
   int dstaddr;  /* Destination addres */
   int length;   /* Length of packet */
   char payload[PAYLOAD_LENGTH + 1];  /* Payload section */
   int valid;   /* Indicates if the contents is valid */ 
   int new;     /* Indicates if the contents has been downloaded */
} packetBuffer;

   


