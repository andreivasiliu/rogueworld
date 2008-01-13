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
   
   send_enterworld( );
   
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


void print_protocol( int type )
{
   /* UDP */
   if ( type == 0 )
     {
	attrset( A_BOLD );
	addch( '[' | A_NORMAL );
	addstr( "UDP" );
	addch( ']' | A_NORMAL );
	attrset( A_NORMAL );
	addstr( "  TCP" );
     }
   else
     {
	attrset( A_NORMAL );
	addstr( " UDP  " );
	attrset( A_BOLD );
	addch( '[' | A_NORMAL );
	addstr( "TCP" );
	addch( ']' | A_NORMAL );
	attrset( A_NORMAL );
     }
}


int main( int argc, char *argv[] )
{
   const char *default_hostname = "localhost";
   const char *default_port = "1623";
   char name[32];
   char hostname[256];
   char port[16];
   int protocol_type = 0;
   int c;
   
   initscr( );
   start_color( );
   cbreak( );
   nonl( );
   keypad( stdscr, TRUE );
   
   init_pair( 1, COLOR_BLACK, COLOR_BLACK );
   
   attrset( A_BOLD );
   mvaddstr( 2, 2, "Character name: " );
   attrset( A_NORMAL );
   echo( );
   getnstr( name, 20 );
   noecho( );
   
   strcpy( port, "1623" );
   /*
   attrset( A_BOLD );
   mvaddstr( 3, 2, "Server" );
   mvaddstr( 4, 3, "[h] Host: " );
   attrset( A_NORMAL );
   addstr( default_hostname );
   mvaddch( 4, 4, 'h' | COLOR_PAIR(1) | A_BOLD );
   
   attrset( A_BOLD );
   mvaddstr( 5, 3, "[p] Port: " );
   attrset( A_NORMAL );
   addstr( default_port );
   mvaddch( 5, 4, 'p' | COLOR_PAIR(1) | A_BOLD );
   
   attrset( A_BOLD );
   mvaddstr( 6, 3, "[t] Type: " );
   attrset( A_NORMAL );
   print_protocol( protocol_type );
   mvaddch( 6, 4, 't' | COLOR_PAIR(1) | A_BOLD );
   
   while ( 1 )
     {
	c = getch( );
     }
   
   getnstr( port, 5 );
    */
   
   
   if ( connect_user( name, "127.0.0.1", atoi(port) ) )
     return 1;
   
   draw_interface( );
   main_loop( );
   
   endwin( );
   
   return 0;
}

