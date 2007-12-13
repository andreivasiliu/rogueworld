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
   
   pl->next = players;
   players = pl;
   
   return pl;
}


void player_file( char *name, char *dest )
{
   char *dir = DATA "/players/";
   char *p;
   
   for ( p = dir; *p; p++ )
     *(dest++) = *(p++);
   
   for ( p = name; *p; p++ )
     {
	if ( *p >= 'A' && *p <= 'Z' )
	  *(dest++) = *(p++) - 'A' + 'a';
	else
	  *(dest++) = *(p++);
     }
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
   
   fclose( fl );
   
   pl->next = players;
   players = pl->next;
   
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
   
   fclose( fl );
}



void destroy_player( PLAYER *pl )
{
   PLAYER *prev;
   
   free( pl->name );
   
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
   
   debugf( "%s logged in.", pl->name );
   
   
   
   
}


void pl_disconnected( PLAYER *player, int by_error )
{
   /* Do we care? */
   if ( !player )
     return;
   
   
}

