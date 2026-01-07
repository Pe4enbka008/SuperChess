#include "Board.h"

// Pieces:
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"


/// <summary>
/// Constructor - sets the board with pieces in starting positions
/// </summary>
Board::Board()
{
    // for en prosant
    this->white_col = -1;
    this->black_col = -1;

    // set board
    for (int i = 0; i < 8; i++)
        for (int t = 0; t < 8; t++)
            pieces[i][t] = nullptr;

    // pawns
    for (int t = 0; t < 8; t++)
    {
        pieces[1][t] = new Pawn('w'); // white pawns
        pieces[6][t] = new Pawn('b'); // black pawns
    } // for

    // rooks
    pieces[0][0] = new Rook('w');
    pieces[0][7] = new Rook('w');
    pieces[7][0] = new Rook('b');
    pieces[7][7] = new Rook('b');

    // knights
    pieces[0][1] = new Knight('w');
    pieces[0][6] = new Knight('w');
    pieces[7][1] = new Knight('b');
    pieces[7][6] = new Knight('b');

    // bishops
    pieces[0][2] = new Bishop('w');
    pieces[0][5] = new Bishop('w');
    pieces[7][2] = new Bishop('b');
    pieces[7][5] = new Bishop('b');

    // queens and kings
    pieces[0][4] = new Queen('w');
    pieces[0][3] = new King('w');
    pieces[7][4] = new Queen('b');
    pieces[7][3] = new King('b');

} // __init__

/// <summary>
/// Deconstructor - frees allocated pieces
/// </summary>
Board::~Board()
{
    this->white_col = -1;
    this->black_col = -1;

    // Free allocated pieces
    for (int i = 0; i < 8; i++)
        for (int t = 0; t < 8; t++)
        {
            delete pieces[i][t];
            pieces[i][t] = nullptr;
        } // for
} // delete


// Moving/Eating

/// <summary>
/// Checks if any piece exists at the space specified
/// Without check for out-of-range
/// </summary>
/// <param name="row">row to check - index-based (0-7)</param>
/// <param name="col">col to check - index-based (0-7)</param>
/// <returns>If a piece exists and the place specified</returns>
bool Board::piece_exists_at(int row, int col) const
{ return this->pieces[row][col] != nullptr; }

/// <summary>
/// Returns piece at place specified
/// Without check for out-of-range
/// </summary>
/// <param name="row">row to check - index-based (0-7)</param>
/// <param name="col">col to check - index-based (0-7)</param>
/// <returns>The piece the place specified</returns>
Piece* Board::get_piece_at(int row, int col) const
{ return this->pieces[row][col]; }



/// <summary>
/// The function checks if the piece can move, doesn't check that another piece exists at the end
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the space to move to - index-based (0-7)</param>
/// <param name="end_col">col of the space to move to - index-based (0-7)</param>
/// <returns>ILLEGAL_MOVE - if unsuccessful / VALID - is successful</returns>
int Board::check_move(int start_row, int start_col, int end_row, int end_col) const
{
    if (!this->is_in_range(start_row) || !this->is_in_range(start_col) || !this->is_in_range(end_row) || !this->is_in_range(end_col))
        return false;

    Piece* piece = this->pieces[start_row][start_col];
    bool can_move = piece->move(start_row, start_col, end_row, end_col);
    if (!can_move)
        return ILLEGAL_MOVE;  // illegal movement of the piece
    return VALID;  /// basic, without check for a check
} // check_move

/// <summary>
/// The function moves the piece, supposes that the piece is there, and the space to move to is empty
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the space to move to - index-based (0-7)</param>
/// <param name="end_col">col of the space to move to - index-based (0-7)</param>
void Board::move(int start_row, int start_col, int end_row, int end_col)
{
    Piece* piece = this->pieces[start_row][start_col];

    // for en_passant   - delete prev move of PAWN
    if (piece->get_colour() == 'b')
        this->black_col = -1;  // same col
    else 
        this->white_col = -1;  // same col

    this->pieces[start_row][start_col] = nullptr;
    this->pieces[end_row][end_col] = piece;

    if (piece->type() == PAWN && start_row == 1 || start_row == 6)  // for en_passant
    {
        if (start_row == 6 && start_row - 2 == end_row)
            this->black_col = start_col;  // same col
        else if (start_row == 1 && start_row + 2 == end_row)
            this->white_col = start_col;  // same col
    } // if
} // check_move



