#include <vector>
#include <list>
#include <set>
#include <stack>
#include "limits.h"
#include "XWindowManagement.h"


#ifndef GAMEBOARDUTILITY_H
#define	GAMEBOARDUTILITY_H


#define BOARDSIZE 5
#define UNDEF     -1
#define PLY       3


enum Owner
{
    NONE = 0,
    PLAYER_A='A',
    PLAYER_B='B'
};

struct Move
{
    int move,score;
};

/* linked list, with vertex coord */
struct neighbourVertex
{
   int row, column;
   neighbourVertex *next;
};

/* vertex coord, and neighbour vertices */
struct Vertex
{
    int row, column, zCoord;
    Owner vertex_owner;
    neighbourVertex *neighbours, *virtual_neighbours;
};


struct Board
{
    std::vector<Vertex> flattenBoard;
    std::vector<std::vector<Owner> > UIBoard;
    std::vector<std::vector<int> > indexLookup;
    std::stack<int> moveHistory;
};

typedef std::set<int> SpanningTree;
typedef std::vector<SpanningTree> SpanningForrest;


struct Player
{
    Owner owner;
    std::list<int> *ownedMoves[BOARDSIZE*BOARDSIZE];
    SpanningForrest forrest;
    std::list<int> moveHistory;
    std::vector<int> connected;

};

struct Players
{
   Player *A, *B;
};

std::vector<Move> positionMobility(const Board *board,
                                   const Player *playerPerspective,
                                   std::vector<int> candidateMoves);

int playMove(XWinManage *xManager, Board *board, std::list<int>openMoves, Player *p1, Player *p2 );

std::list<int> availableMoves(const Board *board);

int firstMove(Board *board);

int distance(const Board *board, int source, int destination );

void XWindowsCleanUp(struct XWinManage *xManager);

void initgameboard(Board *board, Players *players);

bool makeanswermove(XWinManage *xManager, Board *board, Players *players);


void drawgameboard(XWinManage *xManager, const std::vector<std::vector<Owner> > &gameBoard);

/* Checks for out of bound gameboard coordinates. */
int is_valid_coord(int current_row, int current_column);

/* Detects a vertex real-neighbours and virtual-neighbours. */
void obtain_neighbouring_vertices_coordinates(Vertex &vert,int curent_row,
                                              int current_column);

/* builds linked list: each Node is a neighbour of caller */
void add_neighbour_vertex(neighbourVertex **neighbour, int neighbour_row,
                         int neighbour_column);

int vertex_index_lookup(const int (*lookup_table)[BOARDSIZE],int row, int column);

void acquaire_neighbour_vertices(Board *board, Player *player, int move_id );

/* Depth First Search Algorithm employed, to find connected components */
bool is_won_game(Board *board, Player *player);

void available_moves(std::vector<int> &emptyVc, std::vector<std::vector<Owner> > gameBoard );


Move negmax(XWinManage *xManager, Board *board, std::list<int> openMoves, Player *p1, Player *p2, int ply);


int evaluateGameBoard(Board *board, Player *player );

void make_move( Board *board, int move_id, Player *player);

void undo_move( Board *board, int move_id, Player *player);

#endif
/* GAMEBOARDUTILITY_H */
