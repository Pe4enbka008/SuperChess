#include "Piece.h"


Piece::Piece(char color)
{ this->_color = color; this->_moved_once = false; }


char Piece::get_colour() const
{ return this->_color; }

bool Piece::get_state() const
{ return this->_moved_once; }
void Piece::change_state()
{ this->_moved_once = true; }


bool Piece::eat(int start_row, int start_col, int end_row, int end_col) const
{ return this->move(start_row, start_col, end_row, end_col); }


