
#include<cstdlib>
#include"stdafx.h"

block::proj block::dirs =

{

	{ Z,{ { TL, TC, MR },{ TC, ML, BL } } },
	{ T,{ { TR, TC, ML },{ TC, MR, BR } } },
	{ S,{ { ML, MR, BC },{ TC, MR, BC },{ TC, ML, MR },{ TC, ML, BC } } },
	{ O,{ { TL, TC, ML } } },
	{ L,{ { { ML, BL, MR },{ TC, BC, BR },{ TR, ML, MR },{ TL, TC, BC } } } },
	{ J,{ { ML, BR, MR },{ TR, TC, BC },{ TR, TC, BC },{ TL, MR, ML },{ TC, BC, BL } } },
	{ I,{ { ML, MR,{ 2,0 } },{ TC, BC,{ 0,2 } } } }   // sticks out

};


block::block(int x, int y, shapes sp_) :center(x, y), shape(sp_), status(rand() % dirs[sp_].size())
{
	status_update();
}

void block::fill(bool ** board)
{
	center.fill(board);
	for (int i = 0; i < 3; i++)
	{
		others[i].fill(board);
	}
}

void block::clear(bool **board)
{
	center.clear(board);
	for (int i = 0; i < 3; i++)
	{
		others[i].clear(board);
	}
}

bool block::is_overlapped(bool **board)
{
	if (center.is_overlapped(board))
		return true;
	for (int i = 0; i < 3; i++)
	{
		if (others[i].is_overlapped(board))
			return true;
	}
	return false;
}


void block::status_update()
{
	int i = 0;
	for (auto bros : dirs[shape][status])
	{
		others[i++] = std::pair<int, int>{ bros.first + center.get_x(),bros.second + center.get_y() };
	}
}


bool block::rotate(bool **board)
{
	int orig = status;
	if (++status == dirs[shape].size())
	{
		status = 0;
	}
	status_update();

	if (is_overlapped(board))
	{
		status = orig;
		status_update();
		return false;
	}
	return true;
}

bool block::stepdown(bool **board)
{

	cube orig = center;
	center.get_y() = center.get_y() + 1;
	status_update();
	if (is_overlapped(board))
	{
		center = orig;
		status_update();
		return false;
	}
	return true;
}
bool block::stepleft(bool **board)
{
	cube orig = center;
	center.get_x() -= 1;
	status_update();
	if (is_overlapped(board))
	{
		center = orig;
		status_update();
		return false;
	}
	return true;
}
bool block::stepright(bool **board)
{
	cube orig = center;
	center.get_x() += 1;
	status_update();
	if (is_overlapped(board))
	{
		center = orig;
		status_update();
		return false;
	}
	return true;
}
tetris_board::tetris_board(mode md_) :mode_(md_), level(0), cur(b_cols / 2, 3, block::shapes(rand() % 7)),dead(false)
{
	board = new bool *[b_cols];
	for (int i = 0; i < b_cols; i++)
	{
		board[i] = new bool[b_rows];
		for (int j = 0; j < b_rows; j++)
			board[i][j] = false;
	}
}


void tetris_board::place()
{
   	cur.fill(board);
	cur = block(b_cols / 2, 3, block::shapes(rand() % 7));
	if (cur.is_overlapped(board))
	{
		dead = true;
	}
}

bool tetris_board::stepin()
{
	if (dead)
		return false;
	if (cur.stepdown(board))
	{
		return true;
	}

	place();

	eliminate();

	return false;
}
bool tetris_board::shape_rotate()
{
	if (cur.rotate(board))
	{
		return true;
	}
	return false;
}
bool tetris_board::shape_left()
{
	if (dead)
		return false;
	if (cur.stepleft(board))
	{
		return true;
	}
	return false;
}
bool tetris_board::shape_right()
{
	if (dead)
		return false;
	if (cur.stepright(board))
	{
		return true;
	}
	return false;
}
void tetris_board::space_down()
{
	while (stepin());
}
int tetris_board::eliminate()
{

	for (int k = b_rows - 1; k > 3; k--)
	{
		bool full = true;
		for (int i = 0; i < b_cols; i++)
		{
			if (board[i][k] == false)
			{
				full = false;
				break;
			}
		}

		if (full)
		{
			for (int i = k; i > 0; i--)
			{
				for (int j = 0; j < b_cols; j++)
				{
					board[j][i] = board[j][i - 1];
				}
			}
			return eliminate() + 1;
		}
	}
	return 0;

}
void tetris_board::display(HWND hwnd_, int Z_x, int Z_y)
{
	if (dead)
		SetWindowText(hwnd_, L"胜败乃兵家常事，大侠请重新来过");
	bool shaped = false;
	if (!cur.is_overlapped(board))
	{
		cur.fill(board);
		shaped = true;
	}
	for (int i = 0; i < b_cols; i++)
	{


		for (int j = 0; j < b_rows; j++)
		{
			if (board[i][j])
			{
				HDC hdc = GetDC(hwnd_);
				RECT cube{ Z_x + i*x_width,Z_y + j*y_width,Z_x + (i + 1)*x_width,Z_y + (j + 1)* y_width };
				HBRUSH hb = CreateSolidBrush(RGB(92, 167, 186));
				FillRect(hdc, &cube, hb);
				ReleaseDC(hwnd_, hdc);
				DeleteObject(hb);
			}
			else
			{
				HDC hdc = GetDC(hwnd_);
				RECT cube{ Z_x + i*x_width,Z_y + j*y_width,Z_x + (i + 1)*x_width,Z_y + (j + 1)* y_width };
				HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
				FillRect(hdc, &cube, hb);
				ReleaseDC(hwnd_, hdc);
				DeleteObject(hb);
			}

		}
	}

	if (shaped)
		cur.clear(board);
}

//Vector Version
/*
block::proj block::dirs

{
{ { TL, TC, MR },{ TC, ML, BL } } ,
{ { TR, TC, ML },{ TC, MR, BR } } ,
{ { ML, MR, BC },{ TC, MR, BC },{ TC, ML, MR },{ TC, ML, BC } } ,
{ { TL, TC, ML } } ,
{ { ML, BL, MR },{ TC, BC, BR },{ TR, ML, MR },{ TL, TC, BC } } ,
{ { ML, BR, MR },{ TR, TC, BC },{ TR, TC, BC },{ TL, MR, ML },{ TC, BC, BL } } ,
{ {ML, MR,{ 2,0 }},{ TC, BC,{ 0,2 } } }    // sticks out
} ;
*/

