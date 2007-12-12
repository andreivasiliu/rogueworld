
/* Macros */

#define DATA "../data"


/* Data structures */

typedef struct player_data PLAYER;
typedef struct connection_data CONN;

struct player_data
{
   char *name;
   CONN *connection;
   
   PLAYER *next;
};

struct connection_data
{
   int sock;
   PLAYER *player;
   
   CONN *next;
};


/* Prototypes */

/* main.c */
void debugf( char *, ... );

/* comm.c */
extern int main_loop( int );
void kill_connection( CONN *, char * );
void destroy_connection( CONN * );

/* protocol.c */
void player_disconnected( PLAYER *, int by_error ); // not here
void parse_data( CONN *, char *, int );

/* players.c */
void pl_login( CONN *c, char *name );

