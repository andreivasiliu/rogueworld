/* Main file for the RogueWorld Client. */


#include <stdlib.h>
#include <unistd.h> // for pause()
#include <curses.h>

#include "common.h"


MAP *map;
PLAYER *player;


void die( char *reason )
{
   close_connection( reason );
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
   
   mvaddstr( 13, 2, "Ready. [ENTER] to begin, or anything else to leave.." );
   
   c = getch( );
   
   if ( c != '\n' && c != '\r' && c != KEY_ENTER )
     {
	die( "Alright, goodbye." );
     }
   
   send_enterworld( );
   
   return 0;
}



int connect_user( char *name, char *hostname, int port, int protocol_type )
{
   move( 8, 2 );
   clrtoeol( );
   addstr( "Connecting..." );
   refresh( );
   
   if ( connect_to_server( hostname, port, protocol_type ) )
     return 1;
   
   addstr( " done." );
   refresh( );
   
   if ( enter_world( name ) )
     return 1;
   
   return 0;
}


void print_protocol( int type )
{
   move( 6, 13 );
   
   attrset( A_NORMAL );
   
   addch( '[' | A_BOLD );
   /* UDP */
   if ( type == 0 )
     addstr( "UDP" );
   else
     addstr( "TCP" );
   addch( ']' | A_BOLD );
}


int main( int argc, char *argv[] )
{
   char name[32] = "";
   char hostname[256] = "localhost";
   char port[16] = "1623";
   int protocol_type = 0;
   int c;
   
   initscr( );
   start_color( );
   cbreak( );
   nonl( );
   keypad( stdscr, TRUE );
   
   init_pair( 1, COLOR_BLACK, COLOR_BLACK );
   init_pair( 2, COLOR_CYAN, COLOR_BLACK );
   
   attrset( A_BOLD );
   mvaddstr( 2, 2, "Character name: " );
   attrset( A_NORMAL );
   echo( );
   while ( !name[0] )
     {
	move( 2, 18 );
	getnstr( name, 20 );
     }
   noecho( );
   
   attrset( A_BOLD );
   mvaddstr( 3, 2, "Server" );
   mvaddstr( 4, 3, "[h] Host: " );
   attrset( A_NORMAL );
   addstr( hostname );
   mvaddch( 4, 4, 'h' | COLOR_PAIR(2) | A_BOLD );
   
   attrset( A_BOLD );
   mvaddstr( 5, 3, "[p] Port: " );
   attrset( A_NORMAL );
   addstr( port );
   mvaddch( 5, 4, 'p' | COLOR_PAIR(2) | A_BOLD );
   
   attrset( A_BOLD );
   mvaddstr( 6, 3, "[t] Type: " );
   attrset( A_NORMAL );
   print_protocol( protocol_type );
   mvaddch( 6, 4, 't' | COLOR_PAIR(2) | A_BOLD );
   
   mvaddstr( 8, 3, "Press [ENTER] to connect." );
   
   while ( 1 )
     {
	move( 8, 28 );
	
	c = getch( );
	
	if ( c == 't' )
	  {
	     protocol_type = !protocol_type;
	     print_protocol( protocol_type );
	  }
	if ( c == 'h' )
	  {
	     move( 4, 13 );
	     clrtoeol( );
	     echo();
	     getnstr( hostname, 80 );
	     noecho();
	  }
	if ( c == 'p' )
	  {
	     move( 5, 13 );
	     clrtoeol( );
	     echo();
	     getnstr( port, 5 );
	     noecho();
	  }
	if ( c == KEY_ENTER || c == '\n' || c == '\r' )
	  {
	     if ( atoi(port) < 1 || atoi(port) > 65535 )
	       {
		  move( 5, 13 );
		  clrtoeol( );
		  addstr( "(invalid port)" );
		  continue;
	       }
	     else if ( !hostname[0] )
	       {
		  move( 4, 13 );
		  clrtoeol( );
		  addstr( "(invalid hostname)" );
		  continue;
	       }
	     
	     break;
	  }
     }
   
   if ( connect_user( name, "127.0.0.1", atoi(port), protocol_type ) )
     return 1;
   
   draw_interface( );
   main_loop( );
   
   endwin( );
   
   return 0;
}

