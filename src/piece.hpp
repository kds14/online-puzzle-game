#ifndef PIECE_H
#define PIECE_H

typedef std::vector<std::vector<uint8_t>> PieceMap;

static PieceMap iPiece = {{0,0,0},{1,1,1},{0,0,0}};
static PieceMap jPiece = {{1,1},{0,1}};
static PieceMap tPiece = {{0,0,0},{1,1,1},{0,1,0}};

const std::vector<PieceMap> pieces = {jPiece, tPiece, iPiece};

struct GamePiece {
	int8_t x,y,rot;
	PieceMap map;
};

#endif
