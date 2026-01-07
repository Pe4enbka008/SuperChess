#pragma once
#include "Piece.h"


class Rook : public virtual Piece
{
public:
	// Builder:
	Rook(char color);
	PieceType type() const override { return ROOK; }
	Piece* clone() const override { return new Rook(*this); }

	bool move(int start_row, int start_col, int end_row, int end_col) const override;
}; 

