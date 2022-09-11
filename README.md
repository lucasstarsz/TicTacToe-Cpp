# TicTacToe

This is a very slightly overengineered version of Tic-Tac-Toe, for Windows. It does not use any dependencies outside of C++20.

## How to Play

The project is available as an executable here: https://github.com/lucasstarsz/Cpp-TicTacToe/releases

Alternaitvely, you can build the project in a version of Visual Studio which includes support for C++20 (and C17).

Instructions for how to play the game are included within the game itself. You simply need to specify both players' names, and then run `help`.

## Insight

As my first notable C++ project, it aims to demonstrate what I've learned in C++ over ~2 days of learning.

It does not claim to be a strong source of learning material, let alone reputable source material. And of course, it does not claim to be a great Tic-Tac-Toe game.

## Version Log

### v1.1
- added indicator of X vs O to players
- players can now rename the boards and themselves
- players can quietly exit the game with `exit q` or `exit quiet`
- added ability to play rematches ingame

### v1.0
- initial game 'release'