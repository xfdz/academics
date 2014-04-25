#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>

#include "GameBoardUtility.h"
#include "XWindowManagement.h"


using std::vector;
using std::list;


double radius;

int main(int argc, char **argv)
{


    Board *board = new Board;
    Players *players = new Players;

    vector<vector<Owner> > gameBoard;
    struct XWinManage *xManager = new XWinManage();
    xManager->display;

    initgameboard(board, players);
    initXlib(argc, argv,xManager);

    int vertex_index, numberOfMoves;
    /* and now it starts: the event loop */
    while (1)
    {
        XNextEvent(xManager->display, &xManager->report);
        switch (xManager->report.type)
        {
            case Expose:
                /* draw the game board */
                drawgameboard(xManager, board->UIBoard);
                break;
            case ConfigureNotify:
                xManager->win_width = xManager->report.xconfigure.width;
                xManager->win_height = xManager->report.xconfigure.height;
                radius = (double) xManager->win_height / (4.0 * BOARDSIZE + 2.0);
                if (radius > ((double) xManager->win_width /
                    (8 * sqrt(0.75)*(BOARDSIZE - 1) + 6)))
                    radius = ((double) xManager->win_width /
                             (8 * sqrt(0.75)*(BOARDSIZE - 1) + 6));
                break;
            case ButtonPress:
            {

                int i, j, x, y;
                for (i = 0; i < BOARDSIZE; i++)
                {
                    for (j = 0; j < BOARDSIZE; j++)
                    {
                        // ind the field where button was pressed
                        x = 3 * radius + 4 * radius * (i + j) * sqrt(0.75);
                        y = (2 * BOARDSIZE + 1 - 2 * (i - j)) * radius;

                        double result =
                                (xManager->report.xbutton.x - x)*(xManager->report.xbutton.x - x) +
                                (xManager->report.xbutton.y - y)*(xManager->report.xbutton.y - y);

                        if ((result < radius*radius)&&( board->UIBoard[i][j] == NONE))
                        {
                            // game board piece is now player A'sboard->moveHistory.push(vertex_index);
                            board->UIBoard[i][j] = PLAYER_A;
                            drawgameboard(xManager,  board->UIBoard);
                            vertex_index = board->indexLookup[i][j];
                            board->moveHistory.push(vertex_index);
                            board->flattenBoard[vertex_index].vertex_owner = PLAYER_A;

                            players->A->ownedMoves[vertex_index] = new list<int>();
                            acquaire_neighbour_vertices(board, players->A, vertex_index);

                            bool result = is_won_game(board, players->A);



printf("####################\n");
for(int i=0; i != players->A->connected.size(); ++i)
{
    printf("Player A, index: %d, component: %d\n", i, players->A->connected.at(i));
}
printf("\n\n");
for(int i=0; i != BOARDSIZE*BOARDSIZE; ++i)
{
    if( players->A->ownedMoves[i] != NULL )
        printf("\n move index: %d, count connection: %d\n", i, players->A->ownedMoves[i]->size() );
}
printf("####################\n");





                            if( result )
                            {
                                printf("\nPlayer A has won\n");
                                goto WIN;
                            }

                            result=makeanswermove(xManager, board, players);

                            if( result ){
                                printf("\nPlayer B has won\n");
                                goto WIN;
                            }
                        }
                    }
                }

            }
                break;
            default:
                break;
        }

    }

WIN:

              
printf("SLEEP 30 SECONDS");
sleep(30);

    XWindowsCleanUp(xManager);

    return (EXIT_SUCCESS);
}

bool makeanswermove(XWinManage *xManager, Board *board, Players *players)

{
    list<int> openMoves = availableMoves(board);
    int pick = -1;

    if(board->moveHistory.size() == 1)
    {// first CPU move is chosen semi static
        pick = firstMove(board);
    }
    else
    {
        //pick = playMove(xManager, board, openMoves, players->B, players->A);
        Move move_struc = negmax(xManager,board, openMoves, players->B, players->A, PLY);
        pick = move_struc.move;
    }


    int row = board->flattenBoard[pick].row;
    int column = board->flattenBoard[pick].column;
    board->moveHistory.push(pick);
    board->UIBoard[row][column]= PLAYER_B;
    board->flattenBoard[pick].vertex_owner = PLAYER_B;
    drawgameboard(xManager, board->UIBoard);

    players->B->ownedMoves[pick] = new list<int>();
    acquaire_neighbour_vertices(board, players->B, pick);



printf("####################\n");
for(int i=0; i != players->B->connected.size(); ++i)
{
    printf("Player B, index: %d, component: %d\n", i, players->B->connected.at(i));
}

printf("\n\n");
for(int i=0; i != BOARDSIZE*BOARDSIZE; ++i)
{
    if( players->B->ownedMoves[i] == NULL )
        continue;

    printf("\n move index: %d, count connection: %d\n", i, players->B->ownedMoves[i]->size() );
}
printf("####################\n");

    bool result = is_won_game(board, players->B);

    return ( result ) ? true : false;
}

