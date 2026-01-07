#include "Knight.h"


Knight::Knight(char color) : Piece(color)
{ }


bool Knight::move(int start_row, int start_col, int end_row, int end_col) const
{
	if (start_row + 2 == end_row && start_col + 1 == end_col)
        return true;
    if (start_row + 2 == end_row && start_col - 1 == end_col)
        return true;
    if (start_row - 2 == end_row && start_col - 1 == end_col)
        return true;
    if (start_row - 2 == end_row && start_col + 1 == end_col)
        return true;

    if (start_row + 1 == end_row && start_col + 2 == end_col)
        return true;
    if (start_row + 1 == end_row && start_col - 2 == end_col)
        return true;
    if (start_row - 1 == end_row && start_col - 2 == end_col)
        return true;
    if (start_row - 1 == end_row && start_col + 2 == end_col)
        return true;
    return false;
} // move


