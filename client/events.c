/* Handle key presses and tick events. For now, it also houses the full
 * interface management. */


#include <curses.h>

#include "common.h"


/* If not null, pass key-presses to THIS function. */
void (*current_handler)( int key );


int cursor_y;
int cursor_x;


void move_cursor( int y, int x )
{
   cursor_y += y;
   cursor_x += x;
   
   if ( cursor_y < -3 )
     cursor_y = -3;
   if ( cursor_y > 3 )
     cursor_y = 3;
   
   if ( cursor_x < -3 )
     cursor_x = -3;
   if ( cursor_x > 3 )
     cursor_x = 3;
}



void key_event( )
{
   int key;
   
   key = getch( );
   
   if ( current_handler )
     {
	current_handler( key );
	return;
     }
   
   switch ( key )
     {
      case KEY_LEFT:	move_cursor( 0, -1 ); break;
      case KEY_RIGHT:	move_cursor( 0, 1 ); break;
      case KEY_A1:
      case KEY_UP:	move_cursor( -1, 0 ); break;
      case KEY_DOWN:	move_cursor( 1, 0 ); break;
     }
   
   draw_interface( );
}


void draw_map( )
{
   int y, x;
   
   for ( y = 0; y < map->height; y++ )
     {
	move( y, 0 );
	for ( x = 0; x < map->width; x++ )
	  addch( map->map[y*map->width+x] );
     }
}


void draw_objects( )
{
   move( player->pos_y, player->pos_x );
   addch( '@' );
}


void draw_cursor( )
{
   if ( !cursor_x && !cursor_y )
     return;
   
   /* Out of screen bounds. */
   if ( player->pos_x + cursor_x < 0 ||
	player->pos_y + cursor_y < 0 )
     return;
   
   mvaddch( player->pos_y + cursor_y,
	    player->pos_x + cursor_x,
	    '+' );
}


void draw_stats( )
{
   move( LINES-3, 1 );
   printw( "Player: %s", player->name );
   move( LINES-2, 1 );
   printw( "Pos: y%d/x%d", player->pos_y, player->pos_x );
}


void draw_interface( )
{
   clear( );
   
   draw_map( );
   draw_objects( );
   draw_cursor( );
   
   draw_stats( );
   
   refresh( );
}

