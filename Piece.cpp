#include "Piece.h"
#include "Board.h"
#include "Moves.h"
#include "Move.h"

#include <iostream>
using namespace std;

int Piece::mailbox[120] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7, -1, 
    -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
    -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
    -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
    -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
    -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
    -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
    -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

int Piece::mailbox2board[64] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

Piece::~Piece() {}

bool Piece::CrawlerAttacks(int destination) const {
    for(int i=0; i<nRay; ++i) {
        if( destination == mailbox[ mailbox2board[square] + ray[i] ] ) return true;
    }
    return false;
}

bool Piece::CrawlerCanMoveTo(int destination, Board& board) const {
    // cannot move to a square of same side
    if( board.IsSide(side, destination) ) return false;
    
    return CrawlerAttacks(destination);
}

bool Piece::SliderAttacks(int destination, Board& board) const {
    for(int i=0; i<nRay; ++i) {
        for(int step=1; step<8; ++step) {
            int result = mailbox[ mailbox2board[square] + ray[i]*step ];
            if(result == -1) break;
            if(destination == result) return true;
            if(board.IsOccupied(result)) break;
        }
    }
    return false;
}

bool Piece::SliderCanMoveTo(int destination, Board& board) const {
    // cannot move to a square of same side
    if( board.IsSide(side, destination) ) return false;
    
    return SliderAttacks(destination, board);
}

bool Piece::CrawlerGenerateMoves(Moves& moves, Board& board) const {
    for(int i=0; i<nRay; ++i) {
        int to = mailbox[ mailbox2board[square] + ray[i] ];
        // outside: ignore
        if(to == -1) continue;
        // empty: create move
        else if ( board.IsEmpty(to) ) moves.Insert(Move(square,to));
        // own side: ignore
        else if ( board.IsSide(side, to) ) continue;
        // opposite king: position is invalid
        else if ( board.IsKing(to) ) return false;
        // capture: create Move
        else moves.Insert(Move(square, to));
    }
    return true;
}

bool Piece::SliderGenerateMoves(Moves& moves, Board& board) const {
    for(int i=0; i<nRay; ++i) {
        for(int step=1; step<8; ++step) {
            int to = mailbox[ mailbox2board[square] + ray[i]*step ];
            if(to == -1) {
                break;
            }
            if(board.IsEmpty(to)) {
                moves.Insert(Move(square, to));
                continue;
            } else if(board.IsSide(side, to)) {
                break;
            }
            else if(board.IsKing(to)) {
                return false;
            }
            else {
                moves.Insert(Move(square, to));
                break; 
            }
        }
    }
    return true;
}



King::King(Side s, int square) : Piece(s, square) {
    if(side == WHITE){
    	shortName = 'K';
    	score = KING;
    } else {
    	shortName = 'k';
    	score = -KING;
    }
    nRay = 8;
    ray[0] = -11;
    ray[1] = -10;
    ray[2] = -9;
    ray[3] = -1;
    ray[4] = 1;
    ray[5] = 9;
    ray[6] = 10;
    ray[7] = 11;
}

bool King::Attacks(int destination, Board&) const { 
    return CrawlerAttacks(destination); 
}

bool King::CanMoveTo(int destination, Board& board) const { 
    return CrawlerCanMoveTo(destination, board); 
}

bool King::GenerateMoves(Moves& moves, Board& board) const {
    return CrawlerGenerateMoves(moves, board);
}


Queen::Queen(Side s, int square) : Piece(s, square) {
    if(side == WHITE){
    	shortName = 'Q';
    	score = QUEEN;
    } else {
    	shortName = 'q';
    	score = -QUEEN;
    }
    nRay = 8;
    ray[0] = -11;
    ray[1] = -10;
    ray[2] = -9;
    ray[3] = -1;
    ray[4] = 1;
    ray[5] = 9;
    ray[6] = 10;
    ray[7] = 11;
}

bool Queen::Attacks(int destination, Board& board) const { 
    return SliderAttacks(destination, board); 
}

bool Queen::CanMoveTo(int destination, Board& board) const { 
    return SliderCanMoveTo(destination, board); 
}

bool Queen::GenerateMoves(Moves& moves, Board& board) const {
    return SliderGenerateMoves(moves, board);
}


Rock::Rock(Side s, int square) : Piece(s, square) {
    if(side == WHITE){
     	shortName = 'R';
     	score = ROCK;
     } else {
     	shortName = 'r';
     	score = -ROCK;
     }
    nRay = 4;
    ray[0] = -10;
    ray[1] = -1;
    ray[2] = 1;
    ray[3] = 10;
}

bool Rock::Attacks(int destination, Board& board) const { 
    return SliderAttacks(destination, board); 
}

