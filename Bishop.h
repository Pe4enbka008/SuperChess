#pragma once
#include "Piece.h"


class Bishop : public virtual Piece
{
public:
	// Builder:
	Bishop(char color);
	PieceType type() const override { return BISHOP; }
	Piece* clone() const override { return new Bishop(*this); }

	bool move(int start_row, int start_col, int end_row, int end_col) const override;
}; 

