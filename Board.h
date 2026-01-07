#pragma once
#include "Piece.h"
#include <vector>

using std::vector;


enum ErrorType {
	VALID, CHECK, CASTLE, PROMOTION, EN_PASSANT,  // good
	INCORRECT_PIECE, NO_PIECE_SELECTED, BUSY_SPACE, CHECK_IN_PLAY,  // bad
	BAD_CANTLE, OUT_OF_BOUNDS, ILLEGAL_MOVE, SAME_SPACE,   // also bad
	CHECKMATE, STALEMATE, IDK  // better
};  // enum ErrorType


class Board
{
private:
	// for en passant thing:
	int white_col;
	int black_col;

	Piece* pieces[8][8];

public:
	// Builder:
	Board();
	~Board();

	// Pieces moving/eating:
	int check_move(int start_row, int start_col, int end_row, int end_col) const;
	void move(int start_row, int start_col, int end_row, int end_col);

	int check_eat(int start_row, int start_col, int end_row, int end_col) const;
	void eat(int start_row, int start_col, int end_row, int end_col);

	bool check_for_en_passant_move(int start_row, int start_col, int end_row, int end_col) const;
	void en_passant_move(int start_row, int start_col, int end_row, int end_col);

	void undo_move(int start_row, int start_col, int end_row, int end_col, Piece* eaten_piece);

	bool can_castle(int rook_row, int rook_col, int move_to_row, int move_to_col) const;
	void castle(int rook_row, int rook_col);
	void undo_castle(int rook_row, int rook_col);


	// checkings
	bool piece_exists_at(int row, int col) const;
	Piece* get_piece_at(int row, int col) const;

	bool check_for_promotion(int col, char colour) const;
	void promotion(int col, char colour, PieceType piece_type);

	bool pieces_different_colours(int start_row, int start_col, int end_row, int end_col) const;

	bool free_way(int start_row, int start_col, int end_row, int end_col) const;

	void find_king(char colour, int& king_row, int& king_col) const;

	bool is_king_attacked(int king_row, int king_col) const;
	bool is_king_in_checkmate(int king_row, int king_col);
	bool is_king_in_stalemate(int king_row, int king_col);

private:
	bool free_way_rook(int start_row, int start_col, int end_row, int end_col) const;
	bool free_way_bishop(int start_row, int start_col, int end_row, int end_col) const;

	bool is_in_range(int index) const;
	bool is_knight_attacking(int row, int col, char king_colour) const;

	bool is_rook_attacking(int row, int col, char king_colour, bool check_king) const;
	bool is_bishop_attacking(int row, int col, char king_colour, bool check_pawn, bool check_king) const;

	bool can_king_escape_check(int king_row, int king_col);
	bool can_block_attack(int king_row, int king_col);
	vector<std::pair<int, int>> find_attack_squares(int king_row, int king_col) const;

}; // game

