#include <cstdlib>
#include <list>
#include <vector>
#include "GameBoardUtility.h"
#include <time.h>
#include <algorithm>
#include <cstdio>


void obtain_neighbouring_vertices_coordinates(Vertex &vert, int curent_row,
                                              int current_column)
{  // current_row and current_column is origin in the coordinate plane.
   // Col   Col   Col   Col
   // Row: (2,0) (2,1) (2,2)
   // Row: (1,0) (1,1) (1,2)
   // Row: (0,0) (0,1) (0,2)

    int tmp_row = UNDEF, tmp_col = UNDEF, result = 0;

    // ClockWise - north vertex
    tmp_row = curent_row+1; tmp_col = current_column;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.neighbours, tmp_row, tmp_col);

    // virtual: north-east vertex  - add to both lists
    tmp_row = curent_row+1; tmp_col = current_column+1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.virtual_neighbours, tmp_row, tmp_col);

    // east vertex
    tmp_row = curent_row; tmp_col = current_column+1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
       add_neighbour_vertex(&vert.neighbours, tmp_row, tmp_col);

    // virtual: south east
    tmp_row = curent_row-1; tmp_col = current_column+2;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.virtual_neighbours, tmp_row, tmp_col);


    // sourth-east vertex
    tmp_row = curent_row-1; tmp_col = current_column+1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result)
        add_neighbour_vertex(&vert.neighbours, tmp_row, tmp_col);

    // virtual: south east
    tmp_row = curent_row-2; tmp_col = current_column+1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result)
        add_neighbour_vertex(&vert.virtual_neighbours, tmp_row, tmp_col);

    /* south vertex */
    tmp_row = curent_row-1; tmp_col = current_column;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.neighbours, tmp_row, tmp_col);

    // virtual: south west
    tmp_row = curent_row-1; tmp_col = current_column - 1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.virtual_neighbours, tmp_row, tmp_col);

    // west vertex
    tmp_row = curent_row; tmp_col = current_column-1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.neighbours, tmp_row, tmp_col);

    // virtual: north west
    tmp_row = curent_row + 1; tmp_col = current_column-2;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.virtual_neighbours, tmp_row, tmp_col);

    // north west vertex
    tmp_row = curent_row+1; tmp_col = current_column-1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.neighbours, tmp_row, tmp_col);

    // virtual: north west
    tmp_row = curent_row+2; tmp_col = current_column-1;
    result = is_valid_coord(tmp_row,tmp_col);
    if( result )
        add_neighbour_vertex(&vert.virtual_neighbours, tmp_row, tmp_col);

}

int is_valid_coord(int current_row, int current_column )
{
    return( (current_row >= 0 && current_column >= 0) &&
            (current_row < BOARDSIZE && current_column < BOARDSIZE )) ? 1 : 0;
}

void add_neighbour_vertex(neighbourVertex **neighbour,int neighbour_row,
                         int neighbour_column)
{
    neighbourVertex *temp_neighbour = new neighbourVertex;
    temp_neighbour->row   = neighbour_row;
    temp_neighbour->column= neighbour_column;
    temp_neighbour->next  = *neighbour;
    *neighbour            = temp_neighbour;
}


void acquaire_neighbour_vertices(Board *board, Player *player, int move_id )
{

    // from the board get vertex that represents the chosen move
    neighbourVertex *neighbours = NULL;
    Vertex chosenMoveVertex = board->flattenBoard[move_id];

    // get its neighbours
    neighbours = chosenMoveVertex.neighbours;


    for(int index = -1; neighbours != NULL; neighbours = neighbours->next, index = -1 )
    {// update the players list of

        // get this move's identification number
        index = board->indexLookup[neighbours->row][neighbours->column];

        if( index < 0 || index == (BOARDSIZE * BOARDSIZE) )
        {// index out of range

            printf("\nERROR: out of range[%d] in acquaire_neighbour_vertices()",
                   index);
            exit(0);
        }

        // An owned vertex means it was previously selected by this player
        if (board->flattenBoard[index].vertex_owner != player->owner)
            continue;

        // add this move to its neighbours, connecting them
        player->ownedMoves[index]->push_back(move_id);


        // add the index of neighbour vertex
        player->ownedMoves[move_id]->push_back(index);

    }
}