int playMove(XWinManage *xManager, Board *board, list<int>openMoves, Player *p1, Player *p2 )
{
    int playedPosition = -1;

    // last move opponent made
    if( board->moveHistory.size() == 0)
    {
        printf("\nERROR: empty stack  in playMove()");
        exit(0);
    }
    int lastMove = board->moveHistory.top();

    //get opponent last moves virtual neighbours
    neighbourVertex *virtNeigh = NULL;
    virtNeigh = board->flattenBoard[lastMove].virtual_neighbours;

    std::vector<int> killerMoves;
    while( virtNeigh != NULL )
    {// use only those moves not taken

        int row = virtNeigh->row;
        int column = virtNeigh->column;

        if( board->UIBoard[row][column] == NONE)
        {   int currentMove = board->indexLookup[row][column];
            killerMoves.push_back(currentMove);
        }

        virtNeigh = virtNeigh->next;
    }

    if( killerMoves.size() == 0)
    {// get best move under conventional scenario analysis
        //exit(1);
        Move m = negmax(xManager, board,openMoves,p1,p2, PLY);
        playedPosition = m.move;
    }
    else
    {
        vector<Move> moves = positionMobility(board, p1, killerMoves);

        int lowestScore = 0;
        for(int i=0; i!= moves.size(); ++i)
        {// get the most blocked position - lowers score

            if( lowestScore <= moves[i].score )
            {
                lowestScore = moves[i].score;
                playedPosition = moves[i].move;

            }
        }
    }


    return playedPosition;
}

Move negmax(XWinManage *xManager, Board *board, list<int> openMoves, Player *p1, Player *p2, int ply)
{
    Move best = {0,0};

    if( ply == 0 || openMoves.size() == 0 )
    {
        // evaluate the game board for the sequence of moves made
        int board_value = evaluateGameBoard(board,p1);
        
        // retrieve move id once return from recursion
        best.move = 0;
        best.score = board_value;
        return best;
    }
    else if( ply != 0 && openMoves.size() != 0 )
    {

        Move evaluated_move;
        int current_move = -1;/* non-existant */
        list<int>::iterator iter_move = openMoves.begin();

        while( openMoves.size() != 0 )
        {
            // get the move identification
            current_move = *iter_move;

            // erasing element while iterating container, makes iterator invalid
            openMoves.erase(iter_move);

            // resetting iterator makes it valid
            iter_move = openMoves.begin();

            // make the move on the board game (internal - will not be shown )
            make_move(board,current_move,p1);


            // populate place holder with associates who are connected with it
            acquaire_neighbour_vertices(board,p1,current_move);

    //DEBUG
    
    Vertex v = board->flattenBoard[current_move];
    int row = v.row;
    int column = v.column;
    board->UIBoard[row][column]= p1->owner;
    drawgameboard(xManager, board->UIBoard);
    
    
            //evaluate the board game with this board position just taken
            evaluated_move = negmax(xManager, board, openMoves, p2, p1, ply-1 );

            // save move id and evaluation
            evaluated_move.move = current_move;

            // undo move just made in this iteration
            undo_move(board,current_move, p1);
     
    //DEBUG
    
    board->UIBoard[row][column]= NONE;
    printf("SLEEP 2 SECONDS");
    sleep(2);
    drawgameboard(xManager, board->UIBoard);
    

            if( -(evaluated_move.score) > best.score )
            {// opponent p2 always returns a negative number so we negate it

                best.move = evaluated_move.move;
                best.score= -(evaluated_move.score);
            }

        }
    }
    else
    {//DEBUG
        printf("BAAAAAD"); exit(1);
    }
    return best;
}