bool Rock::CanMoveTo(int destination, Board& board) const { 
    return SliderCanMoveTo(destination, board); 
}

bool Rock::GenerateMoves(Moves& moves, Board& board) const {
    return SliderGenerateMoves(moves, board);
}

Bishop::Bishop(Side s, int square) : Piece(s, square) {
    if(side == WHITE){
     	shortName = 'B';
     	score = BISHOP;
     } else {
     	shortName = 'b';
     	score = -BISHOP;
     }
    nRay = 4;
    ray[0] = -11;
    ray[1] = -9;
    ray[2] = 9;
    ray[3] = 11;
}

bool Bishop::Attacks(int destination, Board& board) const { 
    return SliderAttacks(destination, board); 
}

bool Bishop::CanMoveTo(int destination, Board& board) const {
    return SliderCanMoveTo(destination, board); 
}

bool Bishop::GenerateMoves(Moves& moves, Board& board) const {
    return SliderGenerateMoves(moves, board);
}

Knight::Knight(Side s, int square) : Piece(s, square) {
    if(side == WHITE){
     	shortName = 'N';
     	score = KNIGHT;
     } else {
     	shortName = 'n';
     	score = -KNIGHT;
     }
    nRay = 8;
    ray[0] = -21;
    ray[1] = -19;
    ray[2] = -12;
    ray[3] = -8;
    ray[4] = 8;
    ray[5] = 12;
    ray[6] = 19;
    ray[7] = 21;
}

bool Knight::Attacks(int destination, Board& ) const { 
   return CrawlerAttacks(destination); 
}

bool Knight::CanMoveTo(int destination, Board& board) const { 
    return CrawlerCanMoveTo(destination, board); 
}

bool Knight::GenerateMoves(Moves& moves, Board& board) const {
    return CrawlerGenerateMoves(moves, board);
}

Pawn::Pawn(Side s, int square) : Piece(s, square) {
    if(side == WHITE) {
        shortName = 'P';
        score = PAWN;
        ray[0] = -10;
        ray[1] = -11;
        ray[2] = -9;

        // misuse of nRay to signal pawn on starting position
        if(47 < square && square < 56) nRay = 1;
        else                           nRay = 0;
    } else {
        shortName = 'p';
        score = - PAWN;
        ray[0] = 10;
        ray[1] = 11;
        ray[2] = 9;
 
        // misuse of nRay to signal pawn on starting position
        if(7 < square && square < 16) nRay = 1;
        else                           nRay = 0;
    }

}

bool Pawn::Attacks(int destination, Board &) const {
    if( destination == mailbox[ mailbox2board[square] + ray[1] ] ) return true;
    if( destination == mailbox[ mailbox2board[square] + ray[2] ] ) return true;
    return false;
}

bool Pawn::CanMoveTo(int destination, Board& board) const {
    // cannot move to a square of same side
    if( board.IsSide(side, destination) ) return false;
    
    // capture
    if( destination == mailbox[ mailbox2board[square] + ray[1] ]  && board.IsOccupied(destination) ) return true;
    if( destination == mailbox[ mailbox2board[square] + ray[2] ]  && board.IsOccupied(destination) ) return true;
    
    //advance one step
    int onestep = mailbox[ mailbox2board[square] + ray[0] ];
    if( board.IsOccupied(onestep) ) return false;
    if( destination == onestep ) return true;
    
    //advance two steps
    if( nRay == 0 || board.IsOccupied(onestep)) return false;
    if( destination == mailbox[ mailbox2board[onestep] + ray[0] ] ) return true;
 
    return false;
}

bool Pawn::GenerateMoves(Moves& moves, Board& board) const {
    //cout << " Pawn::GenerateMoves " << endl;
    //capture 
    int to;
    //cout << " Pawn::GenerateMoves capture" << endl;
    for(int i=1; i<3; ++i) {
        //cout << " Pawn::GenerateMoves capture i:" << i << endl;
        to = mailbox[ mailbox2board[square] + ray[i] ];
        if (to == -1) continue;
        //cout << " Pawn::GenerateMoves capture to:" << to << endl;
        if(board.IsOccupied(to)) {
            if(!board.IsSide(side,to) ) {
                if(board.IsKing(to)) return false;
                else moves.Insert(Move(square,to));
            }
        }
    }

    //advance one step   
    //cout << " Pawn::GenerateMoves one step" << endl;

    to = mailbox[ mailbox2board[square] + ray[0] ];
    if( board.IsEmpty(to) ) {
        moves.Insert(Move(square, to));

        if( nRay == 1 ) {
            // advance on more step
            to = mailbox[ mailbox2board[to] + ray[0] ];
            if( board.IsEmpty(to) ) moves.Insert(Move(square, to));
        }
    }

    return true;
}

