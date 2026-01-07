#include "Rook.h"


Rook::Rook(char color) : Piece(color)
{ this->_moved_once = false; }


bool Rook::move(int start_row, int start_col, int end_row, int end_col) const
{ return !(start_row != end_row && start_col != end_col); }


