# Sokoban
My implementation of [Sokoban] game with command-line interface.
Program first reads description of the board, writes it to the console and then accepts commands. 
The board is displayed after executing each command.
There are two types of commands: pushing specified box in given direction and reverting last executed push.
Player does not need to give move commands in order to approach box. The program finds shortest
path, consisting only of blank squares, to the position where push in given direction of specified box is possible. 

#### **Board**
At the beginning, board description should be given followed by a blank line.

`-` empty square

`+` storage location

`#` wall

`@` empty square with player

`*` storage location with player

`[a .. z]` (lowercase English alphabet letter) square with box with given name

`[A .. Z]` (uppercase English alphabet letter) storage location with box with given name

There is only one player in correct description of the board. Each box can occur only once.

#### **Commands**
Program accepts following commands, each in a new line:

`[a .. z][2 | 4 | 6 | 8]` (lowercase English alphabet letter followed by one of the digits: 2, 4, 6, 8)
pushing box with given name in specified direction, where `2` denotes down, `8` up, `4` left, `6` right;
if push is not possible, because there is no way to approach specified box or square where box would be
pushed onto is not empty, nothing happens

`0` reverting last push

`.` quit game

#### **Playing**
In order to play the game, execute following commands:

`git clone https://github.com/Czyzmanski/Sokoban Sokoban`

`cd Sokoban`

`mkdir release`

`cd release`

`cmake ..`

`make`

`./sokoban`

For examples, see `examples` directory.

[Sokoban]: https://en.wikipedia.org/wiki/Sokoban