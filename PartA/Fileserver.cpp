#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <bits/stdc++.h>

using namespace std;

struct sockaddr_in serverAddr;
int sockfd;
socklen_t addr_size;
int backLog = 3;
string sat;
   
void startserver()
{
    //specify the ip address,port number and Service access token of the fileserver
    string ipaddr;
    cout<<"Enter desired Ip address"<<endl;
    cin>>ipaddr;
    int port;
    cout<<"Enter Desired Port"<<endl;
    cin>>port;
    cout<<"Enter Service access token"<<endl;
    cin>>sat;
    //send ip address,port number and Service access token of the fileserver to the bootstrap server
    int x = sendto(sockfd,ipaddr.c_str(),strlen(ipaddr.c_str()),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
    int y = sendto(sockfd,sat.c_str(),strlen(sat.c_str()),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
    int z = sendto(sockfd,&port,sizeof(int),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));

    int status;// this variable will confirm that our registration was successfull or not
    recvfrom(sockfd, &status, sizeof(int), 0, (struct sockaddr*)& serverAddr,&addr_size);

    if(status == 1)
        cout<<"Server Registered Successfully"<<endl;
    else
        cout<<"Error in Server Registration"<<endl;

    return;
}

int main(){
  int port = 9000;// port on which the bootstrap server is running
  char buffer[1024];
  addr_size = sizeof(sockaddr);

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));
  
  //setting up the enviorment for server socket address
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr_size = sizeof(serverAddr);

   /*
	   Here MSGTYPE will specify the query that is asked in the below mentioned way
	   MSGTYPE->1 == REGISTRATION query to register the file server
	   MSGTYPE->2 == Find info of the file server query, to get the info about file server
   */
    int MSGTYPE = 1;//File server will always send registration request
    
    sendto(sockfd,&MSGTYPE,sizeof(int),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
    int FileServerType;
    /*
        FileServer Type will specify the type of files that will be handeled by the respective server
        1->Pdf
        2->Txt
        3->img
        4->Mp4/mp3
    */
    cout<<"Enter the Fileserver type"<<endl;
    cout<<"1->pdf"<<endl<<"2->txt"<<endl<<"3->img"<<endl<<"4->Mp4/Mp3"<<endl;
    cin>>FileServerType;
    sendto(sockfd,&FileServerType,sizeof(int), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    startserver();//this function will start the server
    return 0;
}
