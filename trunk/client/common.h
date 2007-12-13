

/* Prototypes */

/* main.c */
void die( char *reason );

/* comm_tcp.c */
int connect_to_server( char *hostname, int port );
int receive_one_packet( );
void send_to_server( char *msg, int bytes );

/* rwp_parse.c */
void parse_data( char *data, int size );

/* rwp_send.c */
void send_login( char *name );

