#ifndef __MOVETREE__

//******************************************************************************
// MovesTree
//******************************************************************************

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include <pico_chess.h>
#include <board.h>
#include <pieces.h>
#include <move.h>

//#define GRAPH_SUPPORT 1

namespace PicoChess {

#ifdef GRAPH_SUPPORT
extern int master_move_id;
#endif
  
//******************************************************************************
// moves tree node...
//******************************************************************************

class MovesTreeNode : public Move {
public:
  MovesTreeNode() : possible_moves(NULL) {
    InitMove();
#ifdef GRAPH_SUPPORT
    move_id = master_move_id++;
#endif
  };
  
  MovesTreeNode(int start_row, int start_column, int end_row, int end_column, int color,
		int outcome = INVALID_INDEX, int capture_type = INVALID_INDEX)
              : possible_moves(NULL) {
    InitMove(start_row, start_column, end_row, end_column, color, outcome, capture_type);
#ifdef GRAPH_SUPPORT
    move_id = master_move_id++;
#endif
  };

  MovesTreeNode(Move move) : possible_moves(NULL) {
    InitMove(move.StartRow(), move.StartColumn(), move.EndRow(), move.EndColumn(),
	     move.Color(), move.Outcome(), move.Check(), move.CaptureType());
#ifdef GRAPH_SUPPORT
    move_id = master_move_id++;
#endif
  };
  
  ~MovesTreeNode() { Flush(); };

  MovesTreeNode *AddMove(Move new_move) {
    assert ( (pm_count + 1) < INT_LEAST8_MAX );
    
    MovesTreeNode *new_node = (MovesTreeNode *) malloc( sizeof(MovesTreeNode) );
    
    new_node->Set(&new_move);
#ifdef GRAPH_SUPPORT
    new_node->move_id = master_move_id++;
#endif
    new_node->pm_count = 0;
    new_node->possible_moves = NULL;
    
    possible_moves = (MovesTreeNode **) realloc(possible_moves, sizeof(MovesTreeNode *) * (pm_count + 1) );
    possible_moves[pm_count] = new_node;
    pm_count++;
    
    return new_node;
  };
  
  int PossibleMovesCount() { return pm_count; };
  
  MovesTreeNode *PossibleMove(int index) { return possible_moves[index]; };

  void Flush() {
    if (pm_count > 0) {
      assert(possible_moves != NULL);
      for (int i = 0; i < pm_count; i++) {
         possible_moves[i]->Flush();
         free(possible_moves[i]);
      }
      free(possible_moves);
      pm_count = 0;
      possible_moves = NULL;
    } else {
      assert( (pm_count == 0) && (possible_moves == NULL) );
    }
  };

  friend std::ostream& operator<< (std::ostream &os, PicoChess::MovesTreeNode &fld);
  
  void Sort( bool (*sortfunction)(MovesTreeNode *m1, MovesTreeNode *m2) ) {
    std::sort( possible_moves, possible_moves + pm_count, sortfunction );
  };

  void Randomize() {
    std::random_shuffle( possible_moves, possible_moves + pm_count );
  };

#ifdef GRAPH_SUPPORT
  int ID() { return move_id; };
  int move_id;
#endif
  
private:
  MovesTreeNode **possible_moves;
};


struct piece_counts {
    piece_counts() : kings(0), queens(0), bishops(0),knights(0),rooks(0),pawns(0) {};

    void dump(const std::string &prefix) {
      std::cout << prefix << "# kings/queens/bishops/knights/rooks/pawns: " << kings
      << "/" << queens << "/" << bishops << "/" << knights << "/" << rooks
      << "/" << pawns << std::endl;
    };

    int kings;
    int queens;
    int bishops;
    int knights;
    int rooks;
    int pawns;
};

//******************************************************************************
// base moves tree class...
//******************************************************************************

class MovesTree {
 public:
  MovesTree(int _color, int _max_levels) : color(_color), max_levels(_max_levels) {
    root_node = new MovesTreeNode;
  };
  
  ~MovesTree() { root_node->Flush(); };

  virtual int ChooseMove(Move *next_move, Board &game_board, Move *suggested_move = NULL) { return 0; };
  bool GetMoves(std::vector<Move> *possible_moves, Board &game_board, int color,bool avoid_check = true);
  bool Check(Board &board,int color);
  
  static Board MakeMove(Board &board, MovesTreeNode *pv);

 protected:
  void EvalBoard(MovesTreeNode *move, Board &current_board, int forced_score=UNKNOWN);
  int MaterialScore(Board &current_board);
  
  bool GetMoves(MovesTreeNode *current_node, Board &current_board, int current_color,
		bool avoid_check = true, bool sort_moves = false);

  void PickBestMove(MovesTreeNode *root_node, Board &game_board, Move *suggested_move);
  bool BestScore(MovesTreeNode *this_move, MovesTreeNode *previous_move);

  void CountPieces(struct piece_counts &counts, Board &game_board,int color);
  int GetPieceCount(Move *node,Board &game_board,int color);

  int Color() { return color; };
  int OtherColor(int current_color) { return (current_color == WHITE) ? BLACK : WHITE; };
  int NextColor(int current_color) { return OtherColor(current_color);  };
  
  int MaxLevels() { return max_levels; };

  void GraphMovesToFile(const std::string &outfile, MovesTreeNode *node);
  void GraphMoves(std::ofstream &grfile, MovesTreeNode *node, int level);

  MovesTreeNode *root_node;

  int color;
  int max_levels;

  int eval_count;

  int kings_row;
  int kings_column;

  Pieces pieces; // used to generate moves for each chess piece type
};

//******************************************************************************
// minimax moves (sub)tree class...
//******************************************************************************

class MovesTreeMinimax : public MovesTree {
 public:
  MovesTreeMinimax(int _color, int _max_levels) : MovesTree(_color,_max_levels) {};

  int ChooseMove(Move *next_move, Board &game_board, Move *suggested_move = NULL);

 private:

  void ChooseMoveInner(MovesTreeNode *current_node, Board &current_board, int current_color,
		       int current_level, int alpha, int beta);
};

};

#endif
#define __MOVETREE__
