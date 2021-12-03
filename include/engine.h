#ifndef __CHESS_ENGINE__

namespace PicoChess {

class ChessEngine {
  public:
    ChessEngine() : color(BLACK), num_of_levels(NUMBER_OF_LEVELS),
		    num_moves(0), num_turns(0), debug(false) {
       Init();		    
    };
    ~ChessEngine() {};

  void Init(int _color = BLACK, int _num_of_levels = NUMBER_OF_LEVELS,
	    bool _debug = false) {
         color = _color;
         num_of_levels = _num_of_levels;
         debug = _debug;

	 NewGame();
       };

    std::string ChooseMove(Board &game_board, Move *suggested_move = NULL);

    void CrackMoveStr(int &start_row,int &start_column,int &end_row,int &end_column,
		      std::string &move_str);

    void DebugEnable(std::string move_str);

    std::string NextMoveAsString(Move *next_move);

    std::string UserMove(std::string opponents_move_str);
    std::string NextMove();
  
    bool PrecheckUserMove(std::string opponents_move_str);
  
    void NewGame() {
           color = BLACK;
           game_board.Setup();
           num_moves = 0;
           num_turns = 0;
    };
  
    void ChangeSides() { color = (color==WHITE) ? BLACK : WHITE; };
  
    void SetColor(std::string color_str) {
            if (color_str == "WHITE")
              color = WHITE;
            else if (color_str == "BLACK")
              color = BLACK;
            else {
              // what the???
            }
    };
    
    void SetDebug(bool _debug) { debug = _debug; };

    std::string BoardAsString() {
      return game_board.AsString();
    };

    int Color() { return color; };
    int OpponentsColor() { return (Color() == WHITE) ? BLACK : WHITE; };

    int Levels() { return num_of_levels; };

 // encode move in algebraic notation...

  static std::string EncodeMove(Board &game_board,Move &src) {
    return game_board.Coordinates(src.StartRow(),src.StartColumn())
      + game_board.Coordinates(src.EndRow(),src.EndColumn());
  };
  static std::string EncodeMove(Board &game_board,Move *src) {
    return game_board.Coordinates(src->StartRow(),src->StartColumn())
      + game_board.Coordinates(src->EndRow(),src->EndColumn());
  };

  private:
    int color;
    Board game_board;
    int num_of_levels;
    int num_moves;
    int num_turns;
    bool debug;
  
    std::string debug_move_trigger;
};

};

#endif
#define __CHESS_ENGINE__
