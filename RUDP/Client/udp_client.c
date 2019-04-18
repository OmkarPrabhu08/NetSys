/*
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024
char buf[1024];
char fname[100];
int clientlen; /* byte size of client's address */
struct sockaddr_in clientaddr; /* client addr */
char rev_buf[BUFSIZE]; /* message buf */
long total_size;
int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;

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
long myAtoi(char *str)
{
    long res = 0; // Initialize result
 
    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
 
    // return result.
    return res;
}
/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}
void rf(){
  long seek_position = 0;
  long total_size;
  long rem_bytes;
  long count = 0;
  long sent_count;
  FILE *fp;
  fp = fopen("foo11","w");
  n = recvfrom(sockfd, buf ,1024,0, &serveraddr, &serverlen);


  total_size = myAtoi(buf);

  n = sendto(sockfd, "OK", 2, 0, &serveraddr, serverlen);
  bzero(buf,1024);
  n = recvfrom(sockfd, buf ,1024,0, &serveraddr, &serverlen);
  rem_bytes = myAtoi(buf);

  struct timeval tv;
  tv.tv_sec = 9;  /* 1 Secs Timeout */
  tv.tv_usec = 0;  // Not init'ing this can cause strange errors
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));

  for(;count<=(total_size/1024);){
    if (count == total_size/1024){
             bzero(buf,BUFSIZE);
          n = recvfrom(sockfd, buf, 15, 0, &serveraddr, &serverlen);
     sent_count = myAtoi(buf);
     printf("\n%ld\n",sent_count);

     n = recvfrom(sockfd, buf, rem_bytes, 0, &serveraddr, &serverlen);

     }
     else{
     bzero(buf,BUFSIZE);
          n = recvfrom(sockfd, buf, 15, 0, &serveraddr, &serverlen);
     sent_count = myAtoi(buf);
     printf("\n%ld\n",sent_count);
     n = recvfrom(sockfd, buf, 1024, 0, &serveraddr, &serverlen);

     }

   if(n<0||sent_count == count -1){        n = sendto(sockfd, "NACK", 4, 0, &serveraddr, serverlen);}
   else{
    n = sendto(sockfd, "ACK", 3, 0, &serveraddr, serverlen);
  
   
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
           (struct sockaddr *) &serveraddr, serverlen);
    //long pck_count = 0;
    n = recvfrom(sockfd, buf ,2, 0,
         (struct sockaddr *)  &serveraddr, serverlen);
    printf("\n%s\n",buf);
        struct timeval tv;
  
  pck_count = total_size/1024;
  rem_bytes = total_size - (pck_count*1024);
  printf("\n%ld\n",rem_bytes);
  bzero(rev_buf,BUFSIZE);
*buf = itoa (rem_bytes,buf,10,rev_buf);

  n = sendto(sockfd,rev_buf , 1024, 0,
           (struct sockaddr *) &serveraddr, serverlen);
  pck_count = 0;

  for(;pck_count<=(total_size/1024);){
     itoa (pck_count,buf,10,pck_ct);
           n = sendto(sockfd, pck_ct, rem_bytes, 0,
           (struct sockaddr *) &serveraddr, serverlen);
     if(pck_count == total_size/1024)
    {         bzero(buf, BUFSIZE);
                 fread(buf,1,rem_bytes,fp);
             n = sendto(sockfd, buf, rem_bytes, 0,
           (struct sockaddr *) &serveraddr, serverlen);
           printf("%s",buf);

     }else if (pck_count < total_size/1024){
        bzero(buf, BUFSIZE);
       fread(buf,1,1024,fp);
 n = sendto(sockfd, buf, 1024, 0,
           (struct sockaddr *) &serveraddr, serverlen);
     
     
     }

        
     tv.tv_sec = 2;  /* 1 Secs Timeout */
       tv.tv_usec = 0;  // Not init'ing this can cause strange errors
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf ,1024, 0,
         (struct sockaddr *) &serveraddr, &serverlen);
   if(strcmp(buf,"NACK")==0||n<=0){}
   else if(n>0&& strcmp(buf,"ACK")==0){
    pck_count++;
     seek_position = seek_position + 1024;
    fseek(fp,seek_position,SEEK_SET);
   
   }
}
  
   fclose(fp);
}
int main(int argc, char **argv) {
    int option = 0;
    char revbuf[100];

     /* check command line arguments */
    if (argc != 3) {
    fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
    exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
      (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* get a message from the user */
    bzero(buf, BUFSIZE);
    printf("Please enter command: ");
    printf("\n1.gets<filename> ");
    printf("\n2.puts<filename>");
    printf("\n3.list");
    printf("\n4.delete<filename>");
    printf("\n5.exit\n");
    bzero(buf, BUFSIZE);
    fgets(buf, BUFSIZE, stdin);
    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, 1024, 0, &serveraddr, serverlen);
    printf("\n%s\n",buf);
    if(buf[0] == '1'){
    rf();
    }
    if(buf[0] == '2'){
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
    if(buf[0] == '3'){
    rf();
    }
    if(buf[0] == '5'){
    printf("\nExiting\n");
    close(sockfd);			
    exit(0);
    }
    return 0;
}


