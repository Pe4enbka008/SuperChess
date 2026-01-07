#pragma once
enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

class Piece
{
protected:
	char _color;
	bool _moved_once;  // only for rooks and kings (castling)

public:
	// Builder:
	Piece(char color);
	virtual ~Piece() = default;

	// Setters-Getters
	char get_colour() const; 
	virtual PieceType type() const = 0;
	virtual Piece* clone() const = 0;

	void change_state();
	bool get_state() const;

	virtual bool move(int start_row, int start_col, int end_row, int end_col) const = 0;
	virtual bool eat(int start_row, int start_col, int end_row, int end_col) const;
}; // Piece

