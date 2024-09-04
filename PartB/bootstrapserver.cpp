#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

using namespace std;
string pdf_ip,img_ip,txt_ip,vid_ip;// stores the ip addresses of the Fileservers respective of the type of files they handle
int    pdf_port=0,img_port=0,txt_port=0,vid_port=0;// stores the port numbers of the Fileservers respective of the type of files they handle
string pdf_sat,img_sat,txt_sat,vid_sat;//stores the Service Access tokens of the Fileservers respective of the type of files they handle
struct sockaddr_in sockaddr, sockaddr1;
socklen_t addr_size;
int sockfd;
void serve(int FileServertype)//Registartion of the Fileserver
{
    /*
        FileServer Type will specify the type of files that will be handeled by the respective server
        1->Pdf
        2->Txt
        3->img
        4->Mp4/mp3
        */
    if(FileServertype == 1)
    {   
        int port;
        char buff1[1024],buff2[1024];
        memset(&buff1,'\0',1024);
        memset(&buff2,'\0',1024);
        recvfrom(sockfd, &buff1, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get ip address
        recvfrom(sockfd, &buff2, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get service access token
        recvfrom(sockfd, &port, sizeof(int), 0, (struct sockaddr*)& sockaddr1, &addr_size);//get port number

        cout<<"pdf ip :-"<<buff1<<endl;
        pdf_ip = string(buff1);//store ip address for maintaining record
        pdf_port = port;//store port number for maintaining record
        pdf_sat = string(buff2);//store service access token for maintaining record
    }
    else if(FileServertype == 2)
    {
        int port;
        char buff1[1024],buff2[1024];
        memset(&buff1,'\0',1024);
        memset(&buff2,'\0',1024);
        recvfrom(sockfd, &buff1, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get ip address
        recvfrom(sockfd, &buff2, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get service access token
        recvfrom(sockfd, &port, sizeof(int), 0, (struct sockaddr*)& sockaddr1, &addr_size);//get port number

        cout<<"txt ip:- "<<buff1<<endl;
        txt_ip = string(buff1);//store ip address for maintaining record
        txt_port = port;//store port number for maintaining record
        txt_sat = string(buff2);//store service access token for maintaining record
    }
    else if(FileServertype == 3)
    {
        int port;
        char buff1[1024],buff2[1024];
        memset(&buff1,'\0',1024);
        memset(&buff2,'\0',1024);
        recvfrom(sockfd, &buff1, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get ip address
        recvfrom(sockfd, &buff2, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get service access token
        recvfrom(sockfd, &port, sizeof(int), 0, (struct sockaddr*)& sockaddr1, &addr_size);//get port number

        cout<<"img ip :-"<<buff1<<endl;
        img_ip = string(buff1);//store ip address for maintaining record
        img_port = port;//store port number for maintaining record
        img_sat = string(buff2);//store service access token for maintaining record
    }
    else if(FileServertype == 4)
    {
        int port;
        char buff1[1024],buff2[1024];
        memset(&buff1,'\0',1024);
        memset(&buff2,'\0',1024);
        recvfrom(sockfd, &buff1, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get ip address
        recvfrom(sockfd, &buff2, 1024, 0, (struct sockaddr*)& sockaddr1, &addr_size);//get service access token
        recvfrom(sockfd, &port, sizeof(int), 0, (struct sockaddr*)& sockaddr1, &addr_size);//get port number

        cout<<"mp3_mp4 ip :-"<<buff1<<endl;
        vid_ip = string(buff1);//store ip address for maintaining record
        vid_port = port;//store port number for maintaining record
        vid_sat = string(buff2);//store service access token for maintaining record
    }
    else 
    {
        exit(1);
    }
}
void fetch_server_info()//this function will simply return all the info of the File servers available with the bootstrap server
{
    string info = "Txt Ip :-" + txt_ip + "\n" + "Txt port :-" + to_string(txt_port) + "\n" + "Txt Servive access token :-" + txt_sat + "\n" +
                    "Pdf Ip :-" + pdf_ip + "\n" + "pdf port :-" + to_string(pdf_port) + "\n" + "pdf Servive access token :-" + pdf_sat + "\n" +
                    "img Ip " + img_ip + "\n" + "img port :-" + to_string(img_port) + "\n" + "img Servive access token :-" + img_sat + "\n" + 
                    "Mp3/Mp4 Ip :-" + vid_ip + "\n" + "Mp3/Mp4 port :-" + to_string(vid_port) + "\n" + "Mp4/mp3 Servive access token :-" + vid_sat + "\n";
    sendto(sockfd,info.c_str(),strlen(info.c_str()),0,(struct sockaddr*)&sockaddr1, sizeof(sockaddr1));
}
int main(){
    int port = 9000;// port on which the bootstrap server is running
    
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);//creating socket for communication
  memset(&sockaddr, '\0', sizeof(sockaddr));
  
   //setting up the enviorment for serversocket address
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(port);
  sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   //binding the socket
  bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

  cout<<"Server Up and Running!"<<endl;
  addr_size = sizeof(sockaddr1);

   while(1)
   {
   /*
   Here MSGTYPE will specify the query that is asked in the below mentioned way
   MSGTYPE->1 == REGISTRATION query to register the file server
   MSGTYPE->2 == Find info of the file server query, to get the info about file server
   */
    int MSGTYPE = 0;
    recvfrom(sockfd, &MSGTYPE, sizeof(int), 0, (struct sockaddr*)& sockaddr1, &addr_size);
    if(MSGTYPE == 1) // registartion query
    {
        int FileServerType;
        /*
        FileServer Type will specify the type of files that will be handeled by the respective server
        1->Pdf
        2->Txt
        3->img
        4->Mp4/mp3
        */
        memset(&FileServerType, '\0',1024);
        recvfrom(sockfd, &FileServerType, sizeof(int), 0, (struct sockaddr*)& sockaddr1, &addr_size);
        serve(FileServerType); // this function will handle the registration of the server
    }
    else if(MSGTYPE == 2) // serverinfo query
    {
        fetch_server_info(); // this function will take care of serverinfo query
    }
   }
}
