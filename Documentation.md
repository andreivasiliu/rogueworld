# Introducere #

Rogueworld  este rezultatul unei încercări de a rescrie o nouă variantă a unui "engine" pentru un joc din categoria Roguelike, un RPG  text în stilul lui NetHack. Privit într-o manieră superficială, un  „roguelike” este de fapt o expunere  bidimensională a unei lumi virtuale cu diferite caracteristici ce conferă posibilitatea explorării ei.


# Specificaţii #

Proiectul de faţă înglobează partea poate cea mai preteţioasă a unui astfel de "engine" şi anume codul specific comunicării pe reţea, împărţirea şi distribuirea pachetelor de informaţii;

Serverul poate deservi mai mulţi clienţi în acelaşi timp indiferent de opţiunea aleasă de client; acesta are posibilitatea să aleagă o conexiune TCP sau una UDP în funcţie de preferinţe, cunoscând însă ce implică alegerea fiecăreia dintre ele.

# Detalii (partea de reţea) #

În această secţiune vom detalia anumite aspecte din implementare.

## Comunicarea la nivelul serverului. ##

  * `main_loop()`
    * `init_udp_server()`
      * această funcţie creează un nou socket şi ascultă pe el eventualii clienţi UDP cu scopul de a adăuga noi clienţi în lista de conexiuni sau de a primi pachete pentru clienţii deja conectaţi;
    * `init_tcp_server()`
      * această funcţie creează un nou socket şi ascultă pe el eventualii clienţi TCP cu acelaţi scop ca mai sus;
    * `while( 1 )`
      * creează o nouă mulţime de sockets pentru verificare;
      * se apelează `select()` pe mulţimea construită;
      * verifică:
        * dacă există un nou client UDP apelează: `new_udp_connection()` - această funcţie adaugă noua conexiune UDP în lista existentă şi îi asociază un port remote;
        * dacă există un nou client TCP apelează: `new_tcp_connection()` - această funcţie adaugă noua conexiune TCP în lista existentă şi îi asociază un port remote;
        * dacă a sosit un nou pachet apelează `read_data()` - această funcţie citeste                        orice fel de pachet;



## Structura pachetelor. ##

```
struct packet_header
{
   int length;
   int msg_id;
   int msg_nr;
   int obj_id;
   int obj_nr;
};
```

```
struct packet_queue_data
{
   int packet_count;
   
   CONN *connection;
   PACKET *packets;
};
```

Detaliile complete sa afla in pagina ProtocolSpecification.

## Operaţii asupra pachetelor ##

Funcţii folosite:
  * `int rwp_parse( char *packet, char *types, ... )`
    * această funcţie asigură integritatea şi despachetarea pachetelor; ea trebuie să primească un string de forma "4411z", apoi un numar variabil de parametri, de tipul int (4), char (1), sau string (z);
  * `void rwp_send_packet( PACKET_QUEUE *pqueue,int type, int objnr, char *types, ... )`
    * această funcţie asigură construcţia şi trimiterea pachetelor; funcţionarea ei este asemeni funcţiei de mai sus;


## Comunicarea la nivelul clientului ##

  * `int main()`
    * validează logare;
    * `int connect_user( char *name, char *hostname, int port, int protocol_type )`
      * această funcţie conectează jucătorul name, la host-ul hostname, portul port utilizând protocolul protocol\_type;
    * `draw_interface( ) /* desenează interfaţa */`
    * `main_loop()`
      * `while( 1 )`
        * dacă s-a primit un pachet apelează `receive_one_packet( )`
          * citeşte un pachet şi-l despachetează apelând `parse_data( )`
        * altfel apelează `key_event( )`
          * actualizează harta în urma apăsării unei taste