void relinquish_neighbour_vertices(Board *board, Player *player, int move_id )
{
    // get vertex of this position
    neighbourVertex *neighbours = NULL;
    Vertex chosenMoveVertex = board->flattenBoard[move_id];

    // get position neighbours list
    neighbours = chosenMoveVertex.neighbours;

    for(int index = -1; neighbours != NULL; neighbours = neighbours->next, index = -1)
    {
        index = -1;
        index = board->indexLookup[neighbours->row][neighbours->column];

        if( index < 0 || index == (BOARDSIZE * BOARDSIZE) )
        {/* index out of range */
            printf("\nERROR: out of range[%d] in acquaire_neighbour_vertices()",
                   index);exit(0);
        }

         /* An owned vertex means: previously selected by this player */
        if (board->flattenBoard[index].vertex_owner != player->owner)
            continue;

        /* remove the index of neighbour vertex */
        player->ownedMoves[index]->remove(move_id);

        /* vertex neighbour conectedness remove */
        player->ownedMoves[move_id]->remove(index);

    }
}


bool is_won_game(Board *board, Player *player)
{
    int max = BOARDSIZE*BOARDSIZE;
    bool visitedVertices[max];
    for(int i=0; i!=max; ++i)
        visitedVertices[i] = false;

    std::list<int> visitingVertices;
    std::list<int>::const_iterator iter_neighbours;

    for(int i=0; i != max; ++i)
    {// search for vertices owned and not visited by player

        if(player->ownedMoves[i] == NULL || visitedVertices[i] == true)
            continue;

        SpanningTree spanTrees;
        visitingVertices.push_back(i);

        while( !visitingVertices.empty() )
        { //visit those vertices */
            int index = visitingVertices.back();
            visitingVertices.pop_back();

            if(player->ownedMoves[index]== NULL || visitedVertices[index]==true)
               continue;

            visitedVertices[index] = true;
            spanTrees.insert(index);

            iter_neighbours = player->ownedMoves[index]->begin();
            for(;iter_neighbours!=player->ownedMoves[index]->end(); ++iter_neighbours)
            {
                if(visitedVertices[*iter_neighbours] == true)
                    continue;
                visitingVertices.push_back(*iter_neighbours);
            }
        }

        if( spanTrees.size () != 0 )
            player->forrest.push_back(spanTrees);
    }


    /*
     * Determine if player has won, this happens when connecting two sides. Sides are connected
     * when the appropriate side coordinates have been selected. It depends on
     * the type of player: PLAYER_A connects horizontally, PLAYER_B connects
     * vertically.  
     */

    player->connected.assign(BOARDSIZE,0);

    int component_1, component_2;
    SpanningForrest::const_iterator iter_sp_f = player->forrest.begin();
    for(; iter_sp_f != player->forrest.end(); ++iter_sp_f)
    {// check for game ending chain in list of chains

        component_1 = component_2 = 0;
        SpanningTree::const_iterator iter_sp_t = iter_sp_f->begin();
        for(; iter_sp_t != iter_sp_f->end(); ++iter_sp_t )
        {// check for game ending chain

            printf("\n**********\n");
            printf( "\n (row:%d,col:%d), iter->index:%d\n",
                    board->flattenBoard[*iter_sp_t].row,
                    board->flattenBoard[*iter_sp_t].column,
                    *iter_sp_t );

            if( player->owner == PLAYER_A){
            // player_A wins connecting vertically

                if( component_1 == 0 && 0 == board->flattenBoard[*iter_sp_t].column)
                // any vertex from the bottom game board row
                    component_1 +=1;

                if( component_2 == 0 && BOARDSIZE-1 == board->flattenBoard[*iter_sp_t].column)
                // any vertex from the top game board row
                    component_2 +=1;


                int row = board->flattenBoard[*iter_sp_t].row;
                player->connected[row] += 1;
            }
            else
            {// player_B wins connecting horizontally

                if( component_1 == 0 && 0 == board->flattenBoard[*iter_sp_t].row)
                // any vertex from the left game board column
                    component_1 +=1;

                if( component_2 == 0 && BOARDSIZE-1 == board->flattenBoard[*iter_sp_t].row )
                // any vertex from the right game board column
                    component_2 +=1;

                int column = board->flattenBoard[*iter_sp_t].column;
                player->connected[column] += 1;

            }

            if( 2 <= component_1 + component_2 )
                return true;
        }
    }

    player->forrest.clear();



    return false;
}


