//////////////////	IMPLEMENT DNS	////////////

// Server Program : dnss.c

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<netdb.h>

int main()
{
    
    struct sockaddr_in server,client;
    int s,n;
    char b1[100],b2[100];

struct hostent *hen;
char *IPaddr;

    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=5000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(s,(struct sockaddr *)&server,sizeof(server));
    n=sizeof(client);
 printf("DNS is ready ..\n");
    while(1)
    {

        strcpy(b2,"");
        recvfrom(s,b1,sizeof b1, 0,(struct sockaddr *)&client,&n);
       
       hen = gethostbyname(b1);

       printf("\n Client request for Domain Name: %s \n",hen->h_name); 
	IPaddr=inet_ntoa(*((struct in_addr *)hen->h_addr));

	printf("DNS IP address is:%s \n",IPaddr);
	strcpy(b2, IPaddr);

	sendto(s,b2,sizeof b2,0,(struct sockaddr *)&client,n); 
   }
return 0;
}

/*
> gcc dnss.c

./a.out
DNS is ready ..

 Client request for Domain Name: google.com 
DNS IP address is:142.250.193.142 

 Client request for Domain Name: uvce.ac.in 
DNS IP address is:134.209.146.145 

 Client request for Domain Name: netflix.com 
DNS IP address is:54.246.79.9 

 Client request for Domain Name: wikipedia.com 
DNS IP address is:103.102.166.226 


*/


// Client Program : dnsc.c

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
int main()
{
    struct sockaddr_in server,client;
    int s,n;
    char b1[100],b2[100];
    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=5000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    n=sizeof(server);
	
while(1){
    printf("\nEnter canonical Domain Name: ");
    scanf("%s",b2);
    sendto(s,b2,sizeof(b2),0,(struct sockaddr *)&server,n);
    recvfrom(s,b1,sizeof(b1), 0,NULL,NULL);
    printf("%s \n",b1);
}
return 0;
}

/*
> gcc dnsc.c
> ./a.out

Enter canonical Domain Name: google.com
142.250.193.142 

Enter canonical Domain Name: uvce.ac.in
134.209.146.145 

Enter canonical Domain Name: netflix.com
54.246.79.9 

Enter canonical Domain Name: wikipedia.com
103.102.166.226 

*/