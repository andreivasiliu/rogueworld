/* Handle key presses and tick events. For now, it also houses the full
 * interface management. */


#include <curses.h>

#include "common.h"


/* If not null, pass key-presses to THIS function. */
void (*current_handler)( int key );


int cursor_tick;
int cursor_changed;
int cursor_y;
int cursor_x;


void move_cursor( int y, int x )
{
   OBJECT *body;
   
   body = player->body;
   
   if ( !body )
     return;
   
   cursor_y += y;
   cursor_x += x;
   
   if ( cursor_y < body->pos_y + -3 )
     cursor_y = body->pos_y + -3;
   if ( cursor_y > body->pos_y + 3 )
     cursor_y = body->pos_y + 3;
   
   if ( cursor_x < body->pos_x + -3 )
     cursor_x = body->pos_x + -3;
   if ( cursor_x > body->pos_x + 3 )
     cursor_x = body->pos_x + 3;
   
   cursor_tick = 1;
   
   /* Don't send it too many times. Just once a tick. */
   if ( !cursor_changed )
     send_setcursor( cursor_y, cursor_x );
   cursor_changed = 1;
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
      case 'q':		die( "Client closed." ); break;
     }
   
   draw_interface( );
}


void tick_event( )
{
   cursor_tick = !cursor_tick;
   if ( cursor_changed )
     {
	cursor_changed = 0;
	send_setcursor( cursor_y, cursor_x );
     }
   
   draw_interface( );
}


void check_spot( short pos_y, short pos_x, void *res )
{
   if ( map->map[pos_y*map->width+pos_x] != '.' )
     *(int *)res = 0;
}



#define SQUARE(x) ((x)*(x))
int dist( short y1, short x1, short y2, short x2 )
{
   return SQUARE( y1 - y2 ) + SQUARE( x1 - x2 );
}


int player_can_see( short pos_y, short pos_x )
{
   void Bresenham(short x1, short y1, short x2, short y2,
		  void (*delegate)( short y, short x, void *p ), void *p );
   int result = 1;
   
   if ( !player->body )
     return 0;
   
   if ( dist( player->body->pos_y, player->body->pos_x,
	      pos_y, pos_x ) >= 100 )
     return 0;
   
   Bresenham( player->body->pos_x, player->body->pos_y,
	      pos_x, pos_y, check_spot, &result );
   
   return result;
}


void draw_map( )
{
   int y, x;
   
   for ( y = 0; y < map->height; y++ )
     {
	move( y, 0 );
	for ( x = 0; x < map->width; x++ )
	  {
	     if ( !player_can_see( y, x ) )
	       attrset( COLOR_PAIR(1) | A_BOLD );
	     else
	       attrset( A_NORMAL );
	     
	     addch( map->map[y*map->width+x] );
	  }
     }
   
   attrset( A_NORMAL );
}


void draw_objects( )
{
   OBJECT *obj;
   
   for ( obj = objects; obj; obj = obj->next )
     {
	move( obj->pos_y, obj->pos_x );
	
	if ( obj->type == OBJ_YOU )
	  addch( '@' | A_BOLD );
	else
	  addch( '@' );
     }
}


void draw_cursor( )
{
   void Bresenham(short x1, short y1, short x2, short y2,
		  void (*delegate)( short y, short x, void *p ), void *p );
   
   if ( !cursor_tick || !player->body )
     return;
   
   if ( cursor_x == player->body->pos_x &&
	cursor_y == player->body->pos_y )
     return;
   
   /* Out of screen bounds. */
   if ( cursor_x < 0 ||
	cursor_y < 0 )
     return;
   
   mvaddch( cursor_y, cursor_x, '+' | A_BOLD );
}


void draw_stats( )
{
   move( LINES-3, 1 );
   printw( "Player: %s", player->name );
   move( LINES-2, 1 );
   if ( player->body )
     printw( "Pos: y%d/x%d", player->body->pos_y, player->body->pos_x );
   else
     printw( "I don't know where my body is!" );
}


void draw_interface( )
{
   erase( );
   
   draw_map( );
   draw_objects( );
   draw_cursor( );
   
   draw_stats( );
   
   refresh( );
}


