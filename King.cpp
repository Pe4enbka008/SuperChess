#include "King.h"


King::King(char color) : Piece(color)
{ this->_moved_once = false; }


bool King::move(int start_row, int start_col, int end_row, int end_col) const
{
    if (start_row + 1 == end_row && start_col == end_col)
        return true;
    if (start_row - 1 == end_row && start_col == end_col)
        return true;
    if (start_row == end_row && start_col + 1 == end_col)
        return true;
    if (start_row == end_row && start_col - 1 == end_col)
        return true;

    if (start_row + 1 == end_row && start_col + 1 == end_col)
        return true;
    if (start_row - 1 == end_row && start_col + 1 == end_col)
        return true;
    if (start_row - 1 == end_row && start_col - 1 == end_col)
        return true;
    if (start_row + 1 == end_row && start_col - 1 == end_col)
        return true;
    return false;
}  // move