int firstMove(Board *board)
{// statically choose the first move: center of the board game

    if( board->moveHistory.empty() )
    {// verify a move by PLAYER_A was made
        printf("\nERROR: empty stack in firstMove()");
        exit(0);
    }
    int opponentMoveId = board->moveHistory.top();

    // get opponent's neighbouring positions
    neighbourVertex *virtList = NULL;
    virtList = board->flattenBoard[opponentMoveId].virtual_neighbours;

    // ideal region to choose a move from- center of the board
    int estimateCenter = (BOARDSIZE -1)/2;
    int centerBoardMove = board->indexLookup[estimateCenter][estimateCenter];

    int currentFurthestDistance = 0;
    int currentClosestDistance = BOARDSIZE * BOARDSIZE;
    int moveId = -1;

    while( virtList != NULL)
    {// find virtual neighbour furthest away from opponents
     // move and closest to the center of the board game.

        // get virtual neighbour move identification
        int neighbourId = board->indexLookup[virtList->row][virtList->column];

        // get the distance between opponent's move and it s virtual neighbour
        int furthestDistance  = distance(board,opponentMoveId,neighbourId);

        // get the distance between virtual neighbour and center of the board
        int closestDistance = distance(board,neighbourId,centerBoardMove );

        if( (currentFurthestDistance <= furthestDistance)
                && (closestDistance <= currentClosestDistance) )
        {
            currentFurthestDistance = furthestDistance;
            currentClosestDistance =  closestDistance;
            moveId = neighbourId;
        }

        virtList = virtList->next;
    }


    return moveId;
}

void initgameboard(Board *board, Players *players) {

    players->A = new Player;
    players->A->owner = PLAYER_A;
    players->A->connected.assign(BOARDSIZE,0);

    players->B = new Player;
    players->B->owner = PLAYER_B;
    players->B->connected.assign(BOARDSIZE,0);

    int i, j, count = 0;
    for (i = 0; i < BOARDSIZE; i++)
    {
        vector<int> tempVecforIndexLookUp;
        vector<Owner> tempVector(BOARDSIZE);
        for (j = 0; j < BOARDSIZE; j++, count++)
        {

            tempVector.push_back (NONE);

            tempVecforIndexLookUp.push_back(count);

            int zCoord = -(i+j);
            Vertex v = { i, j, zCoord, NONE, NULL, NULL };

            //vertex(i,j)'s neighbour and virtual neighbours
            obtain_neighbouring_vertices_coordinates(v, i, j);
            board->flattenBoard.push_back(v);

            int n = 0;
            neighbourVertex *tmp = board->flattenBoard[count].virtual_neighbours;
            while( tmp != NULL )
            {
                int x = tmp->row;
                int y = tmp->column;
                tmp = tmp->next;
                ++n;
            }

            /* player has not made a move yet.*/
            players->A->ownedMoves[count] = NULL;
            players->B->ownedMoves[count] = NULL;


        }

        board->UIBoard.push_back(tempVector);
        board->indexLookup.push_back(tempVecforIndexLookUp);
    }


}

