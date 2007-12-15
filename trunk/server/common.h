
/* Macros */

#define DATA "../data"


/* Data structures */

typedef struct player_data PLAYER;
typedef struct connection_data CONN;
typedef struct map_data MAP;

struct player_data
{
   char *name;
   CONN *connection;
   
   short pos_y, pos_x;
   
   PLAYER *next;
};

struct connection_data
{
   int sock;
   PLAYER *player;
   
   CONN *next;
};

struct map_data
{
   char *map;
   
   short height;
   short width;
};


/* Prototypes */

/* main.c */
extern MAP map;
void debugf( char *, ... );

/* comm.c */
extern int main_loop( int );
void destroy_connection( CONN * );
void send_to_client( CONN *, char *, int );

/* rwp_parse.c */
void parse_data( CONN *, char *, int );

/* rwp_send */
void kill_connection( CONN *, char * );
void send_map( CONN *c, MAP *map );
void send_userinfo( CONN *c, PLAYER *player );

/* players.c */
void pl_login( CONN *c, char *name );
void pl_disconnected( PLAYER *, int by_error );