int evaluateGameBoard(Board *board, Player *player )
{
    bool result = is_won_game(board,player);
    
    if( result == true )
        return ( (BOARDSIZE-1*BOARDSIZE-1)/2 + BOARDSIZE-1);

    int longestConnection = 0;
    for(int i=0; i != board->flattenBoard.size(); ++i)
    {// find the longest connected component in the board
        if( player->ownedMoves[i] == NULL )
            continue;

        int currentLength = player->ownedMoves[i]->size();
        if( longestConnection < currentLength )
                longestConnection = currentLength;
    }


    int mostProgress = 0;
    for(int i=0; i != player->connected.size(); ++i)
    {
        int currentProgress = player->connected.at(i);
        if( mostProgress < currentProgress)
               mostProgress = currentProgress;
    }

    return (( longestConnection *.25) + mostProgress);


}

void make_move( Board *board, int move_id, Player *player)
{

    board->flattenBoard[move_id].vertex_owner = player->owner;
    player->ownedMoves[move_id]  = new std::list<int>();
    acquaire_neighbour_vertices(board, player,move_id);
    
}

void undo_move( Board *board, int move_id, Player *player)
{

    relinquish_neighbour_vertices(board, player, move_id);
    board->flattenBoard[move_id].vertex_owner = NONE;

    delete player->ownedMoves[move_id];
    player->ownedMoves[move_id] = NULL;


}

int distance(const Board *board, int source, int destination )
{
    // get source coordinates
    int sourceXcoord = board->flattenBoard[source].row;
    int sourceYcoord = board->flattenBoard[source].column;

    // get destination coordinate
    int destinationXcoord = board->flattenBoard[destination].row;
    int destinationYcoord = board->flattenBoard[destination].column;

    int xDelta = (sourceXcoord - destinationXcoord );
    int yDelta = (sourceYcoord - destinationYcoord );

    int result = xDelta * yDelta;

    int distance = -1;
    if ( result > 0 )
        distance = std::abs(xDelta) + std::abs(yDelta);
    else
        distance = std::max(xDelta,yDelta);

    return distance;

}


std::list<int> availableMoves(const Board *board)
{

    int index =0;
    std::list<int> openMoves;
    for(int i=0; i != BOARDSIZE; ++i)
    {// get list of available positions
        for(int j=0; j != BOARDSIZE; ++j)
        {
            //moves not owned by anyone
            if( board->UIBoard[i][j] == NONE )
                openMoves.push_back(index);

            ++index;
        }
    }

    return openMoves;
}


std::vector<Move> positionMobility(const Board *board,
                                   const Player *player,
                                   std::vector<int> candidateMoves)
{
    // Lower number means less opposition players moves
    // Higher number means more opposition players moves

    std::vector<Move> moveScores;
    for(int i=0; i != candidateMoves.size(); ++i)
    {
        Move currentMove;
        Vertex *vertex = NULL;
        neighbourVertex *neigh = NULL;
        neighbourVertex *vneigh= NULL;
        int surroundingOpponents = 0;

        // get moves neighbours
        int moveId = candidateMoves[i];
        neigh = board->flattenBoard[moveId].neighbours;
        vneigh= board->flattenBoard[moveId].neighbours;

        Owner opponent = ( player->owner == PLAYER_A) ? PLAYER_B : PLAYER_A;

        currentMove.move = moveId;
        currentMove.score= -1;

        while( neigh != NULL )
        {
            int row    = neigh->row;
            int column = vneigh->column;

            if( board->UIBoard[row][column] == opponent)
            {
                ++surroundingOpponents;
            }

            neigh = neigh->next;
        }

        currentMove.score = surroundingOpponents;
        moveScores.push_back( currentMove);

    }


    return moveScores;
}