void initXlib(int argc, char **argv, XWinManage *xManager){

    /* gameboard conventions here: '0' empty, 'A' user, 'B' program */
    char *display_name = NULL;
    unsigned int display_width, display_height;
    int win_x, win_y;
    int border_width;
    XWMHints *wm_hints;
    XClassHint *class_hints;
    XSizeHints *size_hints;
    XTextProperty win_name, icon_name;
    unsigned long valuemask = 0;
    XGCValues gc_values, gc_yellow_values, gc_red_values, gc_grey_values;
    Colormap color_map;
    XColor tmp_color1, tmp_color2;
    char *win_name_string  = const_cast<char*>("Example Window");
    char *icon_name_string = const_cast<char*>("Icon for Example Window");


    /* opening display: basic connection to X Server */
    if ((xManager->display = XOpenDisplay(display_name)) == NULL) {
        printf("Could not open display. \n");
        exit(-1);
    }

    printf("Connected to X server  %s\n", XDisplayName(display_name));
    int screen_num = DefaultScreen(xManager->display);
    //Screen *screen_ptr = DefaultScreenOfDisplay(xManager->display);
    color_map = XDefaultColormap(xManager->display, screen_num);
    display_width = DisplayWidth(xManager->display, screen_num);
    display_height = DisplayHeight(xManager->display, screen_num);

    printf("Width %d, Height %d, Screen Number %d\n",
            display_width, display_height, screen_num);

    /* creating the game window */
    border_width = 10;
    win_x = 0;
    win_y = 0;
    xManager->win_width = display_width / 2;
    xManager->win_height = (int) (xManager->win_width / 1.7);

    xManager->win = XCreateSimpleWindow(
            xManager->display, RootWindow(xManager->display, screen_num), win_x, win_y,
            xManager->win_width, xManager->win_height, border_width,
            BlackPixel(xManager->display, screen_num),
            WhitePixel(xManager->display, screen_num)
            );

    /* now try to put it on screen, this
     * needs cooperation of window manager
     */
    size_hints = XAllocSizeHints();
    wm_hints = XAllocWMHints();
    class_hints = XAllocClassHint();

    if (size_hints == NULL || wm_hints == NULL || class_hints == NULL) {
        printf("Error allocating memory for hints. \n");
        exit(-1);
    }

    size_hints->flags = PPosition | PSize | PMinSize;
    size_hints->min_width = 60;
    size_hints->min_height = 60;

    XStringListToTextProperty(&win_name_string, 1, &win_name);
    XStringListToTextProperty(&icon_name_string, 1, &icon_name);

    wm_hints->flags = StateHint | InputHint;
    wm_hints->initial_state = NormalState;
    wm_hints->input = False;

    class_hints->res_name = const_cast<char*> ("hex_example");
    class_hints->res_class = const_cast<char*> ("games");

    XSetWMProperties(xManager->display, xManager->win, &win_name, &icon_name,
            argv, argc, size_hints, wm_hints, class_hints
            );

    /* what events do we want to receive */
    XSelectInput(xManager->display, xManager->win,
            ExposureMask | StructureNotifyMask | ButtonPressMask
            );

    /* finally: put window on screen */
    XMapWindow(xManager->display, xManager->win);
    XFlush(xManager->display);

    /* create graphics context, so that we may draw in this window */
    xManager->gc = XCreateGC(xManager->display, xManager->win, valuemask, &gc_values);
    XSetForeground(xManager->display, xManager->gc, BlackPixel(xManager->display, screen_num));
    XSetLineAttributes(xManager->display, xManager->gc, 4, LineSolid, CapRound, JoinRound);

    /* and three other graphics contexts,
     * to draw in yellow and red and grey
     */
    xManager->gc_yellow = XCreateGC(xManager->display, xManager->win, valuemask, &gc_yellow_values);
    XSetLineAttributes(
            xManager->display, xManager->gc_yellow, 6, LineSolid, CapRound, JoinRound
            );

    int result =
            XAllocNamedColor(
            xManager->display, color_map, "yellow", &tmp_color1, &tmp_color2
            );

    if (result == 0) {
        printf("failed to get color yellow\n");
        exit(-1);
    } else {
        XSetForeground(xManager->display, xManager->gc_yellow, tmp_color1.pixel);
        xManager->gc_red = XCreateGC(xManager->display, xManager->win, valuemask, &gc_red_values);
        XSetLineAttributes(
                xManager->display, xManager->gc_red, 6, LineSolid, CapRound, JoinRound
                );
    }

    result =XAllocNamedColor(xManager->display, color_map, "red",
                             &tmp_color1, &tmp_color2);

    if (result == 0) {
        printf("failed to get color red\n");
        exit(-1);
    } else {
        XSetForeground(xManager->display, xManager->gc_red, tmp_color1.pixel);
        xManager->gc_grey = XCreateGC(xManager->display, xManager->win, valuemask, &gc_grey_values);
    }

    result =
            XAllocNamedColor(
            xManager->display, color_map, "light grey", &tmp_color1, &tmp_color2
            );

    if (result == 0) {
        printf("failed to get color grey\n");
        exit(-1);
    } else
        XSetForeground(xManager->display, xManager->gc_grey, tmp_color1.pixel);


}

