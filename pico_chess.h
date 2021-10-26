#ifndef __PICO_CHESS__

#include <string>

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

    // print color as string...
  
  static std::string ColorAsStr(int color) {
    if (color == WHITE) return "white";
    if (color == BLACK) return "black";
    return "not-set?";
  };

  // for character based board display, print
  // color as single char...
  
  static std::string ColorChar(int color) {
    return (color==WHITE) ? "W" : "B";
  };

  // for character based board display, print
  // piece type as single char 'icon'...
  
  static std::string PieceIcon(int type) {
    std::string icon = "?";
    switch(type) {
      case PAWN:   icon = "P"; break;
      case ROOK:   icon = "R"; break;
      case KNIGHT: icon = "N"; break;
      case BISHOP: icon = "B"; break;
      case KING:   icon = "K"; break;
      case QUEEN:  icon = "Q"; break;
      default: break;
    }
    return icon;
  };

  // print piece name given type...
  
  static std::string PieceName(int type) {
    std::string icon = "?";
    switch(type) {
      case PAWN:   icon = "Pawn"; break;
      case ROOK:   icon = "Rook"; break;
      case KNIGHT: icon = "Knight"; break;
      case BISHOP: icon = "Bishop"; break;
      case KING:   icon = "King"; break;
      case QUEEN:  icon = "Queen"; break;
      default: break;
    }
    return icon;
  };

  // print move outcome as string...
  
  static std::string OutcomeAsStr(int outcome) {
    std::string ts;
    switch(outcome) {
       case SIMPLE_MOVE:    ts = "simple-move"; break;
       case CAPTURE:        ts = "capture"; break;
       case PROMOTION:      ts = "promotion"; break;
       case THREAT:         ts = "threat"; break;
       case CHECK:          ts = "check"; break;
       case CHECKMATE:      ts = "checkmate"; break;
       case DRAW:           ts = "draw"; break;
       case SQUARE_BLOCKED: ts = "square-blocked"; break;
       case RESIGN:         ts = "resign"; break;
       case UNKNOWN:        ts = "unknown"; break;
       default: return "?"; break;
    }
    return ts;
  }

  static std::string CaptureTypeAsStr(int capture_type) {
    std::string ts;
    switch(capture_type) {
       case QUEEN:  ts = "queen"; break;
       case BISHOP: ts = "bishop"; break;
       case KNIGHT: ts = "knight"; break;
       case ROOK:   ts = "rook"; break;
       case PAWN:   ts = "pawn"; break;
       default: ts = "???"; break;
    }
    return ts;
  }
  
  // print move outcome as single char...
  
  static std::string OutcomeAsChar(int outcome) {
    std::string ts;
    switch(outcome) {
       case SIMPLE_MOVE:    ts = ""; break;
       case CAPTURE:        ts = "C"; break;
       case PROMOTION:      ts = "P"; break;
       case THREAT:         ts = "T"; break;
       case CHECK:          ts = "C!"; break;
       case CHECKMATE:      ts = "M!"; break;
       case DRAW:           ts = "D!"; break;
       case SQUARE_BLOCKED: ts = "B"; break;
       case RESIGN:         ts = "R!"; break;
       case UNKNOWN:        ts = "?"; break;
       default: return "?"; break;
    }
    return ts;
  };

};

#endif
#define __PICO_CHESS__
