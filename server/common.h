
/* Macros */

/* Directories */
#define DATA "../data"

/* Object Types */
#define OBJ_PLAYER	1


/* Data structures */

typedef struct player_data PLAYER;
typedef struct connection_data CONN;
typedef struct map_data MAP;
typedef struct object_data OBJECT;

struct player_data
{
   char *name;
   CONN *connection;
   OBJECT *persona;
   
   short cursor_y, cursor_y;
   
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

struct object_data
{
   int vnum;
   int type;
   
   PLAYER *player;
   
   short pos_y, pos_x;
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
void send_movement( PLAYER *pl );

/* players.c */
extern PLAYER *players;
void pl_login( CONN *c, char *name );
void pl_disconnected( PLAYER *, int by_error );
void pl_move( PLAYER *, int y, int x );
void pl_enterworld( PLAYER * );

/* events.c */
void tick_event( );

