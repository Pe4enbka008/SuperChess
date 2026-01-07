#include "Queen.h"


Queen::Queen(char color) : Piece(color), Rook(color), Bishop(color)
{ }


bool Queen::move(int start_row, int start_col, int end_row, int end_col) const
{
    return Rook::move(start_row, start_col, end_row, end_col) || Bishop::move(start_row, start_col, end_row, end_col);
} // move


