# Searching

This project implements a game where a hidden number x is chosen in the range 1 ≤ x ≤ n.  
We can ask questions of the form "Is x < y?". At most k answers may be lies.  
The game is repeated g times.

## Interface

- void dajParametry(int &n, int &k, int &g)  
  Provides parameters n (range size), k (max lies per game), g (number of games).

- bool mniejszaNiz(int y)  
  Returns whether x < y in the current game. Answer may be false due to lies.

- void odpowiedz(int x)  
  Submits the final guess. Automatically starts the next game.

## Goal

The program must always guess x correctly, while using the minimal number of questions possible.