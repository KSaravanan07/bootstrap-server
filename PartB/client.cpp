#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <bits/stdc++.h>

struct sockaddr_in serverAddr;
int sockfd;
socklen_t addr_size;
using namespace std;

int main()
{
    int s_port = 9000; // port at which bootstrap server is running
    char info[1024];// this will store all the data of file servers
	
    //Creating a socket for communication
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, '\0', sizeof(serverAddr));//setting memory to 0 to avoid garbage values
    memset(&info, '\0', 1024);//setting memory to 0 to avoid garbage values

    //setting up enviorment for serversocket address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(s_port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_size = sizeof(serverAddr);
    /*
	   Here MSGTYPE will specify the query that is asked in the below mentioned way
	   MSGTYPE->1 == REGISTRATION query to register the file server
	   MSGTYPE->2 == Find info of the file server query, to get the info about file server
   */
    int MSGTYPE = 2;//client will always ask for the info about File servers
    sendto(sockfd,&MSGTYPE,sizeof(int),0,(struct sockaddr*)&serverAddr, addr_size);
    
    recvfrom(sockfd,&info,1024,0,(struct sockaddr*)&serverAddr, &addr_size);//receive all the info abount Files servers

    string serverinfo = string(info);
    cout<<serverinfo<<endl;
    int port;
    string ipaddr;
    string sat;
    int x;//this will specify the type of file required
    cout<<"Enter Type of the file required"<<endl<<"1->pdf"<<endl<<"2->txt"<<endl<<"3->img"<<endl<<"4->Mp4/Mp3"<<endl;
    cin>>x;
    if(x != 1 && x != 2 && x != 3 && x != 4)
    {
    	cout<<"enter correct choice"<<endl;
    	return 0;
    }
    cout<<"Enter Ip address"<<endl;
    cin>>ipaddr;//ip address to connect to
    cout<<"Enter port number"<<endl;
    cin>>port;// port number of the process
    cout<<"Enter servive access token"<<endl;
    cin>>sat; // service access token
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//tcp connection
   if(!clientSocket)
   {
      cout<<"Error creating socket"<<endl;
      exit(1);
   }

   //setting the enviorment for serversocketaddress
   struct sockaddr_in serverSockAddressInfo;
   serverSockAddressInfo.sin_family = AF_INET;
   serverSockAddressInfo.sin_port = htons(port);
   inet_pton(AF_INET, ipaddr.c_str(), &(serverSockAddressInfo.sin_addr));

   memset(&(serverSockAddressInfo.sin_zero), '\0', 8);//setting memory to 0 to avoid garbage values

   socklen_t sinSiz = sizeof(struct sockaddr_in);
   long long dataRecvd = 0, dataSent = 0;// declaring these integers to check whether we are sending/receving the data to and from server
   struct sockaddr_in clientAddressInfo;
   char rcvDataBuf[1460], sendDataBuf[1460];// declaring the buffers to exchange data to and from server
   string sendData, rcvData;

   //now we will bind the socket
   int bind_int = connect(clientSocket, (struct sockaddr *)&serverSockAddressInfo, sizeof(struct sockaddr));
   if (bind_int<0)
   {
      cout<<"Error with server connection "<<endl;
      close(clientSocket);
      exit(1);
   }
   cout<<"successfully binded"<<endl;
   send(clientSocket,sat.c_str(),strlen(sat.c_str()),0);
   int auth;//this variable will specify if the Service access token is valid or not
   recv(clientSocket,&auth,sizeof(int),0);
   if(auth == 1)
   {
      cout<<"Client successfully authenticated"<<endl;
   }
   else if(auth == -1)
   {
      cout<<"Authentication Failed!,Invalid Client"<<endl;
      exit(1);
   }
   
   cout<<"Enter Path of the File"<<endl; // now it will ask user to enter path/name of the file
   memset(&sendDataBuf, 0, 1460);// setting memory to 0 to avoid garbage value
   string path;
   cin>>path;

   FILE *fp;
   dataSent = send(clientSocket, path.c_str(), strlen(path.c_str()), 0);//sending the path to server
   int file_status;// file_status = 1 => file found, file_status = -1 => file not found
   int r = recv(clientSocket,&file_status,sizeof(int),0);
   if(file_status == -1)
   {
         cout<<"File Doesn't Exists"<<endl;
         return 0; // if file not found then notify user and again start from asking file type
   }
   else
      cout<<"File found!"<<endl;
   long long size; // size of the file requested
   memset(&rcvDataBuf, 0, 1460);
   long long data = recv(clientSocket,&size,sizeof(long long),0);
   if(x == 1)
         fp = fopen("received.pdf","wb");
    else if(x == 2)
         fp = fopen("received.txt","wb");
    else if(x == 3)
      {
         int x1;
         cout<<"Enter Format of file requested"<<endl;
         cout<<"1->PNG"<<endl<<"2->JPG"<<endl;
         cin>>x1;
         
         if(x1 == 1)
            fp = fopen("received.png","wb");
         else if(x1 ==2)
            fp = fopen("received.jpg","wb");
         else
         {
            cout<<"Enter correct choice"<<endl;
            return 0;
            }
      }
    else if(x == 4)
    {
         int x1;
         cout<<"Enter type of file requested"<<endl;
         cout<<"1->MP4"<<endl<<"2->MP3"<<endl;
         cin>>x1;

         if(x1 == 1)
            fp = fopen("received.mp4","wb");
         else if(x1 ==2)
            fp = fopen("received.mp3","wb");
         else
         {
            cout<<"Enter correct choice"<<endl;
            return 0;
         }
    }
   long long received = 0;
   long long size_left = size;
   cout<<"size is "<<size<<endl;
   char buffer[1024];
   while(received < size) // this loop will run till we have received data which is equal to the size of file
   {
      dataRecvd = recv(clientSocket,&buffer,1024,0);// receiving files in chunk of 1024 bytes from server            
      received += dataRecvd;
      if(size_left <= 1024)
         fwrite(buffer,1,size_left,fp);// writing in the file 
      else
         fwrite(buffer,1,dataRecvd,fp); // writing in the file 
         size_left -= dataRecvd;
         memset(&buffer,'\0',1024);
   }
   fclose(fp);
   cout<<"File created succesfully"<<endl;
   return 0;
}
