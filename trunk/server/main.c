/* RogueWorld server main file.
 * Developers:
 *   Andrei Vasiliu (whyte.vuhuni@gmail.com)
 *   Andrei Arusoaie (Andrei.Arusoaie@gmail.com)
 */

#include "common.h"



char *map[] = 
{
     "################",
     "#         #    #",
     "#              #",
     "#         #    #",
     "#         ### ##",
     "#         #    #",
     "#              #",
     "################",
};


int init_world( )
{
   
   
   return 0;
}


int tick( )
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

