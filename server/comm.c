/* Network and Main Loop.
 * This is the only file that will allow platform-specific code.
 */


#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include "common.h"


typedef struct connection_data CONN;

struct connection_data
{
   int sock;
   
   CONN *next;
};

CONN *connections;


int init_server( int port )
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int fd, x = 1;
   
   
   if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
     {
	debugf( "init_server: socket: %s.", strerror( errno ) );
	return -1;
     }

   if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
		    (char *) &x, sizeof( x ) ) < 0 )
     {
	debugf( "init_server: SO_REUSEADDR: %s.", strerror( errno ) );
	shutdown( fd, SHUT_RDWR );
	return -1;
     }
   
   sa            = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port   = htons( port );

   if ( bind( fd, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
     {
	debugf( "init_server: bind: %s.", strerror( errno ) );
	shutdown( fd, SHUT_RDWR );
	return -1;
     }

   if ( listen( fd, 1 ) < 0 )
     {
	debugf( "init_server: listen: %s.", strerror( errno ) );
	shutdown( fd, SHUT_RDWR );
	return -1;
     }
   
   return fd;
}


int new_connection( int server )
{
   
   
   return 0;
}


int read_data( CONN *conn )
{
   
   
   return 0;
}


int main_loop( int port )
{
   //struct timeval pulsetime;
   fd_set in_set;
   fd_set out_set;
   fd_set exc_set;
   int server, max_fd;
   CONN *c;
   
   server = init_server( port );
   if ( server < 0 )
     return 1;
   
   while ( 1 )
     {
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	
	FD_SET( server, &in_set );
	max_fd = server;
	
	/* What descriptors do we want to select? */
	for ( c = connections; c; c = c->next )
	  {
	     FD_SET( c->sock, &in_set );
	     
	     if ( max_fd < c->sock )
	       max_fd = c->sock;
	  }
	
	select( max_fd + 1, &in_set, &out_set, &exc_set, NULL );
	
	/* New connection. */
	if ( FD_ISSET( server, &in_set ) )
	  {
	     if ( new_connection( server ) )
	       return 1;
	  }
	
	/* Go through the connection list. */
	for ( c = connections; c; c = c->next )
	  if ( FD_ISSET( c->sock, &in_set ) )
	    {
	       if ( read_data( c ) )
		 return 1;
	       
	       /* Don't continue.. the connections list might've changed. */
	       break;
	    }
     }
}