/// <summary>
/// The function check if the piece can eat another piece, doesn't check that what piece exists at the and
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the piece to eat - index-based (0-7)</param>
/// <param name="end_col">col of the piece to eat - index-based (0-7)</param>
/// <returns>ILLEGAL_MOVE - if unsuccessful / VALID - is successful</returns>
int Board::check_eat(int start_row, int start_col, int end_row, int end_col) const
{
    if (!this->is_in_range(start_row) || !this->is_in_range(start_col) || !this->is_in_range(end_row) || !this->is_in_range(end_col))
        return false;

    Piece* piece = this->pieces[start_row][start_col];
    bool can_move = piece->eat(start_row, start_col, end_row, end_col);
    if (!can_move)
        return ILLEGAL_MOVE;  // illegal movement of the piece
    return VALID;  /// basic, without check for a check
} // check_eat

/// <summary>
/// The function moves the piece and deletes the second piece, supposes that both pieces are there
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the piece to eat - index-based (0-7)</param>
/// <param name="end_col">col of the piece to eat - index-based (0-7)</param>
void Board::eat(int start_row, int start_col, int end_row, int end_col)
{
    Piece* piece = this->pieces[start_row][start_col];

    // for en_passant   - delete prev move of PAWN
    if (piece->get_colour() == 'b')
        this->black_col = -1;  // same col
    else
        this->white_col = -1;  // same col

    this->pieces[start_row][start_col] = nullptr;
    delete(pieces[end_row][end_col]);   // ate
    this->pieces[end_row][end_col] = piece;
} // check_eat



/// <summary>
/// The function check if the pawn can eat another pawn that jumped
/// </summary>
/// <param name="start_row">row of the pawn to move - index-based (0-7)</param>
/// <param name="start_col">col of the pawn to move - index-based (0-7)</param>
/// <param name="end_row">row of the pawn to eat - index-based (0-7)</param>
/// <param name="end_col">col of the pawn to eat - index-based (0-7)</param>
/// <returns>ILLEGAL_MOVE - if unsuccessful / VALID - is successful</returns>
bool Board::check_for_en_passant_move(int start_row, int start_col, int end_row, int end_col) const
{
    if (start_row != 3 && start_row != 4)   // must be 2 lines before starting possition (1, 6) -> (3, 4)
        return false;
    if (!(end_col - 1 == start_col || end_col + 1 == start_col))   // eats diagonally
        return false;
    if (end_col != this->black_col && end_col != this->white_col)   // ate not last-en-passant-moved pawn
        return false;

    // White logic - eaten:
    if (end_col == this->white_col && end_row == 2)
    {
        Piece* black_pawn = this->pieces[start_row][start_col];
        if (black_pawn->get_colour() == 'b')   // not same colour eating
            return this->piece_exists_at(end_row + 1, end_col) && pieces[end_row + 1][this->white_col]->get_colour() == 'w';  // +1 to go higher
    } // if

    // Black logic - eaten:
    else if (end_col == this->black_col && end_row == 5)
    {
        Piece* white_pawn = this->pieces[start_row][start_col];
        if (white_pawn->get_colour() == 'w')   // not same colour eating
            return this->piece_exists_at(end_row - 1, end_col) && pieces[end_row - 1][this->black_col]->get_colour() == 'b';  // -1 to go lower
    } // else if

    return false;
} // check_for_en_passant_move


/// <summary>
/// The function check if the pawn can eat another pawn that jumped
/// </summary>
/// <param name="start_row">row of the pawn to move - index-based (0-7)</param>
/// <param name="start_col">col of the pawn to move - index-based (0-7)</param>
/// <param name="end_row">row of the pawn to eat - index-based (0-7)</param>
/// <param name="end_col">col of the pawn to eat - index-based (0-7)</param>
void Board::en_passant_move(int start_row, int start_col, int end_row, int end_col)
{
    // White logic - eaten:
    if (end_col == this->white_col && end_row == 2)
    {
        delete(this->pieces[end_row + 1][this->white_col]);   // free memory 
        this->pieces[end_row + 1][this->white_col] = nullptr; // eaten

        this->pieces[end_row][end_col] = this->pieces[start_row][start_col];
        this->pieces[start_row][start_col] = nullptr;   // remove from the prev place
    } // if

    // Black logic - eaten:
    else if (end_col == this->black_col && end_row == 5)
    {
        delete(this->pieces[end_row - 1][this->black_col]);   // free memory 
        this->pieces[end_row - 1][this->black_col] = nullptr; // eaten

        this->pieces[end_row][end_col] = this->pieces[start_row][start_col];
        this->pieces[start_row][start_col] = nullptr;   // remove from the prev place
    } // else if
} // en_passant_move



/// <summary>
/// Undoes a move, putting back the eaten piece if any, no checks done
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the piece to eat - index-based (0-7)</param>
/// <param name="end_col">col of the piece to eat - index-based (0-7)</param>
/// <param name="eaten_piece">Pointer to a piece that was eaten if any</param>
void Board::undo_move(int start_row, int start_col, int end_row, int end_col, Piece* eaten_piece)
{
    Piece* piece = this->pieces[end_row][end_col];
    if (!piece) return; // make sure piece is moved
    this->pieces[start_row][start_col] = piece;
    this->pieces[end_row][end_col] = eaten_piece;
} // undo_move


