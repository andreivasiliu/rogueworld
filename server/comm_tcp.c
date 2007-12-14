/* Network and Main Loop.
 * This is the only file that will allow platform-specific code.
 */


#if !defined(WIN32)
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
#else
# include <winsock2.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "common.h"


CONN *connections;


int init_server( int port )
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int fd, x = 1;
   
#if defined(WIN32)
     {
        /* Initialize WSA. */
        WORD wVersionRequested;
        WSADATA wsaData;
        wVersionRequested = MAKEWORD( 1, 0 );
        
        if ( WSAStartup( wVersionRequested, &wsaData ) )
          {
             return 0;
          }
        
        if ( LOBYTE( wsaData.wVersion ) != 1 ||
             HIBYTE( wsaData.wVersion ) != 0 )
          {
             WSACleanup( );
             return 0;
          }
        /* End of WSA. */
     }
#endif
   
   if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
     {
	debugf( "init_server: socket: %s.", strerror( errno ) );
	return -1;
     }

   if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
		    (char *) &x, sizeof( x ) ) < 0 )
     {
	debugf( "init_server: SO_REUSEADDR: %s.", strerror( errno ) );
	close( fd );
	return -1;
     }
   
   sa            = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port   = htons( port );

   if ( bind( fd, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
     {
	debugf( "init_server: bind: %s.", strerror( errno ) );
	close( fd );
	return -1;
     }

   if ( listen( fd, 1 ) < 0 )
     {
	debugf( "init_server: listen: %s.", strerror( errno ) );
	close( fd );
	return -1;
     }
   
   return fd;
}


int new_connection( int control )
{
   struct sockaddr_in addr;
   unsigned int size;
   int desc;
   CONN *c;
   
   size = sizeof( addr );
   if ( ( desc = accept( control, (struct sockaddr *) &addr, &size) ) < 0 )
     {
	debugf( "new_connection: accept: %s.", strerror( errno ) );
	return 1;
     }
   
   debugf( "New connection: %s", inet_ntoa( addr.sin_addr ) );
   
   /* In future, we could resolve it:
    * from = gethostbyaddr( (char *) &sock.sin_addr,
    *                       sizeof(sock.sin_addr), AF_INET );
    * But not now.
    */
   
   /* Make a new connection. */
   c = calloc( 1, sizeof( CONN ) );
   c->sock = desc;
   c->player = NULL;
   
   /* Link it. */
   c->next = connections;
   connections = c;
   
   return 0;
}


void destroy_connection( CONN *ze_unfortunate )
{
   CONN *c;
   
   close( ze_unfortunate->sock );
   
   /* First one in the chain. */
   if ( connections == ze_unfortunate )
     {
	connections = connections->next;
     }
   else
     {
	for ( c = connections; c; c = c->next )
	  if ( c->next == ze_unfortunate )
	    {
	       c->next = ze_unfortunate->next;
	       break;
	    }
     }
   
   free( ze_unfortunate );
}



int read_data( CONN *conn )
{
   char buf[4096];
   int bytes;
   
   bytes = read( conn->sock, buf, 4096 );
   
   /* Connection error. */
   if ( bytes < 0 )
     {
	pl_disconnected( conn->player, 1 );
	destroy_connection( conn );
	
	return 0;
     }
   /* Connection closed. */
   if ( bytes == 0 )
     {
	pl_disconnected( conn->player, 0 );
	destroy_connection( conn );
	
	return 0;
     }
   
   parse_data( conn, buf, bytes );
   
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

