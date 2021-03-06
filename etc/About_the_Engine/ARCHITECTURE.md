PROJECT ARCHITECTURE
====================

##GENERAL PROJECT INFORMATION:

**Authors**: Platon-Nikolaos Kiorpelidis
**Language**: C11 until commit \#200
          C++11 after commit \#201
**Date**: Started at 28-1-2016

###CLASSES:

###*class Board*:
####GENERAL INFO:
* Keeps track of board size, board configuration, game history and the number
  of walls of either color.

* Is responsible for functions like setting up the board size, clearing the
  board, updating the number of the available walls.

* Is responsible for core play functions like play a move, play a wall,
  generating a AI move or undoing a move. It can also check if there is a
  winner or it can show the current board configuration.

####PRIVATE CLASSES:
* class Board uses two instances of private class, player.
  The class player is responsible for playing a move(?) and a wall(?) and for
  keeping track of number of available walls and the player position.

####PUBLIC MEMBERS:
* function showboard
* function boardsize
* function clear_board
* function walls(?)
* function playmove(?)
* function playwall(?)
* function undo
* function genmove
* function winner
* more(?)

####PRIVATE MEMBERS:
* class Player
* stack with board history
* current board configuration
* more(?)

###*class Player*:
####GENERAL INFO:
* Keeps track of the player position and number of available walls.
* Is responsible for playing a move(?) and a wall(?).

####PRIVATE CLASSES:
* does not have.

####PUBLIC MEMBERS:
* function walls(?)
* function playmove(?)
* function playwall(?)

####PRIVATE MEMBERS:
* location of the player
* amount of available walls

###NAMESPACES:

###*administrative commands*:

####MEMBERS:
* funtion name
* function known_command
* function list_commands
* function quit