void drawgameboard(XWinManage *xManager, const vector<vector<Owner> > &gameBoard) {
    int i, j;
    int x, y;

    /* draw the grid */
    for (i = 0; i < BOARDSIZE - 1; i++) {
        for (j = 0; j < BOARDSIZE - 1; j++) {

            x = 3 * radius + 4 * radius * (i + j) * sqrt(0.75);
            y = (2 * BOARDSIZE + 1 - 2 * (i - j)) * radius;

            XDrawLine(xManager->display, xManager->win, xManager->gc, x, y,
                    (int) (x + 4 * radius * sqrt(0.75)),(int) (y - 2 * radius));

            XDrawLine(xManager->display, xManager->win, xManager->gc, x, y,
                    (int) (x + 4 * radius * sqrt(0.75)),(int) (y + 2 * radius));

            XDrawLine(xManager->display, xManager->win, xManager->gc,
                    (int) (x + 4 * radius * sqrt(0.75)),(int) (y - 2 * radius),
                    (int) (x + 4 * radius * sqrt(0.75)),(int) (y + 2 * radius));

            XDrawLine(xManager->display, xManager->win, xManager->gc,
                    (int) (x + 4 * radius * sqrt(0.75)),(int) (y - 2 * radius),
                    (int) (x + 8 * radius * sqrt(0.75)), y);

            XDrawLine(xManager->display, xManager->win, xManager->gc,
                    (int) (x + 4 * radius * sqrt(0.75)),(int) (y + 2 * radius),
                    (int) (x + 8 * radius * sqrt(0.75)), y);
        }
    }

    /* draw the positions: A yellow, B red, empty gray */
    for (i = 0; i < BOARDSIZE; i++) {
        for (j = 0; j < BOARDSIZE; j++) {
            x = 3 * radius + 4 * radius * (i + j) * sqrt(0.75);
            y = (2 * BOARDSIZE + 1 - 2 * (i - j)) * radius;


            switch (gameBoard[i][j] )
            {
                case NONE :
                    XFillArc(xManager->display, xManager->win, xManager->gc_grey, (int) x - radius,
                            (int) y - radius, (int) 2 * radius,
                            (int) 2 * radius, 0, 360 * 64);
                    break;
                case PLAYER_A:
                    XFillArc(xManager->display, xManager->win, xManager->gc_yellow, (int) x - radius,
                            (int) y - radius, (int) 2 * radius,
                            (int) 2 * radius, 0, 360 * 64);
                    break;
                case PLAYER_B:
                    XFillArc(xManager->display, xManager->win, xManager->gc_red,
                            (int) x - radius, (int) y - radius,
                            (int) 2 * radius, (int) 2 * radius, 0, 360 * 64);
                    break;
                default:
                    printf("bad entry in gameBoard: %c at %d, %d.\n",
                            gameBoard[i][j], i, j);
                    break;
            }

            XDrawArc(xManager->display, xManager->win, xManager->gc, (int) x - radius, (int) y - radius,
                    (int) 2 * radius, (int) 2 * radius, 0, 360 * 64
                    );
        }
    }

    /* draw the red pair of sides and the yellow pair of sides */
    XDrawLine(xManager->display, xManager->win, xManager->gc_red,
            (int) 3 * radius - 4 * radius * sqrt(0.75),
            (int) (2 * BOARDSIZE + 3) * radius,
            (int) 3 * radius + 4 * radius * (-2 + BOARDSIZE) * sqrt(0.75),
            (int) (4 * BOARDSIZE + 1) * radius
            );
    XDrawLine(xManager->display, xManager->win, xManager->gc_red,
            (int) 3 * radius + 4 * radius * (BOARDSIZE) * sqrt(0.75),
            (int) radius,
            (int) 3 * radius + 4 * radius * (2 * BOARDSIZE - 1) * sqrt(0.75),
            (int) (2 * BOARDSIZE - 1) * radius
            );
    XDrawLine(xManager->display, xManager->win, xManager->gc_yellow,
            (int) 3 * radius - 4 * radius * sqrt(0.75),
            (int) (2 * BOARDSIZE - 1) * radius,
            (int) 3 * radius + 4 * radius * (BOARDSIZE - 2) * sqrt(0.75),
            (int) (2 * BOARDSIZE + 1 - 2 * (BOARDSIZE)) * radius
            );
    XDrawLine(xManager->display, xManager->win, xManager->gc_yellow,
            (int) 3 * radius + 4 * radius * (BOARDSIZE) * sqrt(0.75),
            (int) (4 * BOARDSIZE + 1) * radius,
            (int) 3 * radius + 4 * radius * (2 * BOARDSIZE - 1) * sqrt(0.75),
            (int) (2 * BOARDSIZE + 3) * radius
            );

    XFlush(xManager->display);
}

void XWindowsCleanUp(struct XWinManage *xManager)
{
    // clean up
    XFreeGC(xManager->display, xManager->gc);
    XFreeGC(xManager->display, xManager->gc_grey);
    XFreeGC(xManager->display, xManager->gc_red);
    XFreeGC(xManager->display, xManager->gc_yellow);
    XCloseDisplay(xManager->display);
    delete xManager;
}


