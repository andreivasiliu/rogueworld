/* This handles players.. actually, it mostly only handles player commands,
 * so it might be renamed later.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


/* List of logged-in players. */
PLAYER *players;


PLAYER *create_player( char *name )
{
   PLAYER *pl;
   
   pl = calloc( 1, sizeof( PLAYER ) );
   pl->name = strdup( name );
   pl->body_pos_x = 2;
   pl->body_pos_y = 2;
   
   pl->cursor_y = pl->body_pos_y;
   pl->cursor_x = pl->body_pos_x;
   
   debugf( "New player: %s.", pl->name );
   
   return pl;
}


void player_file( char *name, char *dest )
{
   char *dir = DATA "/players/";
   char *p;
   
   for ( p = dir; *p; p++ )
     *(dest++) = *p;
   
   for ( p = name; *p; p++ )
     {
	if ( *p >= 'A' && *p <= 'Z' )
	  *(dest++) = *p - 'A' + 'a';
	else
	  *(dest++) = *p;
     }
   
   *dest = 0;
}



PLAYER *load_player( char *name )
{
   PLAYER *pl;
   FILE *fl;
   char buf[256];
   
   player_file( name, buf );
   fl = fopen( buf, "r" );
   if ( !fl )
     return NULL;
   
   pl = calloc( 1, sizeof( PLAYER ) );
   
   fscanf( fl, "Name: %s\n", buf );
   pl->name = strdup( buf );
   fscanf( fl, "PosY: %hd\n", &pl->body_pos_y );
   fscanf( fl, "PosX: %hd\n", &pl->body_pos_x );
   
   fclose( fl );
   
   pl->cursor_y = pl->body_pos_y;
   pl->cursor_x = pl->body_pos_x;
   
   debugf( "%s logged in.", pl->name );
   
   return pl;
}



void save_player( PLAYER *pl )
{
   FILE *fl;
   char buf[256];
   
   player_file( pl->name, buf );
   fl = fopen( buf, "w" );
   if ( !fl )
     {
	debugf( "Critical: Cannot open '%s' for writing.", buf );
	return;
     }
   
   fprintf( fl, "Name: %s\n", pl->name );
   if ( pl->persona )
     {
	fprintf( fl, "PosY: %hd\n", pl->persona->pos_y );
	fprintf( fl, "PosX: %hd\n", pl->persona->pos_x );
     }
   else
     {
	fprintf( fl, "PosY: %hd\n", pl->body_pos_y );
	fprintf( fl, "PosX: %hd\n", pl->body_pos_x );
     }
   
   fclose( fl );
}



void destroy_player( PLAYER *pl )
{
   PLAYER *prev;
   
   debugf( "%s logged out.", pl->name );
   
   if ( pl == players )
     players = players->next;
   else
     {
	for ( prev = players; prev; prev = prev->next )
	  if ( prev->next == pl )
	    {
	       prev->next = pl->next;
	       break;
	    }
     }
   
   if ( pl->persona )
     destroy_object( pl->persona );
   free( pl->name );
   free( pl );
}



void unload_player( PLAYER *pl )
{
   save_player( pl );
   
   destroy_player( pl );
}


void pl_login( CONN *c, char *name )
{
   PLAYER *pl;
   char *p;
   
   if ( strlen( name ) > 20 )
     {
	kill_connection( c, "Name too large." );
	return;
     }
   
   for ( p = name; *p; p++ )
     if ( ( *p < 'a' || *p > 'z' ) && ( *p < 'A' || *p > 'Z' ) )
       {
	  kill_connection( c, "Name contains illegal characters." );
	  return;
       }
   
   
   pl = load_player( name );
   
   if ( !pl )
     pl = create_player( name );
   
   c->player = pl;
   pl->connection = c;
   
   send_map( c, &map );
   send_userinfo( c, pl );
}


void pl_disconnected( PLAYER *player, int by_error )
{
   /* Do we care? */
   if ( !player )
     return;
   
   unload_player( player );
}


void pl_enterworld( PLAYER *player )
{
   PLAYER *p;
   
   for ( p = players; p; p = p->next )
     if ( p == player )
       {
	  debugf( "%s thought he wasn't already in the world.", p->name );
	  return;
       }
   
   player->next = players;
   players = player;
   
   player->persona = create_object( OBJ_PLAYER );
   player->persona->player = player; /* Don't ask. */
   place_object( player->persona, player->body_pos_y, player->body_pos_x );
   
   debugf( "%s has entered the world.", player->name );
}

