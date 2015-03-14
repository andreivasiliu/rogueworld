# Design Goals #

Even though initially tested over TCP, the intent is to provide a fast, safe protocol for use with UDP. Packets must be small and error-safe, and subsequent information about an object or aspect of the game should make all such previous packets obsolete. The goal is to support a packet loss rate of at least 5-10%. The project also aims towards high portability, so big-endian ordering is used everywhere in communication.

**Status**: _Partially Planned and Partially Implemented_.

# High-Level Specification #

In case of any error (e.g. invalid user/password), the communication will end with a Close packet.

Login Protocol:
  * Client:
    * send 'login user, password'
  * Server:
    * send 'userinfo real\_name, position'
    * send 'map'
  * Client:
    * send 'enterworld'

Movement Protocol:
  * Client:
    * send 'setcursor x, y'
  * Server:
    * send 'movement x, y'
    * send 'object nr, type, whathappened'

# Low-Level Specification #

Packet Data Types:
  * **int8**: 1 byte
  * **int32**: 4 bytes in network-order (big-endian).
  * **zstring**: null-terminated string.

Global Packet Header:
```
 <int32><int32><int32><int32><int32>
  ^      ^      ^      ^      ^-- object number
  |      |      |      \-- object ID
  |      |      \-- message number
  |      \-- message ID (mid)
  \-- message length
```

Login Packet (_client_) (mid = MSG\_LOGIN):
```
 <zstring>
  ^-- name
```

Close Packet (_both_) (mid = MSG\_CLOSE):
```
 <zstring>
  ^-- reason
```

User Info Packet (_server_) (mid = MSG\_USERINFO):
```
 <zstring><int8><int8>
  ^        ^     ^-- position x on map
  |        \-- position y on map
  \-- properly capitalized player-name
```

Map Packet (_server_) (mid = MSG\_MAP):
```
 <int8><int8><zstring>
  ^     ^     ^-- RLE-encoded map representation
  |     \-- map width
  \-- map height
```

Enter World Packet (_client_) (mid = MSG\_ENTERWORLD)
```
 <empty>
```

Set Cursor Packet (_client_) (mid = MSG\_SETCURSOR)
```
 <int8><int8>
  ^     ^-- x position on map
  \-- y position on map
```

Movement Packet (_server_) (mid = MSG\_MOVEMENT)
```
 <int32><int32><int8><int8>
  ^      |      ^     ^-- x position on map
  |      |      \-- y position on map
  |      \-- object type
  \-- object vnum
```

Disappear Packet (_server_) (mid = MSG\_DISAPPEAR)
```
 <int32>
  ^-- object vnum
```