// Specials

/// <summary>
/// Checks if the rook can castle with the king (both haven't moved), supposes that on the coords of rook is a rook
/// </summary>
/// <param name="rook_row">Row where rook stands (7/0)</param>
/// <param name="rook_col">Col where rook stands (7/0)</param>
/// <param name="move_to_row">Row where rook goes to (7/0)</param>
/// <param name="move_to_col">Col where rook goes to (7/0)</param>
/// <returns>True, if castling can be done</returns>
bool Board::can_castle(int rook_row, int rook_col, int move_to_row, int move_to_col) const
{
    Piece* rook = this->get_piece_at(rook_row, rook_col);
    int king_row = move_to_row, king_col = move_to_col;
    Piece* king = this->get_piece_at(move_to_row, king_col);

    if (!king)
    {
        if (rook_col < king_col)
            king_col++;
        else
            king_col--;
        king = this->get_piece_at(king_row, king_col);

        if (!king)
            return false;
    } // if
    else // got to the king, moving next to the king
    {
        if (rook_col > move_to_col)
            move_to_col++;
        else
            move_to_col--;
    } // else

    if (king->type() != KING)
        return false;  // the space between the king and the rook must be enpty

    if (king->get_state() || rook->get_state())  // one of them had moved
        return false;   // also means that they are on one line and correct positions

    if (this->is_king_attacked(king_row, king_col))
        return false;   // cannot castle when attacked

    // check that there are no pieces on the way of the rook
    return this->free_way(rook_row, rook_col, king_row, king_col);
} // can_castle

/// <summary>
/// Does castling for rook and king. Rook jumps to the king, supposes that all the checks were completed
/// Cannot be undone
/// </summary>
/// <param name="rook_row">Row where rook stands (7/0)</param>
/// <param name="rook_col">Col where rook stands (7/0)</param>
void Board::castle(int rook_row, int rook_col)
{
    int king_col = 3;
    int move_rook_to = 2, move_king_to = 1;
    if (rook_col > king_col)
    {
        move_rook_to += 2; // 4
        move_king_to = 5;
    } // if

    this->move(rook_row, rook_col, rook_row, move_rook_to);   // move rook
    this->move(rook_row, king_col, rook_row, move_king_to);  // jump kings
} // castle

/// <summary>
/// Undoes castling for rook and king. Returns the pieces to their places, supposes that all the checks were completed
/// Cannot be undone
/// </summary>
/// <param name="rook_row">Row where rook stands (7/0)</param>
/// <param name="rook_col">Col where rook stands (7/0)</param>
void Board::undo_castle(int rook_row, int rook_col)
{
    int king_col = 3;
    int move_rook_to = 2, move_king_to = 1;
    if (rook_col > king_col)
    {
        move_rook_to += 2; // 4
        move_king_to = 5;
    } // if

    this->undo_move(rook_row, rook_col, rook_row, move_rook_to, nullptr);   // move rook
    this->undo_move(rook_row, king_col, rook_row, move_king_to, nullptr);  // jump kings
} // castle



/// <summary>
/// Checks if on the col given is a pawn colour given that can be promoted
/// </summary>
/// <param name="col">col of the pawn</param>
/// <param name="colour">colour of the pawn</param>
/// <returns>If a ppawn of colour and on the col given can be promoted</returns>
bool Board::check_for_promotion(int col, char colour) const
{
    int row = 7;
    switch (colour)
    {
    case 'w':
        break;
    case 'b':
        row = 0;
        break;
    default:
        return false;
    } // switch-case

    // get the pawn:
    Piece* piece = this->pieces[row][col];
    return piece != nullptr && piece->type() == PAWN;

} // check_for_promotion

/// <summary>
/// Promotes the pawn and the col given, check that the pawn exists
/// starting posiotions: 0 - w, 7 - b
/// </summary>
/// <param name="col">col where the pawn promotes</param>
/// <param name="colour">colour of the pawn</param>
/// <param name="piece_type">Piece type to which the pawn promotes to</param>
void Board::promotion(int col, char colour, PieceType piece_type)
{
    int row = 7;
    switch (colour)
    {
    case 'w':
        break;
    case 'b':
        row = 0;
        break;
    default:
        return;
    } // switch-case

    // get the pawn:
    Piece* piece = this->pieces[row][col];
    if (piece == nullptr) return;
    if (piece->type() != PAWN || piece->get_colour() != colour) return;   // check for the piece being the pawn and the colour is correct

    Piece* promote_to;
    switch (piece_type)
    {
    case QUEEN:
        promote_to = new Queen(colour);
        break;
    case BISHOP:
        promote_to = new Bishop(colour);
        break;
    case ROOK:
        promote_to = new Rook(colour);
        break;
    case KNIGHT:
        promote_to = new Knight(colour);
        break;
    default:
        return;  // cannot promote into another pawn or king
    } // switch-case

    delete(piece);  // clear memory
    this->pieces[row][col] = promote_to;
} // promotion



