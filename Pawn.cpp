#include "Pawn.h"


Pawn::Pawn(char color) : Piece(color)
{ }

/// <summary>
/// INDEXES!
/// </summary>
/// <param name="start_row"></param>
/// <param name="start_col"></param>
/// <param name="end_row"></param>
/// <param name="end_col"></param>
/// <returns></returns>
bool Pawn::move(int start_row, int start_col, int end_row, int end_col) const
{
    if (this->get_colour() == 'w')
    {
        if (start_row + 2 == end_row && start_col == end_col && start_row == 1)
            return true;
        if (start_row + 1 == end_row && start_col == end_col)
            return true;
    } // if
    else
    {
        if (start_row - 2 == end_row && start_col == end_col && start_row == 6)
            return true;
        if (start_row - 1 == end_row && start_col == end_col)
            return true;
    } // else
    return false;
}  // move

bool Pawn::eat(int start_row, int start_col, int end_row, int end_col) const
{
    if (this->get_colour() == 'w')
    {
        if (start_row + 1 == end_row && start_col + 1 == end_col) 
            return true;
        if (start_row + 1 == end_row && start_col - 1 == end_col)
            return true;
    } // if
    else
    {
        if (start_row - 1 == end_row && start_col - 1 == end_col)
            return true;
        if (start_row - 1 == end_row && start_col + 1 == end_col)
            return true;
    } // else
    return false;
}  // eat


/// <summary>
/// INDEXES!
/// </summary>
/// <param name="row"></param>
/// <returns></returns>
bool Pawn::can_promote(int row) const
{
    return (this->get_colour() == 'w' && row == 7) || (this->get_colour() == 'b' && row == 0);
} // can_promote


