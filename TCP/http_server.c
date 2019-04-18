#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define BUFSIZE 100000
#define MAX_COUNT  200
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

void error(char *msg) {
  perror(msg);
  exit(1);
}
char header_1[100];
char header_2[100] = "Content-Length: ";
char header_3[100];
char header_4[100]= "\r\n\r\n";

int main(int argc, char **argv) {

	int sockfd; /* socket */
	int portno; /* port to listen on */
	int clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	struct hostent *hostp; /* client host info */
  	char buf[BUFSIZE]; /* message buf */
	char resp[BUFSIZE]; /*For index or default page*/
	char file[BUFSIZE];
	char f_name[100]; /*file_name*/
        FILE *fp;
	char e_name[100]; /*extention name*/
	char *hostaddrp; /* dotted decimal host addr string */
	int optval; /* flag value for setsockopt */
	long nb; /* number of bytes */
   	long file_desc;/*file_descriptor*/
        char *start_p;/*pointer which points to starting of the file name*/
        char *end_p;/*pointer which points to ending of the file name*/
        long size; /*End of a file number*/
        long fn_ct = 0;
	/* 
   	* check command line arguments 
   	*/
  	if (argc != 2) {
    		fprintf(stderr, "usage: %s <port>\n", argv[0]);
    		exit(1);
  	}
  	portno = myAtoi(argv[1]);

  	/* 
   	* socket: create the parent socket 
   	*/
  	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (sockfd < 0) 
    		error("ERROR opening socket");

  	/* setsockopt: Handy debugging trick that lets 
   	* us rerun the server immediately after we kill it; 
   	* otherwise we have to wait about 20 secs. 
   	* Eliminates "ERROR on binding: Address already in use" error. 
   	*/
  	optval = 1;
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

	clientlen = sizeof(clientaddr);
	nb=listen(sockfd,100000);
	int i=0;
	bzero(buf,10);
	printf("Waiting for connection\n");
	while(1)
       {

		file_desc=accept(sockfd,(struct sockaddr *)&clientaddr,&clientlen);
		if(file_desc < 0)
                {
			error("INvalid error");
			exit(1);
		}
                bzero(buf,BUFSIZE);
                bzero(f_name,20);
                bzero(e_name,20);
                fn_ct = 0;
		nb=recv(file_desc,buf,100000,0);
                start_p = buf;
               
                start_p = start_p + 5;
                /*searching for the second space to get the end of the file name*/
                while(*start_p != ' ')
                {
                 f_name[fn_ct] = *start_p;
                 fn_ct++;
                 start_p++;
               }
               /*storing file name in f_name*/
               f_name[fn_ct] = '\0';
               /*searching for the last '.' to get the extention of the file*/
               start_p = f_name;
               while(*start_p != '\0'){
               if(*start_p == '.')
               {
                end_p = start_p;
               }
               start_p++;
               }
               /*putting file extention name in the end_p array*/
               fn_ct = 0;
               while(*end_p != '\0'){
               e_name[fn_ct] = *end_p;
                 fn_ct++;
                 end_p++;
               }
              
             
      bzero(resp,BUFSIZE);
      bzero(header_1,100);

if(!(strcmp(e_name,".html")))
      {
      strcpy(header_1,"HTTP/1.1 200 Ok\r\nContent-type: text/html\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
else if(!(strcmp(e_name,".css")))
      {
      strcpy(header_1,"HTTP/1.1 200 OK\r\nContent-type: text/css\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
else if(!(strcmp(e_name,".js")))
      {
      strcpy(header_1,"HTTP/1.1 200 OK\r\nContent-type: application/javascript\r\n\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
else if(!(strcmp(e_name,".txt")))
      {
      strcpy(header_1,"HTTP/1.1 200 Document Follows\r\nContent-type: text/plain\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
else if(!(strcmp(e_name,".gif")))
      {
      strcpy(header_1,"HTTP/1.1 200 Ok\r\nContent-type: image/gif\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
else if(!(strcmp(e_name,".png")))
      {
      strcpy(header_1,"HTTP/1.1 200 OK\r\nContent-type: image/png\r\n\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
else if(!(strcmp(e_name,".txt")))
      {
      strcpy(header_1,"HTTP/1.1 200 OK\r\nContent-type: image/jpg\r\n\r\n");
      strcpy(resp,header_1);
      fp =fopen(f_name,"r");
      fseek(fp,0,SEEK_END);
      size=ftell(fp);
      rewind(fp);
      sprintf(header_3,"%ld",size);
      strcat(resp,header_2);
      strcat(resp,header_3);
      strcat(resp,header_4);
      fread(file,1,size,fp);
      strcat(resp,file);
      nb=send(file_desc,resp,strlen(resp),0);
      }
close(file_desc);
}
}
