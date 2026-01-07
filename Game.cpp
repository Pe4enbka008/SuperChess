#include "Game.h"

#include <iostream>
#include <thread>
#include <stdlib.h>
#include <chrono>

using std::cout;
using std::endl;


Game::Game()
{
	srand(time_t(NULL));
	current_turn = 'w';
} // Game

Game::~Game()
{ this->board.~Board(); } // deconstruct


// Main loop:
/// <summary>
/// Loop of the gaimplay
/// </summary>
void Game::main_loop()
{
	// connect:
	if (!this->confirm_connection())
		return;

    // send the board string
	char* board_string = this->get_board_as_string();
	board_string[8 * 8] = '0';
	board_string[8 * 8 + 1] = '\0';

	this->pipe_socket.sendMessageToGraphics(board_string);
	string msgFromGraphics = this->pipe_socket.getMessageFromGraphics();	// get message from graphics
	bool game_over = msgFromGraphics == "quit";
	int return_code = VALID; // default - no error

	while (!game_over)
	{
		// numbers - rows / 1   letter - cols / 2
		string coords_from = msgFromGraphics.substr(0, 2); // from, length
		string coords_to = msgFromGraphics.substr(2, 2); // from, length

		if (coords_from == coords_to)  // not the same coords
		{
			return_code = SAME_SPACE; // same place
			msgFromGraphics = this->receive_input(return_code);	// send message to and get message from graphics
			game_over = msgFromGraphics == "quit";
			return_code = VALID;
			continue; // while
		} // if

		// check that a piece was chosen
		bool piece_exists = this->board.piece_exists_at(
			coords_from[1] - '0' - 1,   // index based!
			this->transalte_letter(coords_from[0])
		); // piece_exists
		if (!piece_exists)
		{
			return_code = NO_PIECE_SELECTED; // no piece at from_coords
			msgFromGraphics = this->receive_input(return_code);	// send message to and get message from graphics
			game_over = msgFromGraphics == "quit";
			return_code = VALID;
			continue; // while
		} // if
		
		// moving / eating
		piece_exists = this->board.piece_exists_at(
			coords_to[1] - '0' - 1,   // index based!
			this->transalte_letter(coords_to[0])
		); // piece_exists
		if (piece_exists) // eat
			return_code = this->eat_statement(coords_from, coords_to);
		else // move
			return_code = this->move_statement(coords_from, coords_to);

		// finishing touches:
		if (return_code < INCORRECT_PIECE)
			this->switch_player();

		if (return_code < INCORRECT_PIECE && this->is_current_player_in_check()) // check if opposite king is attacked
		{
			if (this->is_current_player_in_checkmate())
				return_code = CHECKMATE;    // code that the opposite king is checkmated
			else return_code = CHECK;   // code that the opposite king is attacked
		} // if
		else if (return_code < INCORRECT_PIECE && this->is_current_player_in_stalemate()) // check if opposite king isn't attacked and has no moves
			return_code = STALEMATE;    // code that the opposite king is stalemated

		msgFromGraphics = this->receive_input(return_code);	// send message to and get message from graphics
		game_over = msgFromGraphics == "quit" || return_code == CHECKMATE;
		return_code = VALID;
	} // while

	pipe_socket.close();
} // main_loop



// checks of the chess:

/// <summary>
/// The function moves a piece from one position to another, suuposes the piece exists there, and the place to go to is empty
/// </summary>
/// <param name="coords_from">string coords of piece to move</param>
/// <param name="coords_to">string coords of place to go to</param>
/// <returns> Returns ErrorType value for the move </returns>
int Game::move_statement(string coords_from, string coords_to)
{
	// not his piece
	if (!this->correct_colour_piece(coords_from))
		return INCORRECT_PIECE; // not his piece


	// convert coordinates
	int start_col = this->transalte_letter(coords_from[0]);
	int start_row = coords_from[1] - '0' - 1;

	int end_col = this->transalte_letter(coords_to[0]);
	int end_row = coords_to[1] - '0' - 1;

	// check move
	int code = this->board.check_move(
		start_row, start_col,
		end_row, end_col
	); // code
	if (code == ILLEGAL_MOVE)  // illegal move
	{
		// check for en passant after trying eating, but an enpty space, if it wasn't successful:
		if (this->board.piece_exists_at(start_row, start_col))
			if (this->board.get_piece_at(start_row, start_col)->type() == PAWN)
				if (this->board.check_for_en_passant_move(start_row, start_col, end_row, end_col))
				{
					this->board.en_passant_move(start_row, start_col, end_row, end_col);
					return EN_PASSANT;
				} // if
		return code;  // if not en passant, then error
	} // if
		

	// does run through other pieces
	if (!this->board.free_way(start_row, start_col, end_row, end_col))
		return ILLEGAL_MOVE;

	// move
	this->board.move(
		start_row, start_col,
		end_row, end_col
	); // move

	// king safety
	if (this->is_current_player_in_check())  // checks if the king is under attack AFTER the move
	{
		this->board.undo_move(start_row, start_col, end_row, end_col, nullptr);  // return the piece back
		return CHECK_IN_PLAY;
	} // if

	// check for promotion after moving:
	if (this->board.get_piece_at(end_row, end_col)->type() == PAWN)
		if (this->board.check_for_promotion(end_col, this->current_turn))
			if (this->board.check_for_promotion(end_col, this->current_turn))
			{
				this->board.promotion(end_col, this->current_turn, this->get_promotion_type(this->receive_input(PROMOTION)[0]));
				return PROMOTION;
			} // if

	this->board.get_piece_at(end_row, end_col)->change_state();  // piece moved
	return code;
} // move_statement


