
/* Number representation of MIDs. */

#define MSG_LOGIN	1
#define MSG_ENTERWORLD	2
#define MSG_USERINFO	3
#define MSG_MAP		4
#define MSG_SETCURSOR	5
#define MSG_MOVEMENT	6
#define MSG_DISAPPEAR	7
#define MSG_CLOSE	8



/* Structures. */
typedef struct packet_data PACKET;
typedef struct packet_queue_data PACKET_QUEUE;

struct packet_data
{
   int length;
   int msg_id;
   int msg_nr;
   int obj_id;
   int obj_nr;
   
   char *raw;
   
   PACKET *next;
};

struct packet_queue_data
{
   int packet_count;
   
   CONN *connection;
   PACKET *packets;
};


struct packet_header
{
   int length;
   int msg_id;
   int msg_nr;
   int obj_id;
   int obj_nr;
};



/* Some more defines. */
#define PACKET_HEADER_SIZE      sizeof(struct packet_header)
#define MAX_PACKET_SIZE         4096



/* Interface to RWP. */
void rwp_send_packet( PACKET_QUEUE *pqueue,
		      int type, int objnr, char *types, ... );
int get_packet_length( char *data );
void rwp_parse_header( char *packet, struct packet_header *dest );
int rwp_parse( char *packet, char *types, ... );
