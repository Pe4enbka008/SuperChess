#pragma once
#include "Piece.h"


class King : public Piece
{
public:
	// Builder:
	King(char color);
	PieceType type() const override { return KING; }
	Piece* clone() const override { return new King(*this); }

	bool move(int start_row, int start_col, int end_row, int end_col) const override;
};

