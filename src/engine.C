#include <chess.h>
#include <pico/time.h>

namespace PicoChess {

//******************************************************************************
// ChooseMove...
//******************************************************************************

std::string ChessEngine::ChooseMove(Board &game_board, Move *suggested_move) {
  absolute_time_t abs_start_time = get_absolute_time();
  
  MovesTree *moves_tree = new MovesTreeMinimax(Color(), Levels());

  Move next_move;
  int num_moves = moves_tree->ChooseMove(&next_move,game_board,suggested_move);

  absolute_time_t abs_end_time = get_absolute_time();

  int64_t elapsed_time_ms = absolute_time_diff_us(abs_start_time,abs_end_time) / 1000;
  float elapsed_time_s = ((float) elapsed_time_ms) / 1000.0;
  
  char tbuf[128];
  sprintf(tbuf,"# of levels: %d, number of moves evaluated: %d, elapsed time: %3.1f seconds\n",
	  moves_tree->MaxLevels(), moves_tree->MovesEvalCount(), elapsed_time_s);
  
  std::string move_str = std::string(tbuf) + NextMoveAsString(&next_move);

  delete moves_tree;
  
  return move_str;  
}

//******************************************************************************
// CrackMoveStr...
//******************************************************************************

void ChessEngine::CrackMoveStr(int &start_row,int &start_column,int &end_row,
			       int &end_column,std::string &move_str) {
  if (move_str.size() == 5) {
    if (move_str.substr(4,1) == "q") {
      // will ASSUME pawn promotion. 'q' indicates promotion to queen...
    } else {
#ifdef USE_EXCEPTIONS
      throw std::runtime_error("unsupported xboard move string???");
#else
      return;
#endif
    }
  } else if (move_str.size() != 4) {
#ifdef USE_EXCEPTIONS
    throw std::runtime_error("xboard move string is NOT four chars???");
 
#else
   return; 
#endif
  }
  
  std::string src_coordinate_str = move_str.substr(0,2);
  std::string dest_coordinate_str = move_str.substr(2,2);
  
  game_board.Index(start_row,start_column,src_coordinate_str);
  game_board.Index(end_row,end_column,dest_coordinate_str);
}

//******************************************************************************
// DebugEnable...
//******************************************************************************

void ChessEngine::DebugEnable(std::string move_str) {
  if (move_str == "!") {
    std::cout << "# debug enabled now..." << std::endl;
    debug = true;
    return;
  }
    
  unsigned int which_turn = 0;
  
  if ( (sscanf(move_str.c_str(),"%u",&which_turn) == 1) && (num_turns >= which_turn) ) {
    //std::cout << " debug enabled after " << which_turn << " moves..." << std::endl;
    debug = true;
    return;
  }
  
  if (move_str == debug_move_trigger) {
    //std::cout << "move " << move_str << " has caused debug to be enabled..." << std::endl;
    debug = true;
  }
}

//******************************************************************************
// UserMove...
//******************************************************************************

std::string ChessEngine::UserMove(std::string opponents_move_str) {
  int start_row = -1,start_column = -1,end_row = -1,end_column = -1; // move made prior to

  //ChooseOpening(opponents_move_str);

  //DebugEnable(opponents_move_str); // opponents move could enable debug
  
  // update board with opponents move, evaluate for possible checkmate...
  
  CrackMoveStr(start_row,start_column,end_row,end_column,opponents_move_str);

  Move omove(start_row,start_column,end_row,end_column,OpponentsColor());
  Board tmp_board = game_board; // in case of invalid move on users part

#ifdef USE_EXCEPTIONS
  try {
     tmp_board.MakeMove(start_row,start_column,end_row,end_column);
  } catch( std::logic_error reason) {
     //std::cout << "# Invalid move, reason: '" << reason.what() << ". move ignored." << std::endl;
     return "Illegal move: " + opponents_move_str;
  }
#else
  tmp_board.MakeMove(start_row,start_column,end_row,end_column);
#endif

  MovesTree moves_tree(Color(), Levels());

  if ( moves_tree.Check(tmp_board,OpponentsColor()) ) {
    //std::cout << "# Invalid move for " << ColorAsStr(OpponentsColor())
    //          << ". You are, or would be in check." << std::endl;
    return "Illegal move (in or moving into check): " + opponents_move_str;
  }

  std::vector<Move> all_possible_moves;
  
  moves_tree.GetMoves(&all_possible_moves,game_board,OpponentsColor());

  bool this_move_is_possible = false;
  
  for (auto pmi = all_possible_moves.begin(); pmi != all_possible_moves.end(); pmi++) {
    if (*pmi == omove) {
      this_move_is_possible = true;
      break;
    }
  }

  if (!this_move_is_possible) {
    //std::cout << "# Invalid move for " << ColorAsStr(OpponentsColor()) << std::endl;
    return "Illegal move: " + opponents_move_str;
  }
  
  game_board = tmp_board;
  
  return "";
}

//******************************************************************************
// CpuMove - internal method. Used to facilate game replay. Not for general use
//           otherwise!
//******************************************************************************
  
std::string ChessEngine::ReplayMove(std::string move_str, int color) {
  int start_row = -1,start_column = -1,end_row = -1,end_column = -1;

  CrackMoveStr(start_row,start_column,end_row,end_column,move_str);

  Move omove(start_row,start_column,end_row,end_column,color);
  
  game_board.MakeMove(start_row,start_column,end_row,end_column);

  return "";
}
  
//******************************************************************************
// quick check on proposed user move via game board...
//******************************************************************************
  
bool ChessEngine::PrecheckUserMove(std::string opponents_move_str) {
  int start_row = -1,start_column = -1,end_row = -1,end_column = -1;
  
  CrackMoveStr(start_row,start_column,end_row,end_column,opponents_move_str);

  Move omove(start_row,start_column,end_row,end_column,OpponentsColor());
  
  std::vector<Move> all_possible_moves;
  
  MovesTree moves_tree(Color(), Levels());

  moves_tree.GetMoves(&all_possible_moves,game_board,OpponentsColor());

  bool this_move_is_possible = false;
  
  for (auto pmi = all_possible_moves.begin(); pmi != all_possible_moves.end(); pmi++) {
    if (*pmi == omove) {
      this_move_is_possible = true;
      break;
    }
  }

  return this_move_is_possible;
}
  
//******************************************************************************
// NextMoveAsString...
//******************************************************************************

std::string ChessEngine::NextMoveAsString(Move *next_move) {
  std::string next_move_str;

  //std::cout << "#  Outcome: " << OutcomeAsStr(next_move->Outcome()) << std::endl;
 
  if (next_move->Outcome() == RESIGN) {
    //std::cout << "#  " << ColorAsStr(Color()) << " resigns!" << std::endl;
    next_move_str = "resign";
  } else if (next_move->Outcome() == DRAW) {
    //std::cout << "#  " << ColorAsStr(Color()) << " draw!" << std::endl;
    next_move_str = "1/2-1/2 {Draw by repetition}";
  } else if (next_move->Outcome() == CHECKMATE) {
    //std::cout << "#  " << ColorAsStr(OpponentsColor()) << " is checkmated!" << std::endl;
    //std::cout << "#  move that will cause mate: " + EncodeMove(game_board,next_move) << "\n" << std::endl;
    next_move_str = (OpponentsColor() == WHITE) ? "1-0 {Black mates}" : "0-1 {White mates}";
    next_move_str += "\nmove " + EncodeMove(game_board,next_move);
  } else {
    game_board.MakeMove(next_move->StartRow(),next_move->StartColumn(), // the root node 
  		        next_move->EndRow(),next_move->EndColumn());    //  contains the next move...
    next_move_str = "move " + EncodeMove(game_board,next_move);
    DebugEnable(next_move_str); // machine move could enable debug
  }

  return next_move_str;
}

//******************************************************************************
// NextMove...
//******************************************************************************

  std::string ChessEngine::NextMove() {
  //ChooseOpening("?"); // opening may have already been chosen, but if not...
  
  std::string opening_move_str; // = NextOpeningMove();

  Move opening_move;
  
  if (opening_move_str.size() > 0) {
    //std::cout << "#  next opening move: '" << opening_move_str << "'" << std::endl;
    int om_start_row,om_start_column,om_end_row,om_end_column;
    CrackMoveStr(om_start_row,om_start_column,om_end_row,om_end_column,opening_move_str);
    Move omove(om_start_row,om_start_column,om_end_row,om_end_column,Color());
    opening_move.Set(&omove);
    opening_move.SetOutcome(SIMPLE_MOVE);
  }

  num_turns++;

  return ChooseMove(game_board,&opening_move);
}

}
