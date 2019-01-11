#ifndef PIECE_H
#define PIECE_H

typedef std::vector<std::vector<bool>> PieceMap;

static PieceMap oPiece = {{0,1,1},{0,1,1}};
static PieceMap jPiece = {{1,1,1},{0,0,1}};
static PieceMap tPiece = {{1,1,1},{0,1,0}};
static PieceMap lPiece = {{1,1,1},{1,0,0}};
static PieceMap iPiece = {{1,1,1,1}};
static PieceMap sPiece = {{0,1,1},{1,1,0}};
static PieceMap zPiece = {{1,1,0},{0,1,1}};

const std::vector<PieceMap> pieces = {oPiece, jPiece, tPiece, lPiece, iPiece, sPiece, zPiece};

struct GamePiece {
	int x,y,rot;
	PieceMap map;
};

#endif