// Checkings

/// <summary>
/// Checks that the two pieces anr different colours
/// </summary>
/// <param name="start_row">row of a piece - index-based (0-7)</param>
/// <param name="start_col">col of a piece - index-based (0-7)</param>
/// <param name="end_row">row of a piece - index-based (0-7)</param>
/// <param name="end_col">col of a piece - index-based (0-7)</param>
/// <returns>True if the colours are different</returns>
bool Board::pieces_different_colours(int start_row, int start_col, int end_row, int end_col) const
{
    Piece* start_piece = this->get_piece_at(start_row, start_col);
    Piece* end_piece = this->get_piece_at(end_row, end_col);
    return start_piece->get_colour() != end_piece->get_colour();
} // pieces_different_colours


// free-way chekings

/// <summary>
/// Checks that there are no pieces in the way of a piece
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the space to move to - index-based (0-7)</param>
/// <param name="end_col">col of the space to move to - index-based (0-7)</param>
/// <returns>True if the piece is free to go</returns>
bool Board::free_way(int start_row, int start_col, int end_row, int end_col) const
{
    switch (this->pieces[start_row][start_col]->type()) // PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
    {
        case KING:
            return true;  // always free way
        case KNIGHT:
            return true;  // always free way

        case PAWN:
        {
            int direction = -1;
            if (this->pieces[start_row][start_col]->get_colour() == 'w')
                direction = 1;

            if (start_col == end_col) // moving, not eating
            {
                if (end_row - start_row == direction) // one step
                    return true;
                return !this->piece_exists_at(start_row + direction, start_col);
            } // if
            return true; // eating - no need to check free way
        } // case

        case ROOK:
            return this->free_way_rook(start_row, start_col, end_row, end_col);
        case BISHOP:
            return this->free_way_bishop(start_row, start_col, end_row, end_col);
        case QUEEN:
            return this->free_way_rook(start_row, start_col, end_row, end_col) || this->free_way_bishop(start_row, start_col, end_row, end_col);
        default:
            return false;
    } // switch-case
    return false;
} // free_way


/// <summary>
/// Checks that there are no pieces in the way of a rook-like movement
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the space to move to - index-based (0-7)</param>
/// <param name="end_col">col of the space to move to - index-based (0-7)</param>
/// <returns>True if the piece is free to go</returns>
bool Board::free_way_rook(int start_row, int start_col, int end_row, int end_col) const
{
    if (start_row == end_row && start_col == end_col) 
        return false;

    if (start_col == end_col)
    {
        if (start_row > end_row)
        {
            for (int i = start_row - 1; i > end_row; i--)
                if (this->pieces[i][end_col] != nullptr)
                    return false;
            return true;
        } // if
        
        if (start_row < end_row)
        {
            for (int i = start_row + 1; i < end_row; i++)
                if (this->pieces[i][end_col] != nullptr)
                    return false;
            return true;
        } // if
    } // if

    if (start_row == end_row)
    {
        if (start_col > end_col)
        {
            for (int i = start_col - 1; i > end_col; i--)
                if (this->pieces[end_row][i] != nullptr)
                    return false;
            return true;
        } // if
        if (start_col < end_col)
        {
            for (int i = start_col + 1; i < end_col; i++)
                if (this->pieces[end_row][i] != nullptr)
                    return false;
            return true;
		} // if
    } // if

    return false;
} // free_way_rook


/// <summary>
/// Checks that there are no pieces in the way of a bishop-like movement
/// </summary>
/// <param name="start_row">row of the piece to move - index-based (0-7)</param>
/// <param name="start_col">col of the piece to move - index-based (0-7)</param>
/// <param name="end_row">row of the space to move to - index-based (0-7)</param>
/// <param name="end_col">col of the space to move to - index-based (0-7)</param>
/// <returns>True if the piece is free to go</returns>
bool Board::free_way_bishop(int start_row, int start_col, int end_row, int end_col) const
{
    if (start_row == end_row && start_col == end_col)
        return false;

	int rows_diff = start_row - end_row;
	int cols_diff = start_col - end_col;

    if (rows_diff == cols_diff && cols_diff < 0)  // 4, 4   5, 5   -1, -1    up right
    {
        for (int i = 1; i < -cols_diff; i++)
            if (this->pieces[start_row + i][start_col + i] != nullptr)
                return false;
        return true;
    } // if
    if (rows_diff == cols_diff && cols_diff > 0)  // 4, 4   3, 3   1, 1   down left
    {
        for (int i = 1; i < cols_diff; i++)
            if (this->pieces[start_row - i][start_col - i] != nullptr)
                return false;
        return true;
    } // if
        
    if (-rows_diff == cols_diff && cols_diff > 0)  // down right
    {
        for (int i = 1; i < cols_diff; i++)
            if (this->pieces[start_row + i][start_col - i] != nullptr)
                return false;
        return true;
    } // if
    if (-rows_diff == cols_diff && cols_diff < 0)  // up left
    {
        for (int i = 1; i < -cols_diff; i++)
            if (this->pieces[start_row - i][start_col + i] != nullptr)
                return false;
        return true;
    } // if

    return false;
} // free_way_bishop



