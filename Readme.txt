Part A

We have to Create a Bootstrap server that accepts the serving information from different
servers like servicename, IP address, port number, and a service access token
on UDP, Later it should allow a
client asking for these servers' information on UDP.

Steps

1)open terminal where the "udpserver.cpp" is stored.
  compile and run "udpserver.cpp" by first 
  ->Compiling -> "g++ udpserver.cpp"
  ->Running   -> "g++ ./a.out"
 this will create a bootstrap server that accepts the serving information from different
 servers like servicename, IP address, port number, and a service access token
 on UDP.

2)Now again open terminal where the "Fileserver.cpp" is stored.
   compile and run "Fileserver.cpp" by first 
  ->Compiling -> "g++ Fileserver.cpp"
  ->Running   -> "g++ ./a.out"
  Now the program will ask us to enter Service type(The input will be in terms of an integer),IP address,Port number and Service Access token.
  and send this data to the bootstrap server where it is registered.
  for eg. 1 == PDF
	    2 == txt
	    3 == img
			etc...
you can follow the step (2) for other file servers like imgserver,mp3_mp4server,txtserver by running the same Fileserver.cpp file
and by providing different service type and all the respective info 

************************************************************************************************************************************************

Part B

Now we have to Extend the file server program in Part A. Once it registers to the
BootStrap server successfully in Part A, it listens on TCP socket. The
server is meant to send a specific file to its clients depending on the
service type it is intended for. Like a video server would send a video file
which a client asks for.

1)open terminal where the "udpserver.cpp" is stored.
  compile and run "udpserver.cpp" by first 
  ->Compiling -> "g++ udpserver.cpp"
  ->Running   -> "g++ ./a.out"
 this will create a bootstrap server that accepts the serving information from different
 servers like servicename, IP address, port number, and a service access token
 on UDP.

2)Now again open terminal where the "Fileserver.cpp" is stored.
   compile and run "Fileserver.cpp" by first 
  ->Compiling -> "g++ -pthread Fileserver.cpp"
  ->Running   -> "g++ ./a.out"
  Now the program will ask us to enter service type(The input will be in terms of an integer),IP address,Port number and Service Access token.
  and send this data to the bootstrap server where it is registered.
  for eg. 1 == PDF
	    2 == txt
	    3 == img
			etc...
you can follow the step (2) for other file servers like imgserver,mp3_mp4server,txtserver by running the same Fileserver.cpp file
and by providing different service type and all the respective info 


3)Now again open the terminal where the "Client.cpp" is stored.(here multiple clients can run and server will serve them in parallel)
  compile and run "client.cpp" by first 
  ->Compiling -> "g++ client.cpp"
  ->Running   -> "g++ ./a.out"
  
the client fetches all the servers’ information from the
BootStrap server using UDP.
service name,respective IP address, port
number, and an accesstoken

3)Now the program will ask user to select a service type(The input will be in terms of integer) and enter its IP address,Port Number and Service access token.
for eg. 1 == pdf
	  2 == txt
	  3 == img
		    etc...

4)Now the Respective File server checks if the Service Access token(SAT) provided is valid or not.
  and if the SAT provided is Invalid it throws and error and informs client that it is invalid.

5)Now as we have verified that access token is valid,the program will asks us to enter the path of the file.

6)but if the file path provided doesn't exists or the file does not exists than, server will inform client and close.

7)Now once the file path is provided and found, appropriate "received" file is generated on the client system.

Note:- Here the above program implements multithreading and therefore can serve multiple clients at once in parallel.


*************************************************************************************************************************************************

PLAGIARISM STATEMENT

I certify that this assignment/report is my own work, based on my personal study and/or
research and that I have acknowledged all material and sources used in its preparation, whether
they be books, articles, reports, lecture notes, and any other kind of document, electronic or
personal communication. I also certify that this assignment/report has not previously been
submitted for assessment in any other course, except where specific permission has been
granted from all course instructors involved, or at any other time in this course, and that I have
not copied in part or whole or otherwise plagiarized the work of other students and/or persons. I
pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I understand my responsibility to report honor violations by other students if I become aware of it.


Name of the student => K Saravanan
Roll No => CS22MTECH12007
