#ifndef PIECE_H
#define PIECE_H

typedef std::vector<std::vector<bool>> PieceMap;

static PieceMap iPiece = {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}};
static PieceMap oPiece = {{0,0,0},{0,1,1},{0,1,1}};
static PieceMap jPiece = {{0,0,0},{1,1,1},{0,0,1}};
static PieceMap tPiece = {{0,0,0},{1,1,1},{0,1,0}};
static PieceMap lPiece = {{0,0,0},{1,1,1},{1,0,0}};
static PieceMap sPiece = {{0,0,0},{0,1,1},{1,1,0}};
static PieceMap zPiece = {{0,0,0},{1,1,0},{0,1,1}};

const std::vector<PieceMap> pieces = {iPiece, oPiece, tPiece, lPiece,
jPiece, sPiece, zPiece};

struct GamePiece {
	int x,y,rot;
	PieceMap map;
};

#endif
