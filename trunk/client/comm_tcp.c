/* Network code and main loop. This is the only place where platform-specific
 * code is allowed. */


#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#include "common.h"


int server_socket;


int connect_to_server( char *hostname, int port )
{
   struct sockaddr_in server;
   struct hostent *host;
   int sock;
   
   if ( ( sock = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
     {
	perror( "socket" );
	return 1;
     }
   
   host = gethostbyname( hostname );
   if ( !host )
     {
	fprintf( stderr, "Cannot resolve the host '%s'.\n", hostname );
	return 1;
     }
   
   server.sin_family = AF_INET;
   server.sin_port = htons( port );
   server.sin_addr = *(struct in_addr*) host->h_addr;
   
   if ( connect( sock, (struct sockaddr*) &server, sizeof(server) ) )
     {
	die( "Connection failed." );
	return 1;
     }
   
   server_socket = sock;
   
   return 0;
}


int receive_one_packet( )
{
   char buf[4096];
   int bytes;
   
   bytes = read( server_socket, buf, 4096 );
   if ( bytes < 0 )
     {
	die( "Connection error." );
	return 1;
     }
   if ( bytes == 0 )
     {
	die( "Connection closed by server." );
	return 1;
     }
   
   parse_data( buf, bytes );
   
   return 0;
}



void send_to_server( char *msg, int bytes )
{
   if ( !server_socket )
     {
	die( "Trying to send without a socket." );
	return;
     }
   
   write( server_socket, msg, bytes );
}

