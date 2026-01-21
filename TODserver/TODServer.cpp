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

int main(int argc, char *argv)
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
  int listen_socket = socket ( AF_INET, SOCK_STREAM, 0 );

  if (listen_sock<0)
    {
      cerr << "Could not create listening socket!" << endl;
      return 1; 
    }

  // start building the erver address for the socket
  struct sockaddr_in local_addr;

  local_addr.sin_family = AF_INET;            //IPv4 address
  local_addr.sin_sin_addr.s_addr = INADDR_ANY;//use any of my addresses
  local_addr.sin_port = htons (onPort);       //port for server to use

  if (bind(listen_socket, (sockaddr *) &local_addr, sizeof(local_addr) ) )
    {
      cerr << "bind() failed - possible causes include:" << endl
	   << " * an invalid port number (already in use? restricted?)" << endl
	   << " * an invalid local address (wildcard?)" << endl;
      return 1;
    }
}
