# Multithreading

There are 2 programs here, race.c and game.c.

Hardware: linux machine with C compiler with pthreads

To compile: gcc **file-name**.c -o **executable-name** -lpthread
  
To Run: ./**executable-name**

## race.c 
  Uses multithreading to race multiple horses. User inputs the race length and number of horses, both greater then 1. The program then creats a thread for each horse, each 'step' every thread will sleep for a random amount of time between 1 and 3 seconds. The final rankings are displayed when all threads have finished.

## game.c 
  Uses multithreading to have 2 'players' compete in a game of tictacktoe. 2 players are createed and given their own thread. They wait for the other player to take thier turn befor starting. During a turn the player will randomly place thier token in a open space between 1-9, if the space is taken they roll again. They take turns untill one wins with 3 tokens in a row, or tie if the game board is full.
  
There are also 2 text/script files provided that display a recorded session of the code running.
