#include "Bishop.h"


Bishop::Bishop(char color) : Piece(color)
{ }


bool Bishop::move(int start_row, int start_col, int end_row, int end_col) const
{
    if (end_row - start_row == end_col - start_col) 
        return true;
    if (-(end_row - start_row) == end_col - start_col) 
        return true;
    return false;
}  // move


