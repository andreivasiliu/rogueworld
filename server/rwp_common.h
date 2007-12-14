
/* Number representation of MIDs. */

#define MSG_LOGIN	1
#define MSG_ENTERWORLD	2


/* Packet-building functions. */
char *write_zstring( char *p, char *str );
char *write_int32( char *p, int n );
char *new_packet( int message_id, int packet_size );
char *skip_header( char *packet );
void send_packet( CONN *c, char *packet );

/* Packet-parsing functions. */
char *read_zstring( char *p, char *dest, int max );
char *read_int32( char *p, int *dest );

