#ifndef __CHESS_ENGINE__

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
    
  private:
    int color;
    int num_moves;
    int num_turns;
    bool debug;
};

#endif
#define __CHESS_ENGINE__
