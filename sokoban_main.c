#include <stdio.h>

#include "move_stack.h"
#include "board.h"
#include "command.h"
#include "game.h"

#define END_OF_DATA '.'

void readAndExecuteCommands(Game *game) {
    MoveStack stack;
    initMoveStack(&stack);

    int c = getchar();
    while (c != END_OF_DATA) {
        if (c == UNDO_COMMAND) {
            if (!isMoveStackEmpty(&stack)) {
                executeUndoCommand(game, &stack);
            }
        }
        else {
            PushCommand pushComm;
            pushComm.chestNum = getChestNum(c);
            pushComm.direction = getchar();
            if (isPushCommandPossible(game, &pushComm)) {
                executePushCommand(game, &pushComm, &stack);
            }
        }
        printBoard(game->board);
        getchar();
        c = getchar();
    }

    clearMoveStack(&stack);
}

int main() {
    Board board;
    initBoard(&board);
    readInitialBoardState(&board);

    printBoard(&board);

    Game game;
    game.board = &board;

    initChestsPositions(game.chestsPos);
    findChestsPositions(&game);

    Position playerPos;
    findPlayerPosition(&game, &playerPos);
    game.playerPos = &playerPos;

    readAndExecuteCommands(&game);

    disposeGame(&game);

    return 0;
}
