#include "HexBoard.h"


int main(){

   HexBoard board;
   do
   {
      board.printBoard();
      board.makeMove();
   } while (!board.gameOver());
}

