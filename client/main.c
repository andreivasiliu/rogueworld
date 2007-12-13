/* Main file for the RogueWorld Client. */


#include <stdlib.h>
#include <unistd.h> // for pause()
#include <curses.h>

#include "common.h"


void die( char *reason )
{
   endwin( );
   puts( reason );
   
   exit( 1 );
}



int enter_world( char *name )
{
   send_login( name );
   
   /* Enter the initial main-loop, in which we gather all entrance packets. */
   while ( 1 /* !map || !player */ )
     {
	if ( receive_one_packet( ) )
	  return 1;
     }
   
   return 0;
}



int connect_user( char *name, char *hostname, int port )
{
   mvaddstr( 6, 2, "Connecting..." );
   refresh( );
   
   if ( connect_to_server( hostname, port ) )
     return 1;
   
   addstr( " done." );
   refresh( );
   
   if ( enter_world( name ) )
     return 1;
   
   return 0;
}


int main( int argc, char *argv[] )
{
   char name[32];
   char port[16];
   
   initscr( );
   
   mvaddstr( 2, 2, "Character name: " );
   getnstr( name, 20 );
   
   mvaddstr( 3, 2, "Server: localhost" );
   mvaddstr( 4, 2, "Port: " );
   getnstr( port, 5 );
   
   if ( connect_user( name, "127.0.0.1", atoi(port) ) )
     return 1;
   
   // interface
   
   endwin( );
   
   return 0;
}