/// <summary>
/// The function moves a piece from one position to another, eating the second piece, suuposes both pieces exist there
/// </summary>
/// <param name="coords_from">string coords of piece to move</param>
/// <param name="coords_to">string coords of piece to eat</param>
/// <returns> Returns ErrorType value for the eat </returns>
int Game::eat_statement(string coords_from, string coords_to)
{
	// not his piece
	if (!this->correct_colour_piece(coords_from))
		return INCORRECT_PIECE; // not his piece


	// convert coordinates
	int start_col = this->transalte_letter(coords_from[0]);
	int start_row = coords_from[1] - '0' - 1;

	int end_col = this->transalte_letter(coords_to[0]);
	int end_row = coords_to[1] - '0' - 1;

	// check castling: (pressed on the king)
	if (this->board.get_piece_at(start_row, start_col)->type() == ROOK
		&& this->board.get_piece_at(end_row, end_col)->type() == KING
		&& (start_row == 7 || start_row == 0))
	{
		// try to castle:
		if (this->board.can_castle(start_row, start_col, end_row, end_col))
		{
			this->board.castle(start_row, start_col); // gets just the rook
			// must chek that the king is not in check after castling:
			if (this->is_current_player_in_check())
			{
				this->board.undo_castle(start_row, start_col); // gets just the rook
				return CHECK_IN_PLAY;
			} // if

			// calculate where rook and king moved to
			int king_col = 3;
			int move_rook_to = 2, move_king_to = 1;
			if (start_col > king_col)
			{
				move_rook_to += 2; // 4
				move_king_to = 5;
			} // if

			this->board.get_piece_at(start_row, move_rook_to)->change_state();  // rook moved
			this->board.get_piece_at(start_row, move_king_to)->change_state();  // king moved
			return CASTLE;  // moved, all good :]
		} // if
		return BAD_CANTLE;  // cannot castle
	} // if


	// eating his own piece
	if (this->correct_colour_piece(coords_to))
		return BUSY_SPACE; // eating his own piece

	// check eat
	int code = this->board.check_eat(
		start_row, start_col,
		end_row, end_col
	); // code
	if (code == ILLEGAL_MOVE)  // illegal move
		return code;

	// does run through other pieces
	if (!this->board.free_way(start_row, start_col, end_row, end_col))
		return ILLEGAL_MOVE;


	Piece* piece_eaten = this->board.get_piece_at(end_row, end_col)->clone();
	// eat
	this->board.eat(
		start_row, start_col,
		end_row, end_col
	); // eat

	// king safety
	if (this->is_current_player_in_check())  // checks if the king is under attack AFTER the move
	{
		this->board.undo_move(start_row, start_col, end_row, end_col, piece_eaten);  // return the pieces back
		return CHECK_IN_PLAY;
	} // if

	// check for promotion after eating:
	if (this->board.get_piece_at(end_row, end_col)->type() == PAWN)
		if (this->board.check_for_promotion(end_col, this->current_turn))
		{
			this->board.promotion(end_col, this->current_turn, this->get_promotion_type(this->receive_input(PROMOTION)[0]));
			return PROMOTION;
		} // if

	this->board.get_piece_at(end_row, end_col)->change_state();  // piece moved
	return code;
} // eat_statement


/// <summary>
/// Switches the current player / from white to black and wise versa
/// </summary>
void Game::switch_player()
{
	if (this->current_turn == 'w')
		this->current_turn = 'b';
	else
		this->current_turn = 'w';
} // switch_player


// input and translations

/// <summary>
/// Confurms the connection
/// </summary>
/// <returns>the connection succeded</returns>
bool Game::confirm_connection()
{
	bool isConnect = pipe_socket.connect();

	string ans;
	while (!isConnect) 
	{
		cout << "Can't connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		std::cin >> ans;

		if (ans == "0")
		{
			cout << "trying connect again.." << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			isConnect = pipe_socket.connect();
		} // if
		else
		{
			pipe_socket.close();
			return false;
		} // else
	} // while
	return true;
} // confirm_connection


