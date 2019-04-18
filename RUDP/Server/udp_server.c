/*
 * udpserver.c - A simple UDP echo server
 * usage: udpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#define BUFSIZE 1024
char fname[100];
int serverlen;
char* itoa(long num, char* str, int base, char* revstr)
{
    long i = 0;

 

 
    // Process individual digits
    while (num != 0)
    {
        long rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 


 int count = 0;
 int j = 0;
      for (count = i - 1; count >= 0; count--)
      {
          revstr[j++] = str[count];
      }
          revstr[i] = '\0';

 
 
    return revstr;
}
int myAtoi(char *str)
{
    long res = 0; // Initialize result
 
    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
 
    // return result.
    return res;
}
struct packet{
int pck_no;
char payload[1024];
};

int sockfd; /* socket */
int portno; /* port to listen on */
int clientlen; /* byte size of client's address */
struct sockaddr_in serveraddr; /* server's addr */
struct sockaddr_in clientaddr; /* client addr */
struct hostent *hostp; /* client host info */
char buf[BUFSIZE]; /* message buf */
char rev_buf[BUFSIZE]; /* message buf */
char *hostaddrp; /* dotted decimal host addr string */
int optval; /* flag value for setsockopt */
int n; /* message byte size */
char *hostname;
char filename[100];
long total_size;
char ack_type[4];
int loop_count =0;
/*
 * error - wrapper for perror
 */
void rf(){
  long seek_position = 0;
  long total_size;
  long rem_bytes;
  long count = 0;
  long sent_count;
  FILE *fp;
  fp = fopen("foo11","w");
  n = recvfrom(sockfd, buf ,1024,0, &clientaddr, &clientlen);


  total_size = myAtoi(buf);

  n = sendto(sockfd, "OK", 2, 0, &clientaddr, clientlen);
  bzero(buf,1024);
  n = recvfrom(sockfd, buf ,1024,0, &clientaddr, &clientlen);
  rem_bytes = myAtoi(buf);

  struct timeval tv;
  tv.tv_sec = 9;  /* 1 Secs Timeout */
  tv.tv_usec = 0;  // Not init'ing this can cause strange errors
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));

  for(;count<=(total_size/1024);){
    if (count == total_size/1024){
             bzero(buf,BUFSIZE);
          n = recvfrom(sockfd, buf, 15, 0, &clientaddr,&clientlen);
     sent_count = myAtoi(buf);
     

     n = recvfrom(sockfd, buf, rem_bytes, 0, &clientaddr, &clientlen);

     }
     else{
     bzero(buf,BUFSIZE);
          n = recvfrom(sockfd, buf, 15, 0, &clientaddr, &clientlen);
     sent_count = myAtoi(buf);
     printf("\n%d\n",n);
     n = recvfrom(sockfd, buf, 1024, 0, &clientaddr,&clientlen);

     }

   if(n<0||sent_count == count -1){        n = sendto(sockfd, "NACK", 4, 0, &clientaddr, clientlen);}
   else{
    n = sendto(sockfd, "ACK", 3, 0, &clientaddr, clientlen);
  
   
        if (count == total_size/1024){
            fwrite(buf,1,rem_bytes,fp);
           count++;
    }
    else{
    fwrite(buf,1,1024,fp);
      seek_position = seek_position + 1024;
        fseek(fp,seek_position,SEEK_SET);
        count++;
       }

    tv.tv_sec = 2;  /* 1 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
  

    }

}
  fclose(fp);
 
  }
void sf(char *filename){
long seek_position = 0;
long pck_count=0;
long rem_bytes = 0;
  char pck_ct[15];

FILE *fp;
fp = fopen(filename,"r");
 
if(access(filename,F_OK) == -1)
{
printf("File does not exist");
return;
}

    /*
     * recvfrom: receive a UDP datagram from a client
     */

   // struct timeval tv;
   fseek (fp, 0, SEEK_END);   // non-portable
    total_size = ftell (fp);
    rewind(fp);

   *buf = itoa (total_size,buf,10,rev_buf);


    n = sendto(sockfd,rev_buf , 1024, 0,
           (struct sockaddr *) &clientaddr, clientlen);
    //long pck_count = 0;
    n = recvfrom(sockfd, buf ,2, 0,
         (struct sockaddr *) &clientaddr, &clientlen);
    printf("\n%s\n",buf);
        struct timeval tv;
  
  pck_count = total_size/1024;
  rem_bytes = total_size - (pck_count*1024);
  printf("\n%ld\n",rem_bytes);
  bzero(rev_buf,BUFSIZE);
*buf = itoa (rem_bytes,buf,10,rev_buf);

  n = sendto(sockfd,rev_buf , 1024, 0,
           (struct sockaddr *) &clientaddr, clientlen);
  pck_count = 0;

   for(;pck_count<=(total_size/1024);){
     itoa (pck_count,buf,10,pck_ct);
           n = sendto(sockfd, pck_ct, rem_bytes, 0,
           (struct sockaddr *) &clientaddr, clientlen);
     if(pck_count == total_size/1024)
    {         bzero(buf, BUFSIZE);
                 fread(buf,1,rem_bytes,fp);
             n = sendto(sockfd, buf, rem_bytes, 0,
           (struct sockaddr *) &clientaddr, clientlen);
           printf("%s",buf);

     }else if (pck_count < total_size/1024){
        bzero(buf, BUFSIZE);
       fread(buf,1,1024,fp);
 n = sendto(sockfd, buf, 1024, 0,
           (struct sockaddr *) &clientaddr, clientlen);
     
     
     }

        
     tv.tv_sec = 2;  /* 1 Secs Timeout */
       tv.tv_usec = 0;  // Not init'ing this can cause strange errors
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf ,1024, 0,
         (struct sockaddr *) &clientaddr, &clientlen);
   if(strcmp(buf,"NACK")==0||n<=0){}
   else if(n>0&& strcmp(buf,"ACK")==0){
    pck_count++;
     seek_position = seek_position + 1024;
    fseek(fp,seek_position,SEEK_SET);
   
   }
}
  
   fclose(fp);
}
void error(char *msg) {
  perror(msg);
  exit(1);
}


