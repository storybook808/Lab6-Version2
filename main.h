#define PAYLOAD_LENGTH 200 /* Maximum payload size */
#define LOCAL_MAX      2000

typedef struct { /* Packet buffer */
   int srcaddr;  /* Source address */
   int dstaddr;  /* Destination addres */
   int length;   /* Length of packet */
   char payload[PAYLOAD_LENGTH + 1];  /* Payload section */
   int valid;   /* Indicates if the contents is valid */ 
   int new;     /* Indicates if the contents has been downloaded */
   //int standby;    /* Indicates if there are more packets on the way. */
   char  test;
   char test1;
   char test2;
   char test3;
   char test4;
   char test5;
   char test6;
   char test7;
   char test8;
   char test9;
   char test10;



   int type;
} packetBuffer;

typedef struct {
   int  srcaddr;
   int  dstaddr;
   int  length;
   char payload[LOCAL_MAX+1];
   int  valid;
   int  new;
} localBuffer;

   


