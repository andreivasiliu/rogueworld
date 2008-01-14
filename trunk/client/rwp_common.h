
/* Number representation of MIDs. */

#define MSG_LOGIN	1
#define MSG_ENTERWORLD	2
#define MSG_USERINFO	3
#define MSG_MAP		4
#define MSG_SETCURSOR	5
#define MSG_MOVEMENT	6
#define MSG_DISAPPEAR	7
#define MSG_CLOSE	8


/* Packet-building functions. */
char *write_zstring( char *p, char *str );
char *write_int8( char *p, char a );
char *write_int32( char *p, int n );
char *new_packet( int message_id, int packet_size );
char *skip_header( char *packet );

/* Packet-parsing functions. */
char *read_zstring( char *p, char *dest, int max );
char *read_int8( char *p, char *a );
char *read_int32( char *p, int *dest );