// is-the-king-about-to-be-eaten checkings

// CHECKS

/// <summary>
/// The function find the coords of the king, colour of who is specified
/// </summary>
/// <param name="colour">Colour of the king</param>
/// <param name="king_row">row to save the value in - index-based (0-7)</param>
/// <param name="king_col">col to save the value in - index-based (0-7)</param>
void Board::find_king(char colour, int& king_row, int& king_col) const
{
    for (int i = 0; i < 8; i++)
        for (int t = 0; t < 8; t++)
        {
            Piece* piece = pieces[i][t];
            if (piece == nullptr)
                continue;

            if (piece->type() == KING && piece->get_colour() == colour)
            {
                king_row = i;
                king_col = t;
                return;
            } // if
        } // for
    king_row = -1;
    king_col = -1;
} // find_king


/// <summary>
/// The function check that the king on the coordinates aren't attacked
/// Doesn't check that the value is king (may be changed)
/// </summary>
/// <param name="king_row">row where king is - index-based (0-7)</param>
/// <param name="king_col">col where king is - index-based (0-7)</param>
/// <returns>If king is under attack</returns>
bool Board::is_king_attacked(int king_row, int king_col) const
{
    if (!this->is_in_range(king_row) || !this->is_in_range(king_col))
        return true;  // no king found, so undo the move
    if (!this->piece_exists_at(king_row, king_col))
        return true;  // no king found, so undo the move

    char king_colour = this->pieces[king_row][king_col]->get_colour();
    bool under_attack = false;
    int check_row, check_col;

    // check knight's spots:
    check_row = king_row + 2; check_col = king_col + 1;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    check_row = king_row + 2; check_col = king_col - 1;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    check_row = king_row - 2; check_col = king_col - 1;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    check_row = king_row - 2; check_col = king_col + 1;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    // mid-check for attack (by knight)
    if (under_attack)
        return true;

    check_row = king_row + 1; check_col = king_col + 2;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    check_row = king_row + 1; check_col = king_col - 2;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    check_row = king_row - 1; check_col = king_col - 2;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    check_row = king_row - 1; check_col = king_col + 2;
    if (this->is_in_range(check_row) && this->is_in_range(check_col))
        under_attack = under_attack || this->is_knight_attacking(check_row, check_col, king_colour);

    // mid-check for attack (by knight)
    if (under_attack)
        return true;


    
    // check the 8 directions:

    // Liniar (queens and rooks and opposite king (if in close range))
    // check rows:
    for (check_row = king_row - 1; check_row >= 0; check_row--) // check up-wards
        if (this->pieces[check_row][king_col] != nullptr)
        {
            bool check_king = this->pieces[check_row][king_col]->type() == KING && check_row == king_row - 1;
            under_attack = under_attack || this->is_rook_attacking(check_row, king_col, king_colour, check_king);
            break;
        } // if
    
    for (check_row = king_row + 1; check_row <= 7; check_row++) // check down-wards
        if (this->pieces[check_row][king_col] != nullptr)
        {
            bool check_king = this->pieces[check_row][king_col]->type() == KING && check_row == king_row + 1;
            under_attack = under_attack || this->is_rook_attacking(check_row, king_col, king_colour, check_king);
            break;
        } // if
    
    // check cols:
    for (check_col = king_col - 1; check_col >= 0; check_col--) // check left-wards
        if (this->pieces[king_row][check_col] != nullptr)
        {
            bool check_king = this->pieces[king_row][check_col]->type() == KING && check_col == king_col - 1;;
            under_attack = under_attack || this->is_rook_attacking(king_row, check_col, king_colour, check_king);
            break;
        } // if

    for (check_col = king_col + 1; check_col <= 7; check_col++) // check right-wards
        if (this->pieces[king_row][check_col] != nullptr)
        {
            bool check_king = this->pieces[king_row][check_col]->type() == KING && check_col == king_col + 1;;
            under_attack = under_attack || this->is_rook_attacking(king_row, check_col, king_colour, check_king);
            break;
        } // if

    // mid-check for attack (by rooks/queens)
    if (under_attack)
        return true;


    // Diagonal (queens and bishops and pawns (if in close range) and opposite king (if in close range))
    for (int i = 1; i <= 7; i++) // check up-right
    {
        check_row = king_row + i; check_col = king_col + i;
        if (!this->is_in_range(check_row) || !this->is_in_range(check_col))  // out of the board
            break;
        if (this->pieces[check_row][check_col] != nullptr)
        {
            bool check_king = this->pieces[check_row][check_col]->type() == KING && check_row == king_row + 1;  // both are changed fo the same number
            bool check_pawn = this->pieces[check_row][check_col]->type() == PAWN;
            if (check_pawn)
            {
                Piece* pawn = this->pieces[check_row][check_col];
                bool check_black = pawn->get_colour() == 'b' && check_row == king_row + 1;
                bool check_white = pawn->get_colour() == 'w' && check_row == king_row - 1;
                check_pawn = check_black || check_white;
            } // if
            under_attack = under_attack || this->is_bishop_attacking(check_row, check_col, king_colour, check_pawn, check_king);
            break;
        } // if
    } // for

    for (int i = 1; i <= 7; i++) // check down-left
    {
        check_row = king_row - i; check_col = king_col - i;
        if (!this->is_in_range(check_row) || !this->is_in_range(check_col))  // out of the board
            break;
        if (this->pieces[check_row][check_col] != nullptr)
        {
            bool check_king = this->pieces[check_row][check_col]->type() == KING && check_row == king_row - 1;  // both are changed fo the same number
            bool check_pawn = this->pieces[check_row][check_col]->type() == PAWN;
            if (check_pawn)
            {
                Piece* pawn = this->pieces[check_row][check_col];
                bool check_black = pawn->get_colour() == 'b' && check_row == king_row + 1;
                bool check_white = pawn->get_colour() == 'w' && check_row == king_row - 1;
                check_pawn = check_black || check_white;
            } // if
            under_attack = under_attack || this->is_bishop_attacking(check_row, check_col, king_colour, check_pawn, check_king);
            break;
        } // if
    } // for

    for (int i = 1; i <= 7; i++) // check down-right
    {
        check_row = king_row + i; check_col = king_col - i;
        if (!this->is_in_range(check_row) || !this->is_in_range(check_col))  // out of the board
            break;
        if (this->pieces[check_row][check_col] != nullptr)
        {
			bool check_king = this->pieces[check_row][check_col]->type() == KING && check_row == king_row + 1;  // both are changed fo the same number
            bool check_pawn = this->pieces[check_row][check_col]->type() == PAWN;
            if (check_pawn)
            {
                Piece* pawn = this->pieces[check_row][check_col];
                bool check_black = pawn->get_colour() == 'b' && check_row == king_row + 1;
                bool check_white = pawn->get_colour() == 'w' && check_row == king_row - 1;
                check_pawn = check_black || check_white;
            } // if
            under_attack = under_attack || this->is_bishop_attacking(check_row, check_col, king_colour, check_pawn, check_king);
            break;
        } // if
    } // for

    for (int i = 1; i <= 7; i++) // check up-left
    {
        check_row = king_row - i; check_col = king_col + i;
        if (!this->is_in_range(check_row) || !this->is_in_range(check_col))  // out of the board
            break;
        if (this->pieces[check_row][check_col] != nullptr)
        {
            bool check_king = this->pieces[check_row][check_col]->type() == KING && check_row == king_row - 1;  // both are changed fo the same number
            bool check_pawn = this->pieces[check_row][check_col]->type() == PAWN;
            if (check_pawn)
            {
                Piece* pawn = this->pieces[check_row][check_col];
                bool check_black = pawn->get_colour() == 'b' && check_row == king_row + 1;
                bool check_white = pawn->get_colour() == 'w' && check_row == king_row - 1;
                check_pawn = check_black || check_white;
            } // if
            under_attack = under_attack || this->is_bishop_attacking(check_row, check_col, king_colour, check_pawn, check_king);
            break;
        } // if
    } // for

    return under_attack;
} // is_king_attacked