/// <summary>
/// Sends a code to the app, recieve an answer from it (code of the next move)
/// </summary>
/// <param name="code_to_send">code from 0 to 8</param>
/// <returns>string ffr the nrxt move (from+to)</returns>
string Game::receive_input(int code_to_send) 
{
	char msgToGraphics[1024];
	msgToGraphics[0] = (char)(code_to_send + '0');

	int zero_index = 1;
	if (msgToGraphics[0] > '9')  // I have codes 10, 11, 12
	{
		msgToGraphics[0] = '1';
		msgToGraphics[1] = (char)(code_to_send % 10 + '0');  // get the socond number
		zero_index++;
	} // if

	msgToGraphics[zero_index] = 0;
	this->pipe_socket.sendMessageToGraphics(msgToGraphics);  // return result to graphics		
	return this->pipe_socket.getMessageFromGraphics();    // get message from graphics
} // receive_input


/// <summary>
/// Returns the board as a string
/// BIG letters are white and small are black
/// r - rook
/// q - queen
/// k - king
/// n - kningt
/// p - pawn
/// b - bishop
/// </summary>
/// <returns>String representing the board</returns>
char* Game::get_board_as_string() const
{
	char return_string[1024];
	for (int i = 0; i < 8; i++)  // row
		for (int t = 0; t < 8; t++)  // col
		{
			Piece* piece = this->board.get_piece_at(i, t);
			if (!piece)
			{
				return_string[i * 8 + t] = '#';
				continue;
			} // if

			char piece_char = 'p';
			switch (piece->type())
			{
			case KING:
				piece_char = 'k';
				break;
			case QUEEN:
				piece_char = 'q';
				break;
			case ROOK:
				piece_char = 'r';
				break;
			case BISHOP:
				piece_char = 'b';
				break;
			case KNIGHT:
				piece_char = 'n';
				break;
			default:
				piece_char = 'p';
			} // switch-case

			if (piece->get_colour() == 'b')
				piece_char = 'A' + (piece_char - 'a');
			return_string[i * 8 + t] = piece_char;
		} // for
	return return_string;
} // get_board_as_string


/// <summary>
/// a = 0
/// b = 1
/// c = 2
/// d = 3
/// e = 4
/// f = 5
/// g = 6
/// h = 7
/// </summary>
/// <param name="letter">a/b/c/d/e/f/g/h</param>
/// <returns>number representing on of the letters</returns>
int Game::transalte_letter(char letter) const
{ return letter - 'a'; }


/// <summary>
/// Gets promotion type from the user as char and translates it.
/// The choice is given to the function using receive_input with code PROMOTION
/// </summary>
/// <returns>PieceType piece type choice</returns>
PieceType Game::get_promotion_type(char piece_type) const
{
	// put the pieces on the board to choose
	PieceType type;
	if (piece_type < 'a')  // big letters are BEFORE the small 
		piece_type = (char)('a' + (piece_type - 'A'));

	switch (piece_type)
	{
	case 'q':
		type = QUEEN;
		break;
	case 'r':
		type = ROOK;
		break;
	case 'n':
		type = KNIGHT;
		break;
	case 'b':
		type = BISHOP;
		break;
	default:
		type = PAWN;
	} // switch-case

	return type;
} // get_promotion_type


// chess-checks

/// <summary>
/// Checks that the piece is the same as the current player
/// </summary>
/// <param name="from_coords">coordinates as string of the piece to check</param>
/// <returns>True if the piece is the same colour as the player</returns>
bool Game::correct_colour_piece(string from_coords) const
{
	Piece* piece = this->board.get_piece_at(from_coords[1] - '0' - 1, this->transalte_letter(from_coords[0]));
	return piece->get_colour() == this->current_turn;
} // correct_colour_piece


/// <summary>
/// Checks if the king of current player is attacked
/// </summary>
/// <returns>True if the king is in danger</returns>
bool Game::is_current_player_in_check()
{
	int king_col, king_row;
	this->board.find_king(this->current_turn, king_row, king_col); // finds king
	if (king_col == -1 || king_col == -1)
		return true;   // smth went REALLY wrong
	return this->board.is_king_attacked(king_row, king_col);   // checks if he's under attack
} // is_current_player_in_check


/// <summary>
/// Checks if the king of current player is attacked and no move can be done to protect it
/// </summary>
/// <returns>True if the current player has no legal moves to protect their king</returns>
bool Game::is_current_player_in_checkmate()
{
	int king_col, king_row;
	this->board.find_king(this->current_turn, king_row, king_col); // finds king
	if (king_col == -1 || king_col == -1)
		return true;   // smth went REALLY wrong
	return this->board.is_king_in_checkmate(king_row, king_col);
} // is_current_player_in_checkmate


/// <summary>
/// Checks if the king of current player is not attacked and no other move can be done
/// </summary>
/// <returns>True if the current player has no legal moves to protect their king</returns>
bool Game::is_current_player_in_stalemate()
{
	int king_col, king_row;
	this->board.find_king(this->current_turn, king_row, king_col); // finds king
	if (king_col == -1 || king_col == -1)
		return true;   // smth went REALLY wrong
	return this->board.is_king_in_stalemate(king_row, king_col);
} // is_current_player_in_stalemate
/// 500! WOW!
