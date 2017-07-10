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
#define BUFLEN 3000    //Max length of buffer
#define PORT 8888   //The port on which to send data

 
#define PACKETS 100000  //The port on which to send data
#define MSGIDLENGTH 10

struct Packet
{
  char data[MSGIDLENGTH];
  struct Packet *next;
}*head = NULL,*tail = NULL;


void die(char *s)
{
    perror(s);
    exit(1);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void enQueue(char *value)
{
   struct Packet *newPacket;
   newPacket = (struct Packet*)malloc(1*sizeof(struct Packet));
 //  newPacket->data = value;
   strncpy(newPacket->data,value,MSGIDLENGTH+1);
   newPacket->next = NULL;
   if(head == NULL){
      head = tail = newPacket;
      printf("Empty Queue!");
   }  
   else{
      tail->next = newPacket;
      tail = newPacket;
   }
  // printf("\nInsertion is Success!!! value= %s\n",value);
//   printf("\nInsertion is Success!!! newPacket= %s\n",newPacket->data);
//  printf("\nInsertion is Success!!! newPacket= %s\n",newPacket->next);
int column1 = sizeof(newPacket->data);
//      printf("\n entred msg value : %s len= %d", newPacket->data, column1);
  // printf("\nInsertion is Success!!! tail-data= %s\n",tail->data);
  // printf("\nInsertion is Success!!! tail-next= %s\n",tail->next);
  // printf("\nInsertion is Success!!! head-data= %s\n",head->data);
  // printf("\nInsertion is Success!!! head-next= %s\n",head->next);
}

void populateQueue(int p)
{
   int j;
   char msg[MSGIDLENGTH];
      for (j=0;j<p;j++){
       //  message2[j]= j;
        // sprintf(msg[MSGIDLENGTH], "%10d", j);
         sprintf(msg, "%010d", j);
       //  queueRear = queueRear + 1;
       enQueue(msg);
 int column1 = sizeof(msg);
 //     printf("\n entred msg value : %s len= %d", msg, column1);

 //  printf("\npopulate!!! tail-data= %s\n",tail->data);
//   printf("\npopulate!!! tail-next= %s\n",tail->next);
//   printf("\npopulate!!! head-data= %s\n",head->data);
//   printf("\npopulate!!! head-next= %s\n",head->next);
      }
}


void deQueue()
{
  struct Packet *temp = head;
//  printf("\nHead element: %s\n", head->data);
   if(temp == NULL)
      printf("\nQueue is Empty!!!\n");
   else{
      if (temp->next != NULL){
         temp = temp->next;
        printf("\nDeleted element: %s\n", temp->data);
         free(head);
         head = temp;
      } else
      {
         printf("\n Dequed value : %s", temp->data);
         free(head);
         head = NULL;
         tail = NULL;
      }
     int column1 = sizeof(temp->data);
      printf("\n deleted msg value : %s len= %d", temp->data, column1);

   }
printf("exit deQueue");
}

  char *  readQueue()
{
 static  char  msg[MSGIDLENGTH+1];
   if(head == NULL){
      printf("\nQueue is Empty!!!\n");
      strncpy(msg,"E000000000",MSGIDLENGTH); 
      return msg;
      }
   else{
      printf("\n Read head value : %s", head->data);
      strncpy(msg,head->data,MSGIDLENGTH);
      printf("\n Read msg value : %s", msg); 
      deQueue();
   int column1 = sizeof(msg);
      printf("\n Read msg value : %s len= %d", msg, column1); 
      return msg;
   }


}

int main(void)
{
    struct sockaddr_in si_other;
    int s, i,j,x, slen=sizeof(si_other);
    int queueFront = -1;
    int queueRear= -1;
    int paddingSize = BUFLEN-MSGIDLENGTH;
  //  char buf[BUFLEN];
//    char message[PACKETS];
    char message2[PACKETS][MSGIDLENGTH];
    char * msgSent;
    char paddings[paddingSize];
    clock_t begin = clock();
    struct timeval starts, ends;

//Linked list
 //  char  msgId[MSGIDLENGTH];
   char * msgId;


   populateQueue(PACKETS);


    for (j=0;j<paddingSize;j++){
           paddings[j]= '#';
     // sprintf(message2[j], "%d", j);
	 //   printf ("pading size = %d  j= %d   =  %c \n", strlen(paddings),j,paddings[j]);
      
    }
//       printf ("pading size AFTER FOR = %d", strlen(paddings));
    

   //printf("TEST1  %d", paddingSize);
    queueFront =0;  // first item
    for (j=0;j<PACKETS;j++){
      //      message2[j]= j;
      sprintf(message2[j], "%10d", j);
      queueRear = queueRear + 1;
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
    x=0;

   printf("Sending %d  packets.\n",PACKETS);
//   printf ("queue 99  size = %d\n", sizeof(message2));
printf("TEST1 rear=  %d front= %d\n", queueRear,queueFront );
   gettimeofday(&starts, NULL);


/* here, do your time-consuming job */


  //  while(x<PACKETS)
    while(queueFront<=queueRear)
    {
//              printf("Enter message : ");
      ///   gets(message);
     // printf ("message %s\n", message2[1][0]);
//printf("TEST2 rear=  %d front= %d\n", queueRear,queueFront );
   int column0 = strlen(msgId);
printf("\nTEST400:queue msgId= %s len1=%d ",msgId,column0);

    msgId = readQueue();
//strncpy(msgId,readQueue(),10);
     int column = sizeof(message2[x]);
     int column1 = strlen(msgId);
     
//   printf ("queue 1   size = %d\n", sizeof(message2[1]));
 
      //   char *msgSent =concat(message2[x][0], paddings);
//         msgSent = (char*)malloc(sizeof(message2[x])+sizeof(paddings)+1);//+1 for the zero-terminator
         msgSent = (char*)malloc(strlen(msgId)+sizeof(paddings)+1);//+1 for the zero-terminator
        // in real code you would check for errors in malloc here
         strncpy(msgSent,msgId,column1);
         strcat(msgSent, paddings);
 
//        if (sendto(s, message2[x], strlen(message2[x]) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        if (sendto(s, msgSent, strlen(msgSent) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
        x++;
        queueFront ++;
        //free(msgSent);
        //receive a reply and print it
        //clear thesgId filling null, it might have previously received data
	// memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        //if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
	// {
	//  die("recvfrom()");
	    // }
         
	// puts(buf);
//	printf("\nTEST3 rear=  %d front= %d\n", queueRear,queueFront );
    
//    printf("\nTEST4:queue msgId= %s message2 = %s len= %d len1=%d value: %s",msgId, message2[x],column,column1, msgSent); 
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
