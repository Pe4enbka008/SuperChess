#pragma once
#include "Board.h"
#include "Pipe.h"
#include <string>

using std::string;


class Game 
{
private:
	Board board;
	Pipe pipe_socket;
	char current_turn;


public:
	// Builder:
	Game();
	~Game();

	// Main loop:
	void main_loop();

private:
	// statements
	int move_statement(string coords_from, string coords_to);
	int eat_statement(string coords_from, string coords_to);

	void switch_player();

	// input and translations
	bool confirm_connection();

	string receive_input(int code_to_send);
	char* get_board_as_string() const;

	int transalte_letter(char letter) const;

	PieceType get_promotion_type(char piece_type) const;

	// chess-checks
	bool correct_colour_piece(string from_coords) const;

	bool is_current_player_in_check();
	bool is_current_player_in_checkmate();
	bool is_current_player_in_stalemate();

}; // game

