#include "ActionAndResponseOpCodes.h"
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>
using namespace std;

int lastConnectFd = -1;
char recvBuffer[1500];
char respondedToLastConnect = 1;

#define BUFLEN 512  //Max length of buffer

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void die(char *s)
{
    perror(s);
    exit(1);
}

void CreateServer(int portno)
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, recv_len;
    socklen_t slen = sizeof(si_other);
    char buf[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(portno);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
         
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
}


int TCP_PORT = 5000;

void ServerThread()
{ 
    cout << "Begin Server" << endl;
    CreateServer(TCP_PORT);
}

std::thread* CreateServerThread;

void CreateInterruptServer()
{
  CreateServerThread = new std::thread(ServerThread);
}

void CheckServerForInputs()
{
    if(respondedToLastConnect == 0 && lastConnectFd >= 0)
    {
        int n = 0;

        for(int i = 0; i < 200; i++)
        {
          inputBuffer[i] = recvBuffer[i];
        }

        ExecuteControlOpCodes();
        n = write(lastConnectFd,outputBuffer,outputBufferLastByte);
        if (n < 0) error("ERROR writing to socket");
        close(lastConnectFd);

        lastConnectFd = -1;
        respondedToLastConnect = 1;
    }
    
}

int Write3CharactersFromValue(unsigned char value, char* IPString, int startPoint)
{
  IPString[startPoint] = (value/100)%10 + '0';
  startPoint++;
  IPString[startPoint] = (value/10)%10 + '0';
  startPoint++;
  IPString[startPoint] = (value%10) + '0';
  startPoint++;
  return startPoint;
}

void WriteHeepIPToString(HeepIPAddress destIP, char* IPString)
{
  int counter = Write3CharactersFromValue(destIP.Octet4, IPString, 0);
  IPString[counter] = '.'; counter++;
  counter = Write3CharactersFromValue(destIP.Octet3, IPString, counter);
  IPString[counter] = '.'; counter++;
  counter = Write3CharactersFromValue(destIP.Octet2, IPString, counter);
  IPString[counter] = '.'; counter++;
  counter = Write3CharactersFromValue(destIP.Octet1, IPString, counter);
}

void SendOutputBufferToIP(HeepIPAddress destIP)
{
    char IP [17];
    WriteHeepIPToString(destIP, IP);

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(IP);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(TCP_PORT);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    n = write(sockfd,outputBuffer,outputBufferLastByte);
    if (n < 0) 
         error("ERROR writing to socket");

    bzero(inputBuffer,200);
    n = read(sockfd,inputBuffer,200);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",inputBuffer);
    close(sockfd);
}