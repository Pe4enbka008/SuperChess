#pragma once
#include "Piece.h"


class Knight : public Piece
{
public:
	// Builder:
	Knight(char color);
	PieceType type() const override { return KNIGHT; }
	Piece* clone() const override { return new Knight(*this); }

	bool move(int start_row, int start_col, int end_row, int end_col) const override;
}; 