/// <summary>
/// Checks that the index given is somewhere between 0 and 7
/// </summary>
/// <param name="index">index to check</param>
/// <returns>If the index is in the range</returns>
bool Board::is_in_range(int index) const   // just to see that the knight isn't off the board
{ return index <= 7 && index >= 0; }

/// <summary>
/// Checks that the space specified has a knight with different colour than the king
/// </summary>
/// <param name="row">row to check - index-based (0-7)</param>
/// <param name="col"col to check - index-based (0-7)></param>
/// <param name="king_colour">king's colour</param>
/// <returns>True if such knight exists at the coords given</returns>
bool Board::is_knight_attacking(int row, int col, char king_colour) const
{
    if (this->piece_exists_at(row, col)) // a piece exists
        return this->pieces[row][col]->type() == KNIGHT && this->pieces[row][col]->get_colour() != king_colour; 
    return false;
} // is_knight_attacking



/// <summary>
/// Checks that the space specified has a rook/queen with different colour than the king
/// </summary>
/// <param name="row">row to check - index-based (0-7)</param>
/// <param name="col"col to check - index-based (0-7)></param>
/// <param name="king_colour">king's colour</param>
/// <returns>True if such rook/queen exists at the coords given</returns>
bool Board::is_rook_attacking(int row, int col, char king_colour, bool check_king) const
{
    if (this->piece_exists_at(row, col)) // a piece exists
    {
        Piece* piece = this->pieces[row][col];
        bool liniar = piece->type() == ROOK && piece->get_colour() != king_colour;
        liniar = liniar || (piece->type() == QUEEN && piece->get_colour() != king_colour);
        liniar = liniar || (piece->type() == KING && piece->get_colour() != king_colour && check_king);  // another king
        return liniar;
    } // if 
    return false;
} // is_knight_attacking


