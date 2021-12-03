#include <iostream>
#include <string>
//#include <queue>

#include "pico/stdlib.h"
//#include "pico/multicore.h"

#include <chess.h>

namespace PicoStreamPlayer {
  
#ifdef REDIRECT_XBOARD_CALLS
void get_next_token(std::string &next_token);
void to_xboard(std::string tbuf);
#else
// retreive the next xboard token...
  
void get_next_token(std::string &next_token) {    
  printf(">>>");
  std::cin >> next_token;
  std::cout << "# " << next_token << std::endl;
}

// xboard is connected via bi-directional pipe...

void to_xboard(std::string tbuf) {
  std::cout << tbuf << std::endl;
}
#endif

  
//*************************************************************************
// stream player entry point...
//*************************************************************************

int Play(PicoChess::ChessEngine *my_little_engine) {
  int rcode = 0;
  
  // we 'parse' just enough of xboard commands, responses, to drive our engine...
  
  enum { ACCEPT_STATE = 1, MOVE_STATE = 2, SAVE_STATE = 3, LOAD_STATE = 4, DEBUG_STATE = 5,
         CHECK_MOVE_STATE = 6 };
  
  int input_state = 0;            // parsing state

  bool game_on = true;            // the game is afoot...

  bool xboard_connected = false;  // we assume xboard is NOT connected
  bool force_mode = false;        // force mode is off. engine will make 'next'
                                  // move when a 'usermove' is received
  
  while(game_on) {
    std::string tbuf;
    get_next_token(tbuf);

    if (input_state == ACCEPT_STATE) {
      input_state = 0;
      continue;
    }
      
    if (input_state == SAVE_STATE) {
      // save board state to file...
      std::string save_file = tbuf;
      to_xboard("# BBB save to file not supported" + save_file);
      //my_little_engine->Save(save_file);
      input_state = 0;
      continue;
    }
      
    if (input_state == LOAD_STATE) {
      // load board state from file...
      std::string load_file = tbuf;
      to_xboard("# BBB load from file not supported" + load_file);
      //my_little_engine->Load(load_file);
      input_state = 0;
      continue;
    }
      
    if (input_state == DEBUG_STATE) {
      // debug on or off...
      bool debug_state = (tbuf == "on");
      my_little_engine->SetDebug(debug_state);
      to_xboard( (debug_state ? "# BBB debug ON" : "# BBB debug OFF") );
      input_state = 0;      
      continue;
    }
     
    if (input_state == CHECK_MOVE_STATE) {
      // sanity check on proposed move...
      std::string usermove = tbuf;

      if (my_little_engine->PrecheckUserMove(usermove))
        to_xboard("# BBB OK move");
      else
	to_xboard("# BBB BAD move");
      input_state = 0;
    }
    
    if (input_state == MOVE_STATE) {
      // process 'user' move...
      std::string usermove = tbuf;
      input_state = 0;

      to_xboard("# BBB usermove " + usermove);
      
      std::string usermove_err_msg = my_little_engine->UserMove(usermove);
     
      if (usermove_err_msg == "") {
        // usermove accepted by engine...
      } else {
	// oops! problem with usermove; response from engine indicates error...
        to_xboard(usermove_err_msg);
	continue;
      }
      
      if (force_mode) {
        // engine is idle...
      } else {
	// engine makes a move and responds with same...
	to_xboard("# BBB engine to make move...");
	std::string engine_move = my_little_engine->NextMove();
        to_xboard("# BBB engine move made: " + engine_move);
      }
      
      if (!xboard_connected) {
        // xboard not connected, so show board state
	std::string board_as_str = my_little_engine->BoardAsString();
	to_xboard(board_as_str); // use to_xboard even when NOT connected
      }
      continue;
    }
      
    if (tbuf == "showboard") {
      std::string board_as_str = my_little_engine->BoardAsString();
      to_xboard(board_as_str); 
      continue;
    }
      
    if (tbuf == "xboard") {
      // communicating with xboard or the equivalent...
      // set 'usermove' feature just to make it easier to pick
      // off moves from xboard...
      xboard_connected = true;
      to_xboard("# BBB xboard");
      to_xboard("feature usermove=1 debug=1 sigint=0 sigterm=0 done=1");
      continue;
    }
      
    if (tbuf == "new") {
      // new game. leave force mode. opponent is white, machine is black...
      my_little_engine->NewGame();
      force_mode = false;
      to_xboard("# BBB new");
      continue;	
    }
      
    if (tbuf == "quit") {
      // game is over...
      game_on = false; 
      to_xboard("# BBB quit");
      continue;
    }
      
    if (tbuf == "force") {
      // pause engine...
      force_mode = true;
      to_xboard("# BBB force");
      continue;
    }
      
    if (tbuf == "go") {
      // 'go' instructs engine to leave force mode, then make the next move...
      force_mode = false;
      to_xboard("# BBB go");
      std::string engine_move = my_little_engine->NextMove();
      to_xboard(engine_move);
      to_xboard("# BBB " + engine_move);
      continue;
    }
      
    if (tbuf == "playother") {
      // leave force mode. engine changes sides...
      force_mode = false;
      my_little_engine->ChangeSides();
      to_xboard("# BBB playother");
      continue;
    }
      
    if (tbuf == "changesizes") {
      // engine changes sides. force mode may or may be in effect...
      my_little_engine->ChangeSides();
      to_xboard("# BBB changesizes");
      continue;
    }
      
    if (tbuf == "white") {
      // set white on move. set the engine to play black...
      my_little_engine->SetColor("white");
      to_xboard("# BBB white - engine plays white");	
      continue;
    }
      
    if (tbuf == "black") {
      // set black on move. set the engine to play white...
      my_little_engine->SetColor("black");
      to_xboard("# BBB black - engine plays black");	
      continue;
    }
      
    if (tbuf == "accepted") {
      // xboard has accepted some feature; next token is that feature...
      input_state = ACCEPT_STATE;
      continue;
    }

    if (tbuf == "usermove") {
      // next token is move from xboard...
      input_state = MOVE_STATE;
      continue;
    }
    
    if (tbuf == "checkmove") {
      // next token is move to check from xboard...
      input_state = CHECK_MOVE_STATE;
      continue;
    }
	
    if (tbuf == "?") {
      to_xboard("# BBB ?");	
      // move now, if the engine is enabled, else ignore...
      if (force_mode) {
	// engine is paused...
      } else {
	std::string engine_move = my_little_engine->NextMove();
        to_xboard( "move " + engine_move);
	to_xboard("# BBB " + engine_move);
      }
      continue;
    }
      
    if (tbuf == "save") {
      // next token is filename...
      input_state = SAVE_STATE;
      continue;
    }
      
    if (tbuf == "load") {
      // next token is filename...
      input_state = LOAD_STATE;
      continue;
    }
      
    if (tbuf == "debug") {
      // next token is debug state...
      input_state = DEBUG_STATE;
      continue;
    }
      
    std::cout << "# '" << tbuf << "' ignored or not yet implemented" << std::endl;
    continue;
  }
  
  return rcode;
}

}
