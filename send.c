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
#include <pthread.h>
 
//#define SERVER "127.0.0.1"
#define SERVER "192.168.0.20"
#define BUFLEN 1500    //Max length of buffer
#define PORT 8888   //The port on which to send data
#define MAXIMUM 100000   //The port on which to send data

 
#define PACKETS 1000  //The port on which to send data
#define MSGIDLENGTH 10
pthread_mutex_t mutex; 
struct Packet {
  char data[MSGIDLENGTH];
};	

struct queue {
   int qsize,qelements;
   int head,tail;
   struct Packet *qdata;
};

struct queue* initcq(int size) {
   struct queue *cq=malloc(sizeof(struct queue));
   if(!cq)return NULL;
   cq->qsize=size;
   cq->qelements=0;
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
   return( ((q->tail+1)%q->qsize==q->tail) || (q->qsize==q->qelements) );
}

int  enQueue(struct queue *cq, struct Packet pkt) {
   if(isfullqueue(cq)){
//      printf("queue overflow\n");
      return 1;
   }
   else{
//      printf("\n Top of enQueue head: %d tail= %d, elem = %d", cq->head, cq->tail, cq->qelements); 
       pthread_mutex_lock(&mutex);
      cq->tail=(cq->tail+1)%cq->qsize;
      cq->qdata[cq->tail]=pkt;
      if(cq->head==-1) {
         cq->head=cq->tail;
      }
   
      (cq->qelements++);
      pthread_mutex_unlock(&mutex);
//      printf("\n End of enQueue head: %d tail= %d, elem = %d", cq->head, cq->tail, cq->qelements); 
      return 0;
   }
}

int deQueue(struct queue *cq, struct Packet *pkt) {
    
   //printf("\ndeQueue!!!\n");
   if(isemptyqueue(cq)) {
      printf("queue underflow");
      return 1;
   }
   else {
      //printf("\n Top of deQueue head: %d tail= %d, elem = %d", cq->head, cq->tail, cq->qelements); 
     // printf("\ndeQueue%d, %d \n", pkt,cq->head);
      pthread_mutex_lock(&mutex);
      *pkt =cq->qdata[cq->head];
      if(cq->head==cq->tail)
         cq->head=cq->tail=-1;
      else
         cq->head=(cq->head+1)%cq->qsize;
   }
   (cq->qelements)--;
  pthread_mutex_unlock(&mutex); 
      //printf("\n End of deQueue head: %d tail= %d, elem = %d", cq->head, cq->tail, cq->qelements); 
   

//      printf("\ndeQueue%d, %d \n", pkt,cq->head);
   return 0;
}


void *  populateQueue(void *cq) {
//void populateQueue(struct queue *cq,int p) {
   int j;
   int p = PACKETS;
   int elementAdd= 1;
   char msg[MSGIDLENGTH];
   struct queue *cqt= (struct queue *) cq;
   for (j=0;j<p;j++){
      sprintf(msg, "%010d", j);
      struct Packet newPacket;
      strncpy(newPacket.data,msg,MSGIDLENGTH+1);
      elementAdd=enQueue(cqt,newPacket);
      if (elementAdd){
         printf("\n  Mmsg value : %s loc= %d, qsize%d", msg,cqt->tail ,cqt->qelements);
      }
   }
      int column1 = sizeof(msg);
      
     // sleep (1);
      while (1){
      if (!isfullqueue(cqt)){
      struct Packet newPacket;
      sprintf(msg, "%010d", j);
      
             strncpy(newPacket.data,msg,MSGIDLENGTH+1);
             elementAdd=enQueue(cqt,newPacket);
             if (elementAdd==0){
                 printf("\n  Mmsg value : %s loc= %d, qsize%d", msg,cqt->tail ,cqt->qelements);
            }
      j++;
    //  usleep (1);
                  
      }
      else {
      usleep (500);
     // printf("\n\n\n FULLLLLLLLLLLLLLLLLLLLLLLLLLLLLL TH2..TH2..TH2..TH2..TH2.TH2..TH2..TH2..Th2 \n\n");
      }
     }
      printf("\n\n\n FULLLLLLLLLLLLLLLLLLLLLLLLLLLLLL TH2..TH2..TH2..TH2..TH2.TH2..TH2..TH2..Th2 \n\n");
      pthread_exit(NULL);
      }


char *  readQueue(struct queue *cq, struct Packet *pkt) {
   static  char  msg[MSGIDLENGTH+1];
   //printf("\nreadQueue!!!\n");
   //  struct Packet *pkt;
   int x=deQueue(cq,pkt);
   //printf("\nreadQueue!!!\n");
   if(x == 1){
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
      //printf("\n readQueue head: %d tail= %d", cq->head, cq->tail); 
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
   int totalMsgSent =0;

   char * msgId;

   struct queue * cq = initcq(PACKETS);


   for (j=0;j<paddingSize;j++){
       paddings[j]= '#';
      
   }
    

  // populateQueue(cq,PACKETS);
  // populateQueue((void *)cq); 
  pthread_mutex_init(&mutex, NULL);
  
  pthread_t enq_thread;

  if(pthread_create(&enq_thread, NULL, populateQueue, cq)) {

  fprintf(stderr, "Error creating thread\n");
  return 1;

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
  // while( (!isemptyqueue(cq)) &&  (totalMsgSent<MAXIMUM) )
   while( (!isemptyqueue(cq)) )
   {
   //msgSent = (char*)malloc(strlen(msgId)+sizeof(paddings)+1);//+1 for the zero-terminator
  
   //printf("TEST1 rear=  %d front= %d\n", cq->tail,cq->head );

   msgId = readQueue(cq,pkt);

   msgSent= concat(msgId,paddings);
   if (sendto(s, msgSent, sizeof(msgSent) , 0 , (struct sockaddr *) &si_other, slen)==-1) {
         die("sendto()");
      }
    //free(msgSent);
    totalMsgSent ++;
   printf("\nTEST:queue msgId= %s , %d ,%d , sizeof array = %d, qelements= %d , totalMsgSent= %d",msgId,strlen(msgSent),strlen(paddings), sizeof(*cq),cq->qelements,totalMsgSent); 
   printf("\nTEST1 rear=  %d front= %d\n", cq->tail,cq->head );
    free(msgSent);
   }

   clock_t end = clock();
   double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

   gettimeofday(&ends, NULL);
   double delta = ((ends.tv_sec  - starts.tv_sec) * 1000000u +
   ends.tv_usec - starts.tv_usec) / 1.e6;
   printf("\nTime spent : %f to senn %d\n",delta, totalMsgSent);

   printf ("\nCPU time spent: %1f   at  %d clocks per second \n",time_spent,CLOCKS_PER_SEC); 
   close(s);
   printf("TEST1 rear=  %d front= %d\n", cq->tail,cq->head );
   //printf("\nTEST2:queue msgId= %s , %d ,%d , sizeof array = %d, qelements= %d , totalMsgSent= %d",msgId,strlen(msgSent),strlen(paddings), sizeof(*cq),cq->qelements,totalMsgSent); 
//   pthread_join(enq_thread, NULL);
   return 0;
}
