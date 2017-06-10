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

 
#define PACKETS 10000  //The port on which to send data

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
 
int main(void)
{
    printf("HELPPPPPPPP");
    struct sockaddr_in si_other;
    int s, i,j,x, slen=sizeof(si_other);
    int paddingSize = BUFLEN-10;
  //  char buf[BUFLEN];
//    char message[PACKETS];
unsigned    char message2[PACKETS][10];
    char * msgSent;
    char paddings[paddingSize];

    for (j=0;j<paddingSize;j++){
           paddings[j]= '#';
     // sprintf(message2[j], "%d", j);
	 //   printf ("pading size = %d  j= %d   =  %c \n", strlen(paddings),j,paddings[j]);
      
    }
//       printf ("pading size AFTER FOR = %d", strlen(paddings));
    

printf("TEST1  %d", paddingSize);

    for (j=0;j<PACKETS;j++){
      //      message2[j]= j;
      sprintf(message2[j], "%10d", j);
 //     printf ("message %s\n", message2[j]); 
    }
    //   printf ("queue     size = %d", strlen(message2));
//    int column = sizeof(message2[0]);
   printf ("queue 1   size = %d\n", sizeof(message2[1]));
   printf ("queue 99  size = %d\n", sizeof(message2));
//   printf ("queue 130 size = %d\n", sizeof(message2[0][0]));
//printf("Number of columns: %d\n", column);

 
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

              printf("Enter message :xxxxxx\n");
clock_t begin = clock();


struct timeval starts, ends;
gettimeofday(&starts, NULL);


/* here, do your time-consuming job */


    while(x<PACKETS)
    {
//              printf("Enter message : ");
      ///   gets(message);
     // printf ("message %s\n", message2[1][0]);

     int column = sizeof(message2[x]);
//   printf ("queue 1   size = %d\n", sizeof(message2[1]));
//   printf ("queue 99  size = %d\n", sizeof(message2));
//   printf ("queue 130 size = %d\n", sizeof(message2[0][0]));
  // printf ("queue  str size = %d\n",strlen(message2[0]));
//   printf ("queue  str size = %d\n",strlen(message2[10]));
 //  printf ("queue  str size = %d\n",strlen(message2));
 //  printf("Number of columns: %d\n", column);
 
      //   char *msgSent =concat(message2[x][0], paddings);
         msgSent = (char*)malloc(sizeof(message2[x])+sizeof(paddings)+1);//+1 for the zero-terminator
 //        printf("AC=%d ... S=%s \n", strlen(msgSent), msgSent);
        // in real code you would check for errors in malloc here
         strncpy(msgSent,message2[x],column);
 //        printf("C=%d ... S=%s \n", strlen(msgSent), msgSent);
         strcat(msgSent, paddings);
//           printf("A=%d ... B=%d  ... C=%d ... S=%s \n", strlen(message2[x]), strlen(paddings),strlen(msgSent), msgSent);
 
//        if (sendto(s, message2[x], strlen(message2[x]) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        if (sendto(s, msgSent, strlen(msgSent) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
        x++;
//         printf("d=%d ... S=%s \n", strlen(msgSent), msgSent);
        //free(msgSent);
//         printf("e=%d ... S=%s \n", strlen(msgSent), msgSent);
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
	// memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        //if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
	// {
	//  die("recvfrom()");
	    // }
         
	// puts(buf);
    }

clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

gettimeofday(&ends, NULL);
double delta = ((ends.tv_sec  - starts.tv_sec) * 1000000u +
         ends.tv_usec - starts.tv_usec) / 1.e6;
printf("Time is : %f\n",delta);

printf ("Time Spent: %1f   %d ",time_spent,CLOCKS_PER_SEC); 
    close(s);
    return 0;
}
