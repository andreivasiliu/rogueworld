/* RogueWorld server main file.
 * Developers:
 *   Andrei Vasiliu (whyte.vuhuni@gmail.com)
 *   Andrei Arusoaie (Andrei.Arusoaie@gmail.com)
 */

#include <stdio.h>
#include <stdarg.h>

#include "common.h"


MAP map =
{
   .map = 
     "##########################################"
     "#...........................#............#"
     "#..#..#..#..#..#..#..#..#..##....####....#"
     "#..........................#.....#..#....#"
     "#................................#..#....#"
     "#..........................#.....##.#....#"
     "#..#..#..#..#..#..#..#..#..##............#"
     "#...........................#............#"
     "############..#########.#####............#"
     "           #..#       #.#   #............#"
     "        ####..####    #.#   #....#.##....#"
     "        #........######.#####....#..#....#"
     "        #........................#..#....#"
     "        #........############....####....#"
     "        #........#          #............#"
     "        ##########          ##############",
     
   .height = 16,
   .width = 42,
};



void debugf( char *string, ... )
{
   char buf[2048];
   va_list args;
   
   va_start( args, string );
   vsnprintf( buf, 4096, string, args );
   va_end( args );
   
   printf( "%s\n", buf );
}


int init_world( )
{
   
   
   return 0;
}


int main( )
{
   int port = 1623;
   
   if ( init_world( ) )
     return 1;
   
   if ( main_loop( port ) )
     return 1;
   
   return 0;
}

