#pragma once
#include "Piece.h"


class Pawn : public Piece
{
public:
	// Builder:
	Pawn(char color);
	PieceType type() const override { return PAWN; }
	Piece* clone() const override { return new Pawn(*this); }

	bool move(int start_row, int start_col, int end_row, int end_col) const override;
	bool eat(int start_row, int start_col, int end_row, int end_col) const override;
	bool can_promote(int row) const;
};

