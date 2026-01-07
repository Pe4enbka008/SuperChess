#pragma once
#include "Rook.h"
#include "Bishop.h"


class Queen : virtual Rook, public Bishop
{
public:
	// Builder:
	Queen(char color);
	PieceType type() const override { return QUEEN; }
	Piece* clone() const override { return new Queen(*this); }

	bool move(int start_row, int start_col, int end_row, int end_col) const override;
};

