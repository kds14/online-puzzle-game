#ifndef PIECE_H
#define PIECE_H

#define CLEAR_PIECE 1
#define JUNK_PIECE 2
#define RED_PIECE 3
#define GREEN_PIECE 4
#define BLUE_PIECE 5

typedef std::vector<std::vector<uint8_t>> PieceMap;

static PieceMap iPiece = {{0,0,0},{1,1,1},{0,0,0}};
static PieceMap jPiece = {{1,1},{0,1}};
static PieceMap tPiece = {{0,0,0},{1,1,1},{0,1,0}};
static PieceMap witchPiece = {{0,2,0},{2,2,2},{0,2,0}};

const std::vector<PieceMap> pieces = {jPiece, tPiece, iPiece};

struct GamePiece {
	int8_t x,y,rot;
	PieceMap map;
};

#endif
