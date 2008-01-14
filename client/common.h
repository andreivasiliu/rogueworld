
/* Macros */

/* Object Types */
#define OBJ_YOU		1
#define OBJ_PLAYER	2


/* Data structures */

typedef struct player_data PLAYER;
typedef struct map_data MAP;
typedef struct object_data OBJECT;

struct player_data
{
   char *name;
   
   OBJECT *body;
};

struct map_data
{
   char *map;
   
   int height, width;
};

struct object_data
{
   int type;
   int vnum;
   
   short pos_y, pos_x;
   
   OBJECT **prev, *next;
};


/* Prototypes */

/* main.c */
extern MAP *map;
extern PLAYER *player;
void die( char *reason );

/* comm.c */
int connect_to_server( char *hostname, int port, int type );
int receive_one_packet( );
void send_to_server( char *msg, int bytes );
int main_loop( );
void close_connection( );

/* rwp_parse.c */
void parse_data( char *data, int size );

/* rwp_send.c */
void send_login( char *name );
void send_enterworld( );
void send_setcursor( int y, int x );
void send_close( char *reason );

/* update.c */
void update_userinfo( char *name, int pos_y, int pos_x );
void update_map( int height, int width, char *map );

/* events.c */
extern int cursor_y;
extern int cursor_x;
void key_event( );
void tick_event( );
void draw_interface( );

/* objects.c */
extern OBJECT *objects;
void object_movement( int obj_nr, int obj_type, short pos_y, short pos_x );
void object_disappeared( int obj_nr );