/// <summary>
/// Checks that the space specified has a bishop/queen/pawn with different colour than the king
/// </summary>
/// <param name="row">row to check - index-based (0-7)</param>
/// <param name="col"col to check - index-based (0-7)></param>
/// <param name="king_colour">king's colour</param>
/// <returns>True if such bishop/queen/pawn exists at the coords given</returns>
bool Board::is_bishop_attacking(int row, int col, char king_colour, bool check_pawn, bool check_king) const
{
    if (this->piece_exists_at(row, col)) // a piece exists
    {
        Piece* piece = this->pieces[row][col];

        bool diagonal = piece->type() == BISHOP && piece->get_colour() != king_colour;
        diagonal = diagonal || (piece->type() == QUEEN && piece->get_colour() != king_colour);
        diagonal = diagonal || (piece->type() == PAWN && piece->get_colour() != king_colour && check_pawn);
        diagonal = diagonal || (piece->type() == KING && piece->get_colour() != king_colour && check_king);  // another king
        return diagonal;
    } // if 
    return false;
} // is_knight_attacking



// CHECKMATES

/// <summary>
/// Cheks that the king cannot escape the check in any way
/// </summary>
/// <param name="king_row">row where king is - index-based (0-7)</param>
/// <param name="king_col">col where king is - index-based (0-7)</param>
/// <returns>True if the king cannot move, nor the piece attaking cannot be bloked/eaten, otherwise False</returns>
bool Board::is_king_in_checkmate(int king_row, int king_col)
{
    Piece* king = this->get_piece_at(king_row, king_col);
    if (!king || king->type() != KING)
        return false;

    bool can_escape = can_king_escape_check(king_row, king_col);   // works
    bool can_block = can_block_attack(king_row, king_col);   // works
    return !(can_escape || can_block); 
} // is_king_in_checkmate


/// <summary>
/// Checks if the king can walk somewhere to escape the check
/// </summary>
/// <param name="king_row">row where king is - index-based (0-7)</param>
/// <param name="king_col">col where king is - index-based (0-7)</param>
/// <returns>If the king can escape the check, otherwise False</returns>
bool Board::can_king_escape_check(int king_row, int king_col)
{
    if (!this->piece_exists_at(king_row, king_col) || this->get_piece_at(king_row, king_col)->type() != KING)
        return false;   // no king!

    for (int i = -1; i <= 1; i++)
        for (int t = -1; t <= 1; t++)
        {
            int check_row = king_row + i, check_col = king_col + t;
            if (check_row == king_row && check_col == king_col)
				continue;   // same place

            if (!this->is_in_range(check_row) || !this->is_in_range(check_col))
                continue;   // get next cause the king is by border

            Piece* eat_piece = nullptr;
            if (this->piece_exists_at(check_row, check_col))  // eat logic
            {
                eat_piece = this->get_piece_at(check_row, check_col)->clone();
                if (eat_piece->get_colour() == this->get_piece_at(king_row, king_col)->get_colour())
                    continue;   // my own piece cannot be moved or eaten
                this->eat(king_row, king_col, check_row, check_col);
            } // if
            else this->move(king_row, king_col, check_row, check_col);

            bool under_attack = this->is_king_attacked(check_row, check_col);   // king moved 100%
            this->undo_move(king_row, king_col, check_row, check_col, eat_piece);   // return the pieces to their places

            if (!under_attack) 
                return true;  // all good, found an escape
        } // for
    return false;
} // can_king_escape_check


