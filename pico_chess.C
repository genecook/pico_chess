#include <string>
#include <pico_chess.h>

namespace PicoChess {
  
void ChessEngine::UserMove(std::string &usermove_err_msg, std::string usermove) {
}

void ChessEngine::NextMove(std::string &usermove_err_msg) {
}

void ChessEngine::ShowBoard() {
}

void ChessEngine::NewGame() {
  color = BLACK;
  //game_board.Setup();
  num_moves = 0;
  num_turns = 0;
}

void ChessEngine::ChangeSides() {
  color = (color==WHITE) ? BLACK : WHITE;
}

void ChessEngine::SetColor(std::string color_str) {
  if (color_str == "WHITE")
    color = WHITE;
  else if (color_str == "BLACK")
    color = BLACK;
  else {
    // what the???
  }
}

}
