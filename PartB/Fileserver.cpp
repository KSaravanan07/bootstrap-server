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

void *client_thread(void *pClient)
{
      int newClientFd = *((int *)pClient); // type casting void * to int * and then extracting the client socket id
      char rcvDataBuf[1024], sendDataBuf[1024]; // declaring buffers to store sent and received data
      int dataRecvd = 0;//we will declare integers to check whether we are receiving or sending data or not
      int dataSent = 0;
      char sat_auth[1024];
      memset(&sat_auth,0, 1024);
      recv(newClientFd, &sat_auth, 1024, 0);
      if(string(sat_auth) == sat)// this will verify that the access is authorized or not via serice access token
      {
         int auth = 1;
         send(newClientFd,&auth,sizeof(int),0);
         cout<<"Legal access"<<endl;
      }
      else
      {
         int auth = -1;
         send(newClientFd,&auth,sizeof(int),0);
         cout<<"UnAuthorized access!,Invalid Client"<<endl;
         return NULL;
      }
      memset(&rcvDataBuf, 0, 1024);// setting the memory to 0 to avoid any garbage value
      dataRecvd = recv(newClientFd, &rcvDataBuf, 1024, 0); // now rcvDataBuf will have the path of the file
            FILE *fp;
            fp = fopen(rcvDataBuf,"rb");//we will open the file in read binary mode
            if(fp != NULL)
            {
               int file_status = 1;
               send(newClientFd,&file_status,sizeof(int),0);//send status as 1 if file is found
               fseek(fp,0,SEEK_END);//this will put the pointer at the end of file
            }
            else
            {
               int file_status = -1;
               send(newClientFd,&file_status,sizeof(int),0);//send status as -1 if file is not found
               return NULL;
            }      
            long long size = ftell(fp); // this will tell how far the pointer has travelled from the starting point(size)
            rewind(fp); // this will again set the pointer to begining of file
            send(newClientFd,&size,sizeof(long long),0);//sends the size of file to the client
            char buffer[1024];// we will send the file in the chunks of 1024 bytes
            memset(buffer,'\0',1024);
            while(!feof(fp)) // reads till end of the file
            {
                  fread(buffer,1024,1,fp);
                  dataSent = send(newClientFd,&buffer,1024,0);
                  memset(buffer,'\0',1024);
            }
            cin.clear();
            fclose(fp);
        return NULL;
}
   
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
    sendto(sockfd,ipaddr.c_str(),strlen(ipaddr.c_str()),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
    sendto(sockfd,sat.c_str(),strlen(sat.c_str()),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
    sendto(sockfd,&port,sizeof(int),0,(struct sockaddr*)&serverAddr, sizeof(serverAddr));

   int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if(!serverSocket)
   {
      cout<<"Error creating socket"<<endl;
      exit(1);
   }

   //Now we will set the enviorment for server socket address
   struct sockaddr_in serverSockAddr;
   serverSockAddr.sin_family = AF_INET;
   serverSockAddr.sin_port = htons(port);

   inet_pton(AF_INET, ipaddr.c_str(), &(serverSockAddr.sin_addr));
   memset(&(serverSockAddr.sin_zero), '\0', 8); // setting memory to 0 to avoid any garbage value

   //Server will now listen on the specified Port number and it will run on specified IP address
   cout<<"Server listening on IP "<<ipaddr<<endl;
   cout<<"and port number "<<port<<endl;

    //Now we will bind the socket
   int bind_int = bind(serverSocket, (struct sockaddr *)&serverSockAddr, sizeof(struct sockaddr)); 
   if(bind_int<0)
   {
      cout<<"Error binding socket"<<endl;
      close(serverSocket);
      exit(1);
   }

   //Now we will listen socket and backLog will specify a number ->backLog
   //and after that many clients server will start to put clients into waiting queue
   bind_int = listen(serverSocket, backLog);
   if(!serverSocket)
   {
      cout<<"Error listening socket"<<endl;
      close(serverSocket);
      exit(1);
   }


   socklen_t sinSize = sizeof(struct sockaddr_in);
   int dataRecvd = 0, dataSent = 0; //we will declare integers to check whether we are receiving or sending data or not
   struct sockaddr_in clientAddress;
   char rcvData[1024], sendData[1024]; // we will declare char arrays to store the data which is sent and received
   string sendDataStr;

   memset(&clientAddress, 0, sizeof(struct sockaddr_in));//we will set the memory to 0 to avoid any garbage value
   memset(&rcvData, 0, 1024);//we will set the memory to 0 to avoid any garbage value
   while(1)
   {   
      memset(&clientAddress, 0, sizeof(struct sockaddr_in));//we will set the memory to 0 to avoid any garbage value
      //memset(&rcvBuf, 0, 1024);//we will set the memory to 0 to avoid any garbage value
      int newClientFd = accept(serverSocket, (struct sockaddr *)&clientAddress, &sinSize);//now we will accept the connection from client
      if (!newClientFd)
      {
         cout<<"Error with new client connection "<<endl;
         exit(1);
      }

      cout<<"New client arrived"<<endl;
      pthread_t Client_thread; // Now we are declaring a pthread object to use multithreading
      int *pclient = NULL; // declaring a pointer
      pclient = new int;  // allocating memory to the pointer
      *pclient = newClientFd; // assigning current client socket id to the pointer
      pthread_create(&Client_thread,NULL,client_thread,pclient); // creating individual thread for each client and calling the function 
   }
}

int main(){
  int port = 9000; // port on which the bootstrap server is running
  char buffer[1024];
  addr_size = sizeof(sockaddr);

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));

  //setting up the enviorment for server socket address
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