int main(int argc, char **argv) {

  
  int option = 0;
  /*
   * check command line arguments
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /*
   * socket: create the parent socket
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 10;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
         (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /*
   * bind: associate the parent socket with a port
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr,
       sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  /*
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
 
 
  
   bzero(buf, BUFSIZE);
   n = recvfrom(sockfd, buf, BUFSIZE, 0,
         (struct sockaddr *) &clientaddr, &clientlen);
   
   if(buf[0] == '1'){
   char *char_pt;
   char_pt = buf;
   char_pt = char_pt+2;
   int j = 0;
   printf("\n%s\n",buf);
   while(*(char_pt)!='\0'){
   fname[j] = *(char_pt);
   j++;
   char_pt++;
   }
   j = j-1;
   printf("\n%d\n",j);
   fname[j]= '\0';
   sf(fname);
   }
   if(buf[0] == '2'){
   rf();
   }
   if(buf[0] == '3'){
   system("ls -al > list.txt");
   sf("list.txt");
   }
   if(buf[0] == '4'){
   char *char_pt1;
   char_pt1 = buf;
   char_pt1 = char_pt1+2;
   int j1 = 0;
   printf("\n%s\n",buf);
   while(*(char_pt1)!='\0'){
   fname[j1] = *(char_pt1);
   j1++;
   char_pt1++;
   }
   j1 = j1-1;

   fname[j1]= '\0';
   remove(fname);
   }
   if(buf[0] == '5'){
   printf("\nExiting\n");
   close(sockfd);			
   exit(0);
   }
   
 
}
