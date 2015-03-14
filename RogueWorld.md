# Introduction #

RogueWorld is a college project, with the goal to create a multiplayer Roguelike world.


# Details #

## Initial Phase (basic structure) ##
In the initial phase, the goal is to get a simple, working base.

The following aims exist:
  * A **persistent**, **real-time** world server
  * Basic support for rooms, players and objects
  * An UDP-safe communication [ProtocolSpecification](ProtocolSpecification.md) (currently just over TCP)

The intial phase is complete. [Revision 38](https://code.google.com/p/rogueworld/source/detail?r=38) marks its end, in the Subversion repository.

## Second Phase (network structure) ##
The second phase aims to finalize a good network base, as much of the later code will depend on how it is implemented.

The following aims exist:
  * Switch to full UDP support
  * Full support for resending/confirming of packets in the [ProtocolSpecification](ProtocolSpecification.md)
  * A better and more generalized implementation of the protocol