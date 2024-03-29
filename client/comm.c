/* Network code and main loop. This is the only place where platform-specific
 * code is allowed. */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>


#include "common.h"


int server_socket;
CONN *server;


int connect_to_server( char *hostname, int port, int type )
{
   struct sockaddr_in server_sa;
   struct hostent *host;
   int sock;
   
   if ( ( sock = socket( PF_INET, type ? SOCK_STREAM : SOCK_DGRAM, 0 ) ) < 0 )
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
   
   server_sa.sin_family = AF_INET;
   server_sa.sin_port = htons( port );
   server_sa.sin_addr = *(struct in_addr*) host->h_addr;
   
   if ( connect( sock, (struct sockaddr*) &server_sa, sizeof(server_sa) ) )
     {
	die( "Connection failed." );
	return 1;
     }
   
   server_socket = sock;
   server = calloc( 1, sizeof( CONN ) );
   server->socket = sock;
   
   create_packet_queue( server );
   
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



void send_to_connection( CONN *c, char *msg, int bytes )
{
   write( c->socket, msg, bytes );
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


void close_connection( )
{
   if ( server_socket )
     {
	send_close( "Client quit." );
	close( server_socket );
     }
}


int main_loop( )
{
   struct timeval pulsetime;
   struct timeval lasttick, now;
   fd_set in_set;
   int max_fd;
   
   gettimeofday( &lasttick, NULL );
   
   while( 1 )
     {
	FD_ZERO( &in_set );
	
	/* Socket events. */
	FD_SET( server_socket, &in_set );
	/* Keyboard (stdin) events. */
	FD_SET( 0, &in_set );
	
	/* I think it's safe to say that, whatever the value, it's > 0. */
	max_fd = server_socket;
	
	pulsetime.tv_sec = 0;
	pulsetime.tv_usec = 25000;
	
	select( max_fd + 1, &in_set, NULL, NULL, &pulsetime );
	
	gettimeofday( &now, NULL );
	now.tv_usec -= 200000;
	if ( now.tv_usec < 0 )
	  {
	     now.tv_usec += 1000000;
	     now.tv_sec -= 1;
	  }
	
	if ( timercmp( &lasttick, &now, < ) )
	  {
	     tick_event( );
	     gettimeofday( &lasttick, NULL );
	     continue;
	  }
	
	if ( FD_ISSET( server_socket, &in_set ) )
	  receive_one_packet( );
	else if ( FD_ISSET( 0, &in_set ) )
	  key_event( );
     }
}

