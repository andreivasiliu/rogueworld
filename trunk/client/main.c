/* Main file for the RogueWorld Client. */


#include <stdlib.h>
#include <unistd.h> // for pause()
#include <curses.h>

#include "common.h"


MAP *map;
PLAYER *player;


void die( char *reason )
{
   endwin( );
   puts( reason );
   
   exit( 1 );
}



int enter_world( char *name )
{
   int c;
   
   send_login( name );
   
   /* Enter the initial main-loop, in which we gather all entrance packets. */
   while ( !map || !player )
     {
	if ( receive_one_packet( ) )
	  return 1;
     }
   
   mvaddstr( 11, 2, "Ready. [ENTER] to begin, or anything else to leave.." );
   
   c = getch( );
   
   if ( c != '\n' && c != '\r' && c != KEY_ENTER )
     {
	die( "Alright, goodbye." );
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
   cbreak( );
   nonl( );
   keypad( stdscr, TRUE );
   
   echo( );
   mvaddstr( 2, 2, "Character name: " );
   getnstr( name, 20 );
   
   mvaddstr( 3, 2, "Server: localhost" );
   mvaddstr( 4, 2, "Port: " );
   getnstr( port, 5 );
   
   noecho( );
   
   if ( connect_user( name, "127.0.0.1", atoi(port) ) )
     return 1;
   
   draw_interface( );
   main_loop( );
   
   endwin( );
   
   return 0;
}

