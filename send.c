/*
    Simple udp client
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0)
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<sys/time.h>
 
//#define SERVER "127.0.0.1"
#define SERVER "192.168.0.20"
#define BUFLEN 1500    //Max length of buffer
#define PORT 8888   //The port on which to send data

 
#define PACKETS 1000000  //The port on which to send data
#define MSGIDLENGTH 10

struct Packet {
  char data[MSGIDLENGTH];
};	

struct queue {
   int qsize;
   int head,tail;
   struct Packet *qdata;
};

struct queue* initcq(int size) {
   struct queue *cq=malloc(sizeof(struct queue));
   if(!cq)return NULL;
   cq->qsize=size;
   cq->head=-1;
   cq->tail=-1;
   cq->qdata=malloc(cq->qsize*sizeof(struct Packet));
   if(!cq->qdata)return NULL;
   return cq;
}

void die(char *s)
{
   perror(s);
   exit(1);
}

char* concat(const char *s1, const char *s2) {
   char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
   //in real code you would check for errors in malloc here
   strcpy(result, s1);
   strcat(result, s2);
   return result;
}

int isemptyqueue(struct queue *cq) {
   return(cq->head==-1);
}

int isfullqueue(struct queue *q) {
   return((q->tail+1)%q->qsize==q->tail);
}

void enQueue(struct queue *cq, struct Packet pkt) {
   if(isfullqueue(cq))
      printf("queue overflow\n");
   else{
      cq->tail=(cq->tail+1)%cq->qsize;
      cq->qdata[cq->tail]=pkt;
      if(cq->head==-1) {
         cq->head=cq->tail;
      }
   }
}

int deQueue(struct queue *cq, struct Packet *pkt) {
    
   //printf("\ndeQueue!!!\n");
   if(isemptyqueue(cq)) {
      printf("queue underflow");
      return -1;
   }
   else {
     // printf("\ndeQueue%d, %d \n", pkt,cq->head);
      *pkt =cq->qdata[cq->head];
      if(cq->head==cq->tail)
         cq->head=cq->tail=-1;
      else
         cq->head=(cq->head+1)%cq->qsize;
   }

//      printf("\ndeQueue%d, %d \n", pkt,cq->head);
   return 0;
}


void populateQueue(struct queue *cq,int p) {
   int j;
   char msg[MSGIDLENGTH];
   for (j=0;j<p;j++){
      sprintf(msg, "%010d", j);
      struct Packet newPacket;
      strncpy(newPacket.data,msg,MSGIDLENGTH+1);
      enQueue(cq,newPacket);
      int column1 = sizeof(msg);
      //printf("\n entred msg value : %s len= %d", msg, column1);

      }
}


char *  readQueue(struct queue *cq, struct Packet *pkt) {
   static  char  msg[MSGIDLENGTH+1];
   //printf("\nreadQueue!!!\n");
   //  struct Packet *pkt;
   int x=deQueue(cq,pkt);
   //printf("\nreadQueue!!!\n");
   if(x == -1){
     // printf("\nreadQueue!!!\n");
      printf("\nQueue is Empty!!!\n");
      strncpy(msg,"E000000000",MSGIDLENGTH); 
      return msg;
   }
   else{
      //printf("\n Read head value : %s", pkt->data);
      strncpy(msg,pkt->data,MSGIDLENGTH);
      //printf("\n Read msg value : %s", msg); 
      int column1 = sizeof(msg);
      //printf("\n Read msg value : %s len= %d", msg, column1); 
      return msg;
   }
}

int main(void) {

   struct Packet *pkt;
   struct sockaddr_in si_other;
   int s, i,j,x, slen=sizeof(si_other);
   int queueFront = -1;
   int queueRear= -1;
   int paddingSize = BUFLEN-MSGIDLENGTH;
   char message2[PACKETS][MSGIDLENGTH];
   char * msgSent;
   char paddings[paddingSize];
   clock_t begin = clock();
   struct timeval starts, ends;

   char * msgId;

   struct queue * cq = initcq(PACKETS);
   populateQueue(cq,PACKETS);


   for (j=0;j<paddingSize;j++){
       paddings[j]= '#';
      
   }
    

   queueFront =0;  // first item
   for (j=0;j<PACKETS;j++){
    }
    
   if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
   {
      die("socket");
   }
 
   memset((char *) &si_other, 0, sizeof(si_other));
   si_other.sin_family = AF_INET;
   si_other.sin_port = htons(PORT);
     
   if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
   {
       fprintf(stderr, "inet_aton() failed\n");
       exit(1);
   }

   printf("Sending %d  packets.\n",PACKETS);
   gettimeofday(&starts, NULL);

   msgSent = (char*)malloc(strlen(msgId)+strlen(paddings)+1);//+1 for the zero-terminator
   if(msgSent == NULL)
   {
    printf("Memory allocation failed");
      return;
   }
  //  while(x<PACKETS)
   while(!isemptyqueue(cq))
   {
   //msgSent = (char*)malloc(strlen(msgId)+sizeof(paddings)+1);//+1 for the zero-terminator
  
   //printf("TEST1 rear=  %d front= %d\n", cq->tail,cq->head );

   msgId = readQueue(cq,pkt);
   //printf ("\nqueue 2   idl=%d,sentl=%d,padl=%d,pads=%d,msg=%s, pad=%s\n", strlen(msgId) ,strlen(msgSent),strlen(paddings),sizeof(paddings), msgSent, paddings);
   // in real code you would check for errors in malloc here
   //strncpy(msgSent,msgId,strlen(msgId));

   //strcat(msgSent, paddings);
   msgSent= concat(msgId,paddings);
   if (sendto(s, msgSent, sizeof(msgSent) , 0 , (struct sockaddr *) &si_other, slen)==-1) {
         die("sendto()");
      }
    //free(msgSent);
   printf("\nTEST4:queue msgId= %s , %d ,%d , value: ",msgId,strlen(msgSent),strlen(paddings)); 
    //free(msgSent);
   }

   clock_t end = clock();
   double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

   gettimeofday(&ends, NULL);
   double delta = ((ends.tv_sec  - starts.tv_sec) * 1000000u +
   ends.tv_usec - starts.tv_usec) / 1.e6;
   printf("\nTime spent : %f\n",delta);

   printf ("\nCPU time spent: %1f   at  %d clocks per second \n",time_spent,CLOCKS_PER_SEC); 
   close(s);
   return 0;
}
