#ifndef __PICO_CHESS__

namespace PicoChess {
 enum PIECE_TYPE { NONE=0, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN };

  enum COLOR { NOT_SET=0, WHITE, BLACK };

  enum INDICES { INVALID_INDEX=8 }; // board row/column indices range from 0..7
  
  enum OUTCOME { UNKNOWN=0, SIMPLE_MOVE, CAPTURE, PROMOTION, THREAT, CHECK,
		 CHECKMATE, DRAW, RESIGN, SQUARE_BLOCKED };

  class ChessEngine {
  public:
    ChessEngine() : color(BLACK), num_moves(0), num_turns(0), debug(false) {};
    ~ChessEngine() {};

    void UserMove(std::string &usermove_err_msg, std::string usermove);
    void NextMove(std::string &usermove_err_msg);
    void ShowBoard();
    void NewGame();
    void ChangeSides();
    void SetColor(std::string color);
    
    void SetDebug(bool _debug) { debug = _debug; };
    
    // load, save not implemented...
    void Load(std::string &load_file) {};
    void Save(std::string save_file) {};
    
  private:
    int color;
    int num_moves;
    int num_turns;
    bool debug;
  };

};

#endif
#define __PICO_CHESS__

