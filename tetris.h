#pragma once
#include<Windows.h>
#include<vector>
#include<map>
#include<utility>
const   std::pair<int, int>   TL{ -1,-1 };      /* top left */
const   std::pair<int, int>   TC{ 0,-1 };        /* top center */
const   std::pair<int, int>   TR{ 1,-1 };     /* top right */
const   std::pair<int, int>   ML{ -1,0 };              /* middle left */
const   std::pair<int, int>   MR{ 1,0 };               /* middle right */
const   std::pair<int, int>   BL{ -1,1 };       /* bottom left */
const   std::pair<int, int>   BC{ 0,1 };         /* bottom center */
const   std::pair<int, int>   BR{ 1,1 };       /* bottom right */

static const int b_cols = 12;//x axis
static const int b_rows = 23;//y axis
const int x_width = 25;
const int y_width = 25;


class cube
{
private:
	int pos_y;
	int pos_x;
public:
	cube() :pos_x(0), pos_y(0) {}
	cube(int x, int y) :pos_x(x), pos_y(y) {}
	cube & operator=(const cube & b) { pos_y = b.pos_y;  pos_x = b.pos_x; return *this; }
	cube & operator=(const std::pair<int, int> & p) { pos_x = p.first; pos_y = p.second; return *this; }
	~cube() {}
	int & get_x() { return pos_x; }
	int & get_y() { return pos_y; }
	void fill(bool** board)
	{

		board[pos_x][pos_y] = true;
	}
	void clear(bool** board)
	{

		board[pos_x][pos_y] = false;
	}
	bool is_overlapped(bool **board)
	{
		if (pos_x<0 || pos_x >= b_cols || pos_y >= b_rows || board[pos_x][pos_y])
		{
			return true;
		}
		return false;
	}
};
class block
{
public:
	enum shapes {
		I, J, L, O, S, T, Z
	};
private:
	cube center;

	typedef std::map<shapes, std::vector < std::vector<std::pair<int, int> >>> proj;
	static proj dirs;
	int status;
	shapes shape;
	cube others[3];
public:

	block(int x, int y, shapes sp_);
	~block() {}
	block(const block & b) :center(b.center), status(b.status), shape(b.shape)
	{
		status_update();
	}
	block operator=(const block & b)
	{
		center = b.center;
		status = b.status;
		shape = b.shape;
		status_update();
		return *this;
	}
	void fill(bool ** board);
	void clear(bool **board);
	bool rotate(bool **board);
	bool stepdown(bool **board);
	bool stepleft(bool **board);
	bool stepright(bool **board);
	//before these operation, colors in board should be cleared!
	bool is_overlapped(bool ** board);
	void status_update();
};

class tetris_board
{
public:
	enum mode { Easy, Normal, Hard };
private:
	bool ** board;
	int level;
	bool dead;
	mode mode_;
	block cur;
	static const int b_size = b_cols * b_rows;
protected:
	//tetris_board(tetris_board &);
	//virtual tetris_board operator=(tetris_board &);
	//bool is_fit();
	//void new_shape();
public:
	tetris_board(mode md_);
	~tetris_board()
	{
		for (int i = 0; i < b_cols; i++)
		{
			delete[] board[i];
		}
		delete[] board;
	}
	bool stepin();
	void place();
	//cur filled
	int eliminate();
	bool shape_left();
	bool shape_right();
	bool shape_rotate();
	void space_down();
	void display(HWND hwnd_, int Z_x, int Z_y);

};

