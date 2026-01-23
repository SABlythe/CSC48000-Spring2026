#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#include <ctime>
#include <iostream>
#include <string>

using namespace std;

#define MAX_WAITERS 25

int doServer(int);
void doWork(int, struct sockaddr_in *);

int main(int argc, char *argv[])
{
  if (argc!=2)
    {
      cerr << "ERROR: need port number on command line!" << endl;
      return 1;
    }
  return doServer( atoi(argv[1]) );
}

int doServer(int onPort)
{
  // build a listening "socket"
  int listen_sock = socket ( AF_INET, SOCK_STREAM, 0 );

  if (listen_sock<0)
    {
      cerr << "Could not create listening socket!" << endl;
      return 1; 
    }

  // start building the erver address for the socket
  struct sockaddr_in local_addr;

  local_addr.sin_family = AF_INET;            //IPv4 address
  local_addr.sin_addr.s_addr = INADDR_ANY;//use any of my addresses
  local_addr.sin_port = htons (onPort);       //port for server to use

  if (bind(listen_sock, (sockaddr *) &local_addr, sizeof(local_addr) ) )
    {
      cerr << "bind() failed - possible causes include:" << endl
	   << " * an invalid port number (already in use? restricted?)" << endl
	   << " * an invalid local address (wildcard?)" << endl;
      return 1;
    }

  if (listen(listen_sock, MAX_WAITERS )!=0)
    {
      cerr << "listen() failed" << endl;
      return 1;
    }

  // handle one client each pass through loop 
  while(true)
    {
      int connected_sock;       // id for incoming client socket
      struct sockaddr_in from;  // address info from client
      unsigned int from_len;    // byte count of incoming address

      from_len = sizeof(from);

      // now have a "live" socket connecting client to this server
      connected_sock = accept(listen_sock,
			      (struct sockaddr *) &from,
			      &from_len);

      doWork(connected_sock, &from); 
    }
}

void doWork(int connsock, struct sockaddr_in *clientInfo)
{
  string buffer;

  // build a string with the current time in it
  time_t currTime = time(nullptr);
  buffer = "current time is: ";
  buffer+= ctime(&currTime);

  char *cbuff = (char *) buffer.c_str();
  int needed = buffer.length();
  
  
  // send the contents of buffer back to client.
  while (needed>0)
    {
      int numWritten = write(connsock, cbuff, needed);
      needed -= numWritten;
      cbuff += numWritten; 
    }
  // print a "log" message about the client
  cout << "Connection from: " << inet_ntoa(clientInfo->sin_addr) << endl; 
    
  close(connsock); 
}
