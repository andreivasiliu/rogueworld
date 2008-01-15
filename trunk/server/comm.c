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
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "common.h"


CONN *connections;


#if defined(WIN32)
# define CLOSE(sock)	closesocket(sock)
#else
# define CLOSE(sock)	close(sock)
#endif


#if defined(WIN32)
int init_wsa( )
{
   /* Initialize WSA. */
   WORD wVersionRequested;
   WSADATA wsaData;
   wVersionRequested = MAKEWORD( 1, 0 );
   
   if ( WSAStartup( wVersionRequested, &wsaData ) )
     {
	return -1;
     }
   
   if ( LOBYTE( wsaData.wVersion ) != 1 ||
	HIBYTE( wsaData.wVersion ) != 0 )
     {
	WSACleanup( );
	return -1;
     }
   /* End of WSA. */
   
   return 0;
}
#endif


int init_udp_server( int port )
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int fd, x = 1;
   
   if ( ( fd = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 )
     {
	debugf( "init_udp_server: socket: %s.", strerror( errno ) );
	return -1;
     }

   if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
		    (char *) &x, sizeof( x ) ) < 0 )
     {
	debugf( "init_udp_server: SO_REUSEADDR: %s.", strerror( errno ) );
	CLOSE( fd );
	return -1;
     }
   
   sa            = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port   = htons( port );

   if ( bind( fd, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
     {
	debugf( "init_udp_server: bind: %s.", strerror( errno ) );
	CLOSE( fd );
	return -1;
     }
   
   return fd;
}


int init_tcp_server( int port )
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int fd, x = 1;
   
   if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
     {
	debugf( "init_tcp_server: socket: %s.", strerror( errno ) );
	return -1;
     }

   if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
		    (char *) &x, sizeof( x ) ) < 0 )
     {
	debugf( "init_tcp_server: SO_REUSEADDR: %s.", strerror( errno ) );
	CLOSE( fd );
	return -1;
     }
   
   sa            = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port   = htons( port );

   if ( bind( fd, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
     {
	debugf( "init_tcp_server: bind: %s.", strerror( errno ) );
	CLOSE( fd );
	return -1;
     }

   if ( listen( fd, 1 ) < 0 )
     {
	debugf( "init_tcp_server: listen: %s.", strerror( errno ) );
	CLOSE( fd );
	return -1;
     }
   
   return fd;
}


int new_udp_connection ( int control, int port )
{
   struct sockaddr_in addr, sa;
   static struct sockaddr_in sa_zero;
   unsigned int size;
   char buf[4096];
   int new_sock, bytes, x = 1;
   CONN *c;
   
   size = sizeof( addr );
   
   /*Receive a package from a new player*/
   if ( ( bytes = recvfrom ( control, buf, 4096, 0, ( struct sockaddr * ) &addr, &size ) ) < 0 )
     {
	debugf ( "new_udp_connection: recvfrom: %s.", strerror( errno ) );
	return 1;
     }
   
   /*Create a new socket for it*/
   if ( ( new_sock = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 )
     {
	debugf( "new_udp_connection: socket: %s.", strerror( errno ) );
	return -1;
     }
   
   if ( setsockopt( new_sock, SOL_SOCKET, SO_REUSEADDR,
		    (char *) &x, sizeof( x ) ) < 0 )
     {
	debugf( "new_udp_connection: SO_REUSEADDR: %s.", strerror( errno ) );
	CLOSE( new_sock );
	return -1;
     }
   
   sa            = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port   = htons( port );

   /*Binding...*/
   if ( bind( new_sock, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
     {
	debugf( "new_udp_connection: bind: %s.", strerror( errno ) );
	CLOSE( new_sock );
	return -1;
     }

   
   if ( connect ( new_sock, ( struct sockaddr* )&addr, size ) < 0 )
     {
	debugf ( "new_udp_connection: connect: %s.", strerror ( errno ) );
	return 1;
     }
   
   debugf( "New UDP connection: %s", inet_ntoa( addr.sin_addr ) );
   
   /*Make a new connection*/
   c = calloc ( 1, sizeof ( CONN ) );
   c->sock = new_sock;
   c->player = NULL;
   
   /*Link it*/
   c->next = connections;
   connections = c;
   
   create_packet_queue( c );
   
   /*Doing something with the package received...*/
   parse_data( c, buf, bytes );
   
   return 0;
}


int new_tcp_connection( int control )
{
   struct sockaddr_in addr;
   unsigned int size;
   int desc;
   CONN *c;
   
   size = sizeof( addr );
   if ( ( desc = accept( control, (struct sockaddr *) &addr, &size) ) < 0 )
     {
	debugf( "new_tcp_connection: accept: %s.", strerror( errno ) );
	return 1;
     }
   
   debugf( "New TCP connection: %s", inet_ntoa( addr.sin_addr ) );
   
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
   
   create_packet_queue( c );
   
   return 0;
}


void destroy_connection( CONN *ze_unfortunate )
{
   CONN *c;
   
   destroy_packet_queue( ze_unfortunate );
   
   if ( ze_unfortunate->player )
     destroy_player( ze_unfortunate->player );
   
   CLOSE( ze_unfortunate->sock );
   
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


void send_to_connection( CONN *c, char *data, int bytes )
{
   write( c->sock, data, bytes );
}


int main_loop( int port )
{
   struct timeval pulsetime;
   struct timeval lasttick, now;
   fd_set in_set;
   fd_set out_set;
   fd_set exc_set;
   int udp_server, tcp_server, max_fd;
   CONN *c;
   
#if defined(WIN32)
   if ( init_wsa( ) < 0 )
     return 1;
#endif
   
   if ( ( tcp_server = init_tcp_server( port ) ) < 0 )
     return 1;

   if ( ( udp_server = init_udp_server( port ) ) < 0 )
     return 1;
   
   gettimeofday( &lasttick, NULL );
   
   while ( 1 )
     {
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	
	FD_SET( udp_server, &in_set );
	FD_SET( tcp_server, &in_set );
	if ( udp_server > tcp_server )
	  max_fd = udp_server;
	else 
	  max_fd = tcp_server;
	
	/* What descriptors do we want to select? */
	for ( c = connections; c; c = c->next )
	  {
	     FD_SET( c->sock, &in_set );
	     
	     if ( max_fd < c->sock )
	       max_fd = c->sock;
	  }
	
	pulsetime.tv_sec = 0;
	pulsetime.tv_usec = 50000;
	
	select( max_fd + 1, &in_set, &out_set, &exc_set, &pulsetime );
	
	gettimeofday( &now, NULL );
	now.tv_usec -= 250000;
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
	
	/* New connections. */
	if ( FD_ISSET( udp_server, &in_set ) )
	  {
	     if ( new_udp_connection( udp_server, port ) )
	       return 1;
	  }
	if ( FD_ISSET( tcp_server, &in_set ) )
	  {
	     if ( new_tcp_connection( tcp_server ) )
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
