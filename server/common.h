
/* Macros */

/* Directories */
#define DATA "../data"

/* Object Types */
#define OBJ_YOU		1
#define OBJ_PLAYER	2


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
   
   short cursor_y, cursor_x;
   short body_pos_y, body_pos_x;
   
   PLAYER *next;
};

struct connection_data
{
   int sock;
   PLAYER *player;
   struct packet_queue_data *pqueue;
   
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
   
   OBJECT *next;
};
   

/* Prototypes */

/* main.c */
extern MAP map;
void debugf( char *, ... );

/* comm.c */
extern int main_loop( int );
void destroy_connection( CONN * );
void send_to_connection( CONN *, char *, int );

/* rwp_parse.c */
void parse_data( CONN *, char *, int );

/* rwp_send.c */
void kill_connection( CONN *, char * );
void send_map( CONN *c, MAP *map );
void send_userinfo( CONN *c, PLAYER *player );
void send_movement( PLAYER *pl, OBJECT *obj, short pos_y, short pos_x );
void send_disappear( PLAYER *pl, OBJECT *obj );

/* rwp_common.c */
void create_packet_queue( CONN * );
void destroy_packet_queue( CONN * );

/* players.c */
extern PLAYER *players;
void pl_login( CONN *c, char *name );
void pl_disconnected( PLAYER *, int by_error );
void pl_enterworld( PLAYER * );
void destroy_player( PLAYER * );

/* events.c */
void tick_event( );

/* objects.c */
OBJECT *create_object( int type );
void destroy_object( OBJECT *obj );
void move_object( OBJECT *obj, short y, short x );
void place_object( OBJECT *obj, short pos_y, short pos_x );
void displace_object( OBJECT *obj );