/// <summary>
/// Checks if any piece can move to resolve a check
/// (either by blocking the attack or capturing the attacker)
/// </summary>
/// <param name="king_row">row where king is - index-based (0-7)</param>
/// <param name="king_col">col where king is - index-based (0-7)</param>
/// <returns>True if any piece can protect the king, otherwise false</returns>
bool Board::can_block_attack(int king_row, int king_col)
{
    vector<std::pair<int, int>> hit_spaces = find_attack_squares(king_row, king_col);

    char king_colour = this->get_piece_at(king_row, king_col)->get_colour();

    for (int start_row = 0; start_row < 8; start_row++)
        for (int start_col = 0; start_col < 8; start_col++)
        {
            Piece* chosen_piece = pieces[start_row][start_col];
            if (!chosen_piece || chosen_piece->get_colour() != king_colour) continue;  // wrong colour

            for (size_t i = 0; i < hit_spaces.size(); i++)  // walk through the saved the spaces
            {
                int end_row = hit_spaces[i].first;
                int end_col = hit_spaces[i].second;

                if (!check_move(start_row, start_col, end_row, end_col) || !free_way(start_row, start_col, end_row, end_col))
                    continue; // cannot legally move

                Piece* eaten_piece = nullptr;
                if (this->piece_exists_at(end_row, end_col))
                {
                    eaten_piece = this->get_piece_at(end_row, end_col)->clone();
                    if (eaten_piece->get_colour() == king_colour)
                        continue; // cannot capture own piece
                    this->eat(start_row, start_col, end_row, end_col);
                } // if
                else
                    this->move(start_row, start_col, end_row, end_col);

                bool still_in_check = is_king_attacked(king_row, king_col);
                undo_move(start_row, start_col, end_row, end_col, eaten_piece);

                if (!still_in_check)
                    return true; // king saved
            } // for
        } // for

    return false;
} // can_block_attack


/// <summary>
/// Returns all squares that can be used to resolve a check, between the king and each attacker and where attackers stand 
/// </summary>
/// <param name="king_row">row where king is - index-based (0-7)</param>
/// <param name="king_col">col where king is - index-based (0-7)</param>
/// <returns>Vector of coordinates that can resolve the check</returns>
vector<std::pair<int, int>> Board::find_attack_squares(int king_row, int king_col) const
{
    vector<std::pair<int, int>> attack_squares;
    if (!piece_exists_at(king_row, king_col)) return attack_squares;

    char king_colour = get_piece_at(king_row, king_col)->get_colour();

    // find all attackers
    vector<std::pair<int, int>> attackers;
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++)
        {
            Piece* piece = pieces[row][col];
            if (!piece || piece->get_colour() == king_colour) continue; // skip friendly

            if ((check_move(row, col, king_row, king_col) || check_eat(row, col, king_row, king_col)) && free_way(row, col, king_row, king_col))
                attackers.push_back({ row, col });   // can eat/move on the space of the KING
        } // for

    // find attacked squares
    for (size_t i = 0; i < attackers.size(); i++)
    {
        int attacked_row = attackers[i].first;
        int attacked_col = attackers[i].second;
        attack_squares.push_back({ attacked_row, attacked_col });  // save the attacker's coords

        Piece* attacker = pieces[attacked_row][attacked_col];
        if (!attacker) continue;   // no attacker found

        // only far-shooting pieces can be blocked (queen, bishop or rook)
        if (attacker->type() == QUEEN || attacker->type() == ROOK || attacker->type() == BISHOP)
        {
            int end_row = attacked_row - king_row;
            int end_col = attacked_col - king_col;

			// calculate the movement step:
            int step_row = 0;
            if (end_row > 0) step_row =1;
			else if (end_row < 0) step_row = -1;

			int step_col = 0;
			if (end_col > 0) step_col = 1;
			else if (end_col < 0) step_col = -1;

			// calculate squares between king and attacker
            int start_row = king_row + step_row;
            int start_col = king_col + step_col;
			while (start_row != attacked_row || start_col != attacked_col) // go to the attacker
            {
                attack_squares.push_back({ start_row, start_col });
                start_row += step_row;
                start_col += step_col;
            } // while
        } // if
    } // for

    return attack_squares;
} // find_attack_squares



// STALEMATES

/// <summary>
/// Checks that the kind has no legal moves
/// </summary>
/// <param name="king_row">row where king is - index-based (0-7)</param>
/// <param name="king_col">col where king is - index-based (0-7)</param>
/// <returns>True if the king cannot move, he isn't attacked and the player has no other piece to legally move, otherwise False</returns>
bool Board::is_king_in_stalemate(int king_row, int king_col)
{
    if (!this->piece_exists_at(king_row, king_col) || this->get_piece_at(king_row, king_col)->type() != KING)
		return false;   // no king!
    if (this->is_king_attacked(king_row, king_col))
        return false;   // king is in check, not stalemate

    bool can_escape = can_king_escape_check(king_row, king_col);   // works
    bool can_block = can_block_attack(king_row, king_col);   // works
	return !(can_escape || can_block);
} // is_king_in_stalemate


// The end :> of the file