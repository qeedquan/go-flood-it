/****************************************************************************
** Copyright (C) 2012 Boris Timofeev <mashin87@gmail.com> www.emunix.org
**
**
** This file may be used under the terms of the GNU General Public
** License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "Board.h"
#include <stdlib.h>
#include <time.h>

Board::Board()
{
	Uint32 rmask, gmask, bmask, amask;
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
	board_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, BOARD_SIZE*BLOCK_SIZE, BOARD_SIZE*BLOCK_SIZE, 32, rmask, gmask, bmask, amask);
    if(board_surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
	
	// initialize random seed
	srand (time(NULL));
	
	Generate();
	step_count = 0;
	win = false;
}

Board::~Board()
{
	SDL_FreeSurface(board_surface);
}

void Board::Generate()
{
	for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = rand() % 6;
}

void Board::DrawBlock(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_FillRect(board_surface, rect, SDL_MapRGB(board_surface->format, r, g, b));
}

void Board::DrawBoard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++){
			SDL_Rect rect = {i*BLOCK_SIZE, j*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			switch(board[i][j]){
				case COLOR_BLUE:
					DrawBlock(&rect, 0x00, 0x00, 0xff);
					break;
				case COLOR_GREEN:
					DrawBlock(&rect, 0x00, 0x80, 0x00);
					break;
				case COLOR_ORANGE:
					DrawBlock(&rect, 0xff, 0xa5, 0x00);
					break;
				case COLOR_RED:
					DrawBlock(&rect, 0xff, 0x00, 0x00);
					break;
				case COLOR_VIOLET:
					DrawBlock(&rect, 0xee, 0x82, 0xee);
					break;
				case COLOR_YELLOW:
					DrawBlock(&rect, 0xff, 0xff, 0x00);
					break;
			}
		}
}

void Board::OnRender(SDL_Surface *surf, int x, int y)
{
	DrawBoard();
	SDL_Rect desc;
    desc.x = x;
    desc.y = y;
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = board_surface->w;
    src.h = board_surface->h;
	SDL_BlitSurface(board_surface, &src, surf, &desc);
}

void Board::Reset()
{
	Generate();
	step_count = 0;
	win = false;
}

bool Board::SetColorByCoordinate(int x, int y)
{
	return SetColorByNumber(board[x][y]);
}

bool Board::SetColorByNumber(int color)
{
	if ((color==board[0][0]) || IsLose() || IsWin())
		return false;
	oldcolor = board[0][0];
	ChangeColor(0, 0, color);
	
	step_count++;
	// check for winnings
	win = CheckForWin();
	
	return true;
}

void Board::ChangeColor(int x, int y, int color)
{
	board[x][y] = color;
    if (x - 1 >= 0 && board[x - 1][y] == oldcolor)
        ChangeColor(x - 1, y, color);
    if (x + 1 < BOARD_SIZE && board[x + 1][y] == oldcolor)
        ChangeColor(x + 1, y, color);
    if (y - 1 >= 0 && board[x][y - 1] == oldcolor)
        ChangeColor(x, y - 1, color);
    if (y + 1 < BOARD_SIZE && board[x][y + 1] == oldcolor)
        ChangeColor(x, y + 1, color);
}

bool Board::IsLose(){ 
	if (step_count > 25)
		return true;
	else
		return false;
}
bool Board::IsWin(){
	return win;
}

bool Board::CheckForWin(){
	if (step_count > 25)
		return false;
	for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] != board[0][0])
				return false;
	return true;
}
