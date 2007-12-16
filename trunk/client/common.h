

/* Data structures */

typedef struct player_data PLAYER;
typedef struct map_data MAP;

struct player_data
{
   char *name;
   
   int pos_y, pos_x;
};

struct map_data
{
   char *map;
   
   int height, width;
};


/* Prototypes */

/* main.c */
extern MAP *map;
extern PLAYER *player;
void die( char *reason );

/* comm_tcp.c */
int connect_to_server( char *hostname, int port );
int receive_one_packet( );
void send_to_server( char *msg, int bytes );
int main_loop( );

/* rwp_parse.c */
void parse_data( char *data, int size );

/* rwp_send.c */
void send_login( char *name );
void send_enterworld( );
void send_setcursor( int y, int x );

/* update.c */
void update_userinfo( char *name, int pos_y, int pos_x );
void update_map( int height, int width, char *map );

/* events.c */
extern int cursor_y;
extern int cursor_x;
void key_event( );
void tick_event( );
void draw_interface( );


