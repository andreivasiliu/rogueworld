/* RogueWorld Communication protocol. This will format data into pachets and
 * send it to the network. */


#include "common.h"



void kill_connection( CONN *c, char *msg )
{
   // send kill pachet and message
   
   destroy_connection( c );
